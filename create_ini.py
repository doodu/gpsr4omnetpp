#!/usr/bin/python

import os
import sys
import random


if len(sys.argv) != 3:
    print "Usage:" + sys.argv[0] + " [rows] [cloums]"
    sys.exit(-1)

rows = int(sys.argv[1])
cls = int(sys.argv[2])

width = cls*100        # the width of the canvas
height = rows*100       # the height of the canvas
hosts = rows * cls        # how many host 

# copy the template file
os.system('cp -f omnetpp.ini.temple omnetpp.ini')

ini_file = open("omnetpp.ini","a")

lines = ["sim.playgroundSizeX=" + str(width) + "\n",
         "sim.playgroundSizeY=" + str(height) + "\n",
         "sim.numHosts=" + str(hosts * 2) + "\n"]

ini_file.writelines(lines)

k = 0
for i in range(rows):
    for j in range(cls):
        x = j * 100 + random.randint(0,100)
        y = i * 100 + random.randint(0,100) #to make the point every where
        lines = ["sim.host[" + str(k) + "].x=" + str(x) + "\n", 
                 "sim.host[" + str(k) + "].y=" + str(y) + "\n"]
        ini_file.writelines(lines)
        k = k + 1
        # there is two node in one region
        x = j * 100 + random.randint(0,100)
        y = i * 100 + random.randint(0,100) #to make the point every where
        lines = ["sim.host[" + str(k) + "].x=" + str(x) + "\n", 
                 "sim.host[" + str(k) + "].y=" + str(y) + "\n"]
        ini_file.writelines(lines)
        k = k + 1


ini_file.close()

os.system("./802.11")
