#! /usr/bin/python3
import threading
from webserver import WebServer
from appserver import AppServer
from queue import Queue
import sys

plotting_queue = Queue()

class webUI:
    def parse_config(self):
        ret = {}
        for arg in sys.argv:
            if arg.startswith("APP_SERVER_IP"):
                ret["APP_SERVER_IP"] = arg.split("=")[1]
            if arg.startswith("APP_SERVER_PORT"):
                ret["APP_SERVER_PORT"] = int(arg.split("=")[1])
            if arg.startswith("WEB_SERVER_IP"):
                ret["WEB_SERVER_IP"] = arg.split("=")[1]
            if arg.startswith("WEB_SERVER_PORT"):
                ret["WEB_SERVER_PORT"] = int(arg.split("=")[1])
        return ret


    def run(self):
        config = self.parse_config()
        web_server = WebServer(config["WEB_SERVER_IP"], config["WEB_SERVER_PORT"], plotting_queue)
        socket_server = AppServer(config["APP_SERVER_IP"], config["APP_SERVER_PORT"], plotting_queue)

        threads = [threading.Thread(target=web_server.runServer), threading.Thread(target=socket_server.runServer)]
        for th in threads:
            th.start()
            print(f'threads {th} started')
            th.join(0.1)
        
if __name__ == "__main__":
    webui = webUI()
    webui.run()
