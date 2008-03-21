#ifndef NETWADDR_H
#define NETWADDR_H

#define DISTANCE(x1,y1,x2,y2) (((x1)-(x2))*((x1)-(x2))\
			       + ((y1)-(y2))*((y1)-(y2)))
#define NETW_ADDR(x,y) ((x)<<16|((y)&0xffff))
#define ADDR NETW_ADDR
#define GETX(addr) (addr>>16)
#define GETY(addr) (addr&0xFFFF)
#endif
