# pylint: disable=W0613
from sanic import Sanic, response
from sanic.request import Request
from sanic.log import logger
import aiohttp
from urllib.parse import urljoin
import yaml
import time
import os
try:
    import ujson as json
except:
    import orjson as json


from . import selector
from .views.manager import bp as manager_blueprint

Sanic.start_method = os.environ.get('AL_CONFIG_START_METHOD', 'fork')

# Create Sanic Application Instance
app = Sanic("AgileLB", loads=json.loads, dumps=json.dumps)
# Registration Blueprint
app.blueprint(manager_blueprint)

# 加载配置
with open(os.environ.get("AL_CONFIG_PATH", "lb.yml")) as config_file:
    user_config = yaml.safe_load(config_file)
app.config.update(user_config)

# 初始化调度器
servers = app.config.get("servers", {})
app.ctx.sels = {} 
for server_key in servers.keys():
    server = servers[server_key]
    try:
        sel_type = server["type"]
        uris = server["uris"]
        sel = getattr(selector, sel_type)(uris, app.config)
        app.ctx.sels[server_key] = sel
    except AttributeError:
        logger.error(f"Selector type {sel_type} does not exist.")
        raise

# 钩子
async def clean(app, loop):
    logger.info("Cleaning up resources...")
    for sel_key in app.ctx.sels:
        sel = app.ctx.sels[sel_key]
        sel.is_run = False
        sel.check_thread.join()


async def start(app, loop):
    for sel_key in app.ctx.sels:
        sel = app.ctx.sels[sel_key]
        sel.check_thread.start()


app.register_listener(clean, "main_process_stop")
app.register_listener(start, "main_process_start")


# 定义路由处理函数
@app.route("/", name="RootPath")
@app.route("/<path:path>", name="SubPath")
async def handle_request(request: Request, path: str = None):
    start_time = time.time()
    if request.host not in app.ctx.sels:
        return response.json({"code": 403, "msg": "Invalid domain."}, status=403)

    headers = dict(request.headers)
    # 重写X-Forwarded-For
    headers["X-Forwarded-For"] = headers.get(
        "X-Forwarded-For", f'{request.client_ip}:{request.port}'
    ).split(",")[-1]
    # 重写协议
    headers['X-Forwarded-Proto'] = request.scheme
    # 重写请求ID
    if app.config.get('request_id', True):
        headers['X-Request-ID'] = str(request.id)
    
    sel = app.ctx.sels[request.host]
    server = await sel.get(request)
    if not server:
        return response.json({"code": 502, "msg": "No servers available."}, status=502)
    return response.text('ok')
    try:
        st = time.time()
        async with aiohttp.ClientSession() as session:
            async with session.request(
                request.method,
                urljoin(server, f"{request.path}?{request.query_string}"),
                headers=headers,
                timeout=app.config.get("timeout", 5),  # 使用配置项
            ) as resp:
                if app.config.debug:
                    elapsed_time = (time.time() - start_time) * 1000
                    print(f"Request processed in {elapsed_time:.2f}ms")
                return response.raw(await resp.read(), resp.status, resp.headers)

    except Exception as error:
        print(f"Cannot connect to the server: {error}")
        return response.json(
            {"code": 502, "msg": "Cannot connect to the server."}, status=502
        )


# 运行应用
if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8000, debug=app.config.get("debug", False))
