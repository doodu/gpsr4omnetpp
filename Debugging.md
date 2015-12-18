# Introduction #

Debugging  [r35](https://code.google.com/p/gpsr4omnetpp/source/detail?r=35)


# Details #

In graphic mode, we can see airframes are dropped by snrEavl in the nic. Part of these frames is transmitted with insufficient power.

In 802.11, a link does not guarantee receiving.


airframes passed to decider by the snrEavl could be dropped too. It is because in this stage, their length are check to see if it is too short according to  the standard of 802.11.

See? You cannot send down zero length message in appl layer(i wonder if bit stuffing is provided) I suggest you use standard App Packet and set its parameters carefully.


You also got problems in the net layer. You do not treat a LINK message. DATA messages to other nodes are deleted which should be relayed otherwise.

you are approaching success. good luck.