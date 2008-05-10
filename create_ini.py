#!/usr/bin/python

import os
import sys
import random


if len(sys.argv) != 5:
    print "Usage:" + sys.argv[0] + " [rows] [cloums] [index_length] [dead_probablity]"
    sys.exit(-1)

rows = int(sys.argv[1])
cls = int(sys.argv[2])
index_length = sys.argv[3]
dead_probablity = sys.argv[4]

width = cls*100        # the width of the canvas
height = rows*100       # the height of the canvas
hosts = rows * cls        # how many host 

# copy the template file
os.system('cp -f omnetpp.ini.temple omnetpp.ini')

ini_file = open("omnetpp.ini","a")

lines = ["sim.host[*].net.indexLength=" + index_length + "\n",
         "sim.host[*].net.deadProbablity=" + dead_probablity + "\n"]

ini_file.writelines(lines)

lines = ["sim.playgroundSizeX=" + str(width) + "\n",
         "sim.playgroundSizeY=" + str(height) + "\n",
         "sim.numHosts=" + str(hosts * 2 + 4) + "\n"]

# the additional 4 host are application parameters

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

#paramter nodes for application layer
center = [width/2, height/2]
start = [0, height]
end = [width, 0]
query = [0, 0]

lines = ["sim.host[" + str(k) + "].x=" + str(center[0]) + "\n",
         "sim.host[" + str(k) + "].y=" + str(center[1]) + "\n",
         "sim.host[*].appl.centerx=" + str(center[0]) + "\n",
         "sim.host[*].appl.centery=" + str(center[1]) + "\n",]
ini_file.writelines(lines)
k = k + 1
lines = ["sim.host[" + str(k) + "].x=" + str(start[0]) + "\n",
         "sim.host[" + str(k) + "].y=" + str(start[1]) + "\n",
         "sim.host[*].appl.startx=" + str(start[0]) + "\n",
         "sim.host[*].appl.starty=" + str(start[1]) + "\n"]
ini_file.writelines(lines)
k = k + 1
lines = ["sim.host[" + str(k) + "].x=" + str(end[0]) + "\n",
         "sim.host[" + str(k) + "].y=" + str(end[1]) + "\n",
         "sim.host[*].appl.endx=" + str(end[0]) + "\n",
         "sim.host[*].appl.endy=" + str(end[1]) + "\n"]
ini_file.writelines(lines)
k = k + 1
lines = ["sim.host[" + str(k) + "].x=" + str(query[0]) + "\n",
         "sim.host[" + str(k) + "].y=" + str(query[1]) + "\n",
         "sim.host[*].appl.queryx=" + str(query[0]) + "\n",
         "sim.host[*].appl.queryy=" + str(query[1]) + "\n"]
ini_file.writelines(lines)
ini_file.close()

#os.system("./802.11")
