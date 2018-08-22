from __future__ import print_function

import socketserver
import mysql.connector
from mysql.connector import errorcode
import time
from datetime import datetime, date, timedelta
import re



class MyIMUdataHandler(socketserver.StreamRequestHandler):

    def handle(self):
        cnx = mysql.connector.connect(user='imu_user', password='1MckiD4', host='localhost', database='cyberphyssys')
        cnx.database = 'cyberphyssys'

        myTime = datetime.now()
        print(myTime.year, myTime.month, myTime.day, myTime.hour, myTime.minute, myTime.second, myTime.microsecond)
        print(myTime)

        # self.data = self.request.recv(1024).strip()
        self.data = self.rfile.readlines()

        data = []
        for i in range(len(self.data)):
            tmpROW = self.data[i].decode("utf-8")
            tmpROW = re.sub('[\r\n]', '', tmpROW)
            data.append(tmpROW)

        tblName = data[0] + "-" + data[1]
        TABLES = {}
        TABLES[tblName] = (
            "CREATE TABLE `{}` ("
            "   `datapt_id` BIGINT(100) NOT NULL AUTO_INCREMENT,"
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
            "   `date_time_pt` TIMESTAMP(2) NOT NULL,"
            "   PRIMARY KEY (`datapt_id`),"
            "   UNIQUE INDEX `datapt_id_UNIQUE` (`datapt_id` ASC) VISIBLE"
            ") ENGINE=InnoDB".format(tblName)
        )


        cursor = cnx.cursor()
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

        
        add_datapt = ("INSERT INTO `{}` "
                      "(device_id, sensor_id, gx, gy, gz, ax, ay, az, mx, my, mz, date_time_pt) "
                      "VALUES (%(devID)s, %(senID)s, %(gx)s, %(gy)s, %(gz)s, %(ax)s, %(ay)s, %(az)s, %(mx)s, %(my)s, %(mz)s, %(date_time_pt)s)".format(tblName))

        aDataPT = {
            'devID': data[0],
            'senID': data[1],
            'gx': float(data[2]),
            'gy': float(data[3]),
            'gz': float(data[4]),
            'ax': float(data[5]),
            'ay': float(data[6]),
            'az': float(data[7]),
            'mx': float(data[8]),
            'my': float(data[9]),
            'mz': float(data[10]),
            'date_time_pt': myTime,
        }

        cursor.execute(add_datapt, aDataPT)
        cnx.commit()

        print(data)

        cursor.close()
        cnx.close()


if __name__ == "__main__":
    HOST, PORT = "192.168.1.40", 5000

    print("started server!")

    server = socketserver.TCPServer((HOST, PORT), MyIMUdataHandler)

    server.serve_forever()