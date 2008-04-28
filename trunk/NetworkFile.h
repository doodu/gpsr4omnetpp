#ifndef NETWORK_FILE_H
#define NETWORK_FILE_H
#include <list>


struct Node;
struct Host;
void saveOneNode(int addr, std::list<Node> &routeTable);
void loadOneNode(int addr, std::list<Node> &routeTable);
#endif
