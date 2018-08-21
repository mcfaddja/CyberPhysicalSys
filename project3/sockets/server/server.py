import socketserver

class MyIMUdataHandler(socketserver.StreamRequestHandler):

    def handle(self):
        self.data = self.rfile.readlines().strip()
        print(self.data)
        print(type(self.data))

if __name__ == "__main__":
    HOST, PORT = "localhost", 5000

    print("started server!")
    
    server = socketserver.TCPServer((HOST, PORT), MyIMUdataHandler)

    server.serve_forever()