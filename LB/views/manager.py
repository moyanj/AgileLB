from sanic import Blueprint
from sanic.request import Request

bp = Blueprint("Manger")


@bp.get("/LoadBalancer/manger")
async def manger(request: Request):
    key = request.args.get("lb_key", None)
    if key is None:
        return ""
