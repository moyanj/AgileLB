from LB.app import app
import multiprocessing

def run():
    cpu = multiprocessing.cpu_count()
    ssl_config = app.config.get('ssl', None)
    if ssl_config is None:
        ssl = None
    else:
        if ssl_config.get('enable', False):
            ssl = ssl_config.get('certs')
        else:
            ssl = None    
    app.run(
        host=app.config.host,
        port=app.config.port,
        debug=app.config.get('debug', False),
        workers=app.config.get('worker', cpu*2+1),
        auto_reload=app.config.get('debug', False),
        dev=app.config.get('debug', False),
        ssl=ssl,
    )


if __name__ == "__main__":
    run()
