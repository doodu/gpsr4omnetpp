#ifndef NETWORK_FILE_H
#define NETWORK_FILE_H
#include <list>


struct Node
{
  int addr;			/* the address of the node  */
  int x,y;			        /* the location of the node */
  int watchDog;			/* to count the node's state, if watchDog<=0 the
				           node is timeout*/
};

struct Host
{
  int addr;
  std::list<Node> routeTable;
};
void saveOneNode(int addr, std::list<Node> &routeTable);
void loadOneNode(int addr, std::list<Node> &routeTable);
#endif
