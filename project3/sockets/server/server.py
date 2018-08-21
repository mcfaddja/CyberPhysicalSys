from __future__ import print_function

import socketserver
import mysql.connector
from mysql.connector import errorcode
import time
from datetime import datetime, data, timedelta
import re


n = 0

class MyIMUdataHandler(socketserver.StreamRequestHandler):

    def handle(self):
        cnx = mysql.connector.connect(user='imu_user', password='1MckiD4', host='localhost', database='cyberphyssys')
        cnx.database = 'cyberphyssys'

        myTime = datetime.now()
        print(myTime.year, myTime.month, myTime.day, myTime.hour, myTime.minute, myTime.second, myTime.microsecond)
        
        # self.data = self.request.recv(1024).strip()
        self.data = self.rfile.readlines()

        data = []
        for i in range(len(self.data)):
            tmpROW = self.data[i].decode("utf-8")
            # tmpROW = tmpROW[1:]
            tmpROW = re.sub('[\r\n]', '', tmpROW)
            data.append(tmpROW)

        tblName = data[0] + "-" + data[1]
        # print(tblName)
        TABLES = {}
        TABLES[tblName] = (
            "CREATE TABLE `{}` ("
            "   `datapt_id` INT NOT NULL AUTO_INCREMENT,"
            "   `device_id` VARCHAR(45) NOT NULL,"
            "   `sensor_id` VARCHAR(45) NOT NULL,"
            "   `gx` FLOAT NOT NULL,"
            "   `gy` FLOAT NOT NULL,"
            "   `gz` FLOAT NOT NULL,"
            "   `ax` FLOAT NOT NULL,"
            "   `ay` FLOAT NOT NULL,"
            "   `az` FLOAT NOT NULL,"
            "   `mx` FLOAT NOT NULL,"
            "   `my` FLOAT NOT NULL,"
            "   `mz` FLOAT NOT NULL,"
            "   `date_time_pt` DATETIME NOT NULL,"
            "   `mu_s` INT NOT NULL,"
            "   PRIMARY KEY (`datapt_id`),"
            "   UNIQUE INDEX `datapt_id_UNIQUE` (`datapt_id` ASC) VISIBLE"
            ") ENGINE=InnoDB".format(tblName)
        )

        # print(TABLES[tblName])
        cursor = cnx.cursor()
        # for name, ddl in TABLES.iteritems():
        try:
            print("Creating table {}: ".format(tblName), end='')
            cursor.execute(TABLES[tblName])
        except mysql.connector.Error as err:
            if err.errno == errorcode.ER_TABLE_EXISTS_ERROR:
                print("already exists")
            else:
                print(err.msg)
        else:
            print("OK!")

        



        # print(self.data)
        # print(type(self.data))
        print(data)


        cursor.close()
        cnx.close()


if __name__ == "__main__":
    HOST, PORT = "192.168.1.40", 5000

    print("started server!")

    server = socketserver.TCPServer((HOST, PORT), MyIMUdataHandler)

    server.serve_forever()