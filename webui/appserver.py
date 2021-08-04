import socket
import threading
import ast, json

class ClientThread(threading.Thread):
    def __init__(self, clientAddress, clientsocket, plotting_queue):
        threading.Thread.__init__(self)
        self.csocket = clientsocket
        self._plotting_queue = plotting_queue
        print("client connected")

    def run(self):
        msg = ''
        msg_dict = {}
        self.csocket.setblocking(True)
        while True:
            data = self.csocket.recv(2048)
            if data:
                try:
                    msg = data.decode()
                    msg_dict = ast.literal_eval(msg)
                except:
                    print("can't decode input string '%s'" % msg)

                if "plots" in msg_dict.keys():
                    self._plotting_queue.put(msg_dict["plots"])


class AppServer:
    _server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def __init__(self, addr, port, plotting_queue):
        self._addr = addr
        self._port = port
        self._plotting_queue = plotting_queue

    def runServer(self):
        #self._server.setblocking(0)
        self._server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self._server.bind((self._addr, self._port))
        print("started AppServer on %s %s" % (self._addr, self._port))
        self._server.listen()
        while True:
            clientsock, clientAddress = self._server.accept()
            newthread = ClientThread(clientAddress, clientsock, self._plotting_queue)
            newthread.start()
