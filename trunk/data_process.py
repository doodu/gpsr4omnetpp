#!/usr/bin/python

str = raw_input()

redirected = 0
delay = 0
sended = 0
recieved = 0

while str:
    strs = str.split('\t')
    id = int(strs[0])
    time = float(strs[1])
    data = float(strs[2])

    if data == 1:
        redirected = redirected + 1
    elif data == 2:
        sended = sended + 1
    elif data == 3:
        recieved = recieved + 1
    else:
        if delay == 0:
            delay = data
        else:
            delay = delay + data
            delay = delay / 2

    try:
        str = raw_input()
    except EOFError:
        break

succed = float(recieved) / float(sended)

print redirected,delay,succed
