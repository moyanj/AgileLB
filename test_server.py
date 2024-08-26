from flask import Flask, request

app = Flask(__name__)


@app.route("/")
def j():
    print(request.headers)
    print(request.remote_addr,)
    print(request.data)
    return "6"


if __name__ == "__main__":
    app.run(debug=False, port=5000)
