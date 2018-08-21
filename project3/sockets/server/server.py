import socketserver
import time
from datetime import datetime

n = 0

class MyIMUdataHandler(socketserver.StreamRequestHandler):

    def handle(self):
        myTime = datetime.now()
        print(myTime.year, myTime.month, myTime.day, myTime.hour, myTime.minute, myTime.second, myTime.microsecond)
        # self.data = self.request.recv(1024).strip()
        self.data = self.rfile.readlines()
        print(self.data)
        print(type(self.data))

if __name__ == "__main__":
    HOST, PORT = "192.168.1.40", 5000

    print("started server!")

    server = socketserver.TCPServer((HOST, PORT), MyIMUdataHandler)

    server.serve_forever()