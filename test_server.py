from sanic import Sanic, response
from sys import argv

app = Sanic(__name__)

@app.route("/")
async def j(request):
    return response.text("6")

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=int(argv[1]), debug=False)