import json
import random
import time
from datetime import datetime
import ast

from flask import Flask, Response, render_template

debug = False
limit_msgs_rate = True

class WebServer:
    _application = Flask(__name__)
    _plotting_queue = None

    def __init__(self, addr, port, plotting_queue):
        self._addr = addr
        self._port = port
        WebServer._plotting_queue = plotting_queue


    @_application.route("/")
    def index():
        return render_template("index.html")


    def get_new_data():
        i = 0
        while True:
            if (WebServer._plotting_queue.empty()):
                time.sleep(0.01)
                continue
            plots = WebServer._plotting_queue.get()
            
            if limit_msgs_rate:
                i = i + 1
                if i % 5 != 0:
                    continue
                i = 0

            if debug:
                print(str(WebServer._plotting_queue.qsize()) + " " + str(plots))

            json_data = json.dumps(
                {
                    "time": datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f"),
                    "plots": plots,
                }
            )
            yield f"data:{json_data}\n\n"
            # time.sleep(0.1)


    @_application.route("/chart-data")
    def chart_data():
        return Response(WebServer.get_new_data(), mimetype="text/event-stream")

    def runServer(self):
        self._application.run(host=self._addr, port=self._port, debug=False, threaded=True)
