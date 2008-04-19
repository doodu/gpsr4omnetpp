#include "NetworkFile.h"
#include <list>

static char *fileName = "network.net";
static std::list<Host> hostTable;
static int isLoaded = 0;


static void loadAllNodes()
{
  char line[250];
  FILE *fd = fopen(fileName,"rb");
  if(fd == NULL){
    printf("can not open file %s for read\n",fileName);
    exit(-1);
  }
  int ret = fscanf(fd, "%s", line);
  Host *host = NULL;
  while(ret != EOF){
    if(strcmp(line,"%%") == 0){	// entering a new section
      if(host != NULL){
	      hostTable.push_back(*host);
	      delete host;
      }
      host = new Host;
      int addr;
      fscanf(fd, "%d", &addr);
      host->addr = addr;
    }else{
      Node node;
      int addr,x,y;
      sscanf(line,"%d:%d:%d",&addr,&x,&y);
      node.addr = addr;
      node.x = x;
      node.y = y;
      host->routeTable.push_back(node);
    }
    ret = fscanf(fd,"%s",line);
    if(ret == EOF){
      hostTable.push_back(*host);
      break;
    }
  }
  fclose(fd);
}

static void saveAllNodes()
{
  FILE *fd = fopen(fileName,"wb");
  if(fd == NULL){
    printf("can not open file %s\n",fileName);
    exit(-1);
  }
  std::list<Host>::iterator it;
  for(it = hostTable.begin(); it != hostTable.end(); it++){
    fprintf(fd,"%%\n");		// start with %%\n
    fprintf(fd,"%d\n",it->addr); // address
    std::list<Node>::iterator nodeIt;
    for(nodeIt = it->routeTable.begin(); nodeIt != it->routeTable.end(); nodeIt ++){
      fprintf(fd,"%d:%d:%d\n",nodeIt->addr,nodeIt->x,nodeIt->y);
    }

  }
  fclose(fd);
}
void saveOneNode(int addr, std::list<Node> &routeTable)
{
  if(!isLoaded){
    loadAllNodes();
    isLoaded = 1;
  }
  std::list<Host>::iterator it;
  for (it = hostTable.begin(); it != hostTable.end(); it++){
    if(it->addr == addr){	// if find delete the item
      hostTable.erase(it);
      break;
    }
  }
  Host host;
  host.addr = addr;
  host.routeTable = routeTable;
  hostTable.push_back(host);
  saveAllNodes();
}

void loadOneNode(int addr, std::list<Node> &routeTable)
{
  if(! isLoaded){
    loadAllNodes();
    isLoaded = 1;
  }
  std::list<Host>::iterator it;
  for(it = hostTable.begin(); it != hostTable.end(); it++){
    if(it->addr == addr){
      routeTable = it->routeTable; // copy it out
      break;
    }
  }
}
