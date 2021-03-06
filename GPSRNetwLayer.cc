/***************************************************************************
 * file:        GPSRNetwLayer.cc
nn *
* author:      Daniel Willkomm
*
* copyright:   (C) 2004 Telecommunication Networks Group (TKN) at
*              Technische Universitaet Berlin, Germany.
*
*              This program is free software; you can redistribute it 
*              and/or modify it under the terms of the GNU General Public 
*              License as published by the Free Software Foundation; either
*              version 2 of the License, or (at your option) any later 
*              version.
*              For further information see file COPYING 
*              in the top level directory
***************************************************************************
* part of:     framework implementation developed by tkn
* description: network layer: general class for the network layer
*              subclass to create your own network layer
***************************************************************************/


#include "GPSRNetwLayer.h"
#include "NetwControlInfo.h"
#include "MacControlInfo.h"

#include "GPSRPkt_m.h"
#include "NetwAddr.h"
#include "NetworkFile.h" 	// for storing the network into a file
#include "Packet.h"

#include <list>
#include <math.h>

#define DEBUG

using namespace std;

Define_Module(GPSRNetwLayer);

void GPSRNetwLayer::initialize(int stage)
{
  BasicLayer::initialize(stage);
  if(stage == 0){
    x = par("x");
    y = par("y");
    //    myNetwAddr = ADDR(x,y);
    indexLength = par("indexLength");
    deadProbablity = par("deadProbablity");
    if(hasPar("beaconDelay"))
      beaconDelay = par("beaconDelay");
    else	
      beaconDelay = 10;
        
    if(hasPar("maxWatchDog"))
      maxWatchDog = par("maxWatchDog");
    else
      maxWatchDog = 10;
    headerLength= par("headerLength");
    //    printf("header length:%d\n", headerLength);
    stable = false;
    dead = false;
    beaconTimer = new cMessage("beacon-timer", NET_TIMER_PACKET);
    deadTimer = new cMessage("dead-timer", NET_DEAD_TIMER_PACKET);
  }
  else if(stage==1){
    arp = SimpleArpAccess().get();
    myNetwAddr = this->id();
    EV << " myNetwAddr " << myNetwAddr << endl;
    
    scheduleAt(simTime() + dblrand()*beaconDelay, beaconTimer); // timer to send beacon
    scheduleAt(simTime() + 100,deadTimer);
  }
}

void GPSRNetwLayer::handleSelfMsg(cMessage *msg)
{

  GPSRPkt *pkt = static_cast <GPSRPkt *> (msg);

  if(dead){
    delete msg;
    return;
  }
  switch(msg->kind()){
  case NET_TIMER_PACKET:
    sendBeacon();
    if(count < 10){						  // when 10 times later we stop echo
      scheduleAt(simTime() + dblrand() * beaconDelay, beaconTimer); // timer to send beacon
      count ++;
    }else{
      stable = true;
      // save route table
      // saveOneNode(myNetwAddr,routeTable);
    }
    //    EV << " receive a beacon from addr "<<pkt->getSrcAddr()<<endl;
    break;
  case NET_DEAD_TIMER_PACKET:{
    double d = dblrand();
    if(d < deadProbablity) {
      dead = true;
    }
    scheduleAt(simTime() + 100, deadTimer);
  }break;
  default:
    EV <<" Unkown selfmessage! -> delete, kind: "<<msg->kind()<<endl;
  }
}
/**
 * Decapsulates the packet from the received Network packet 
 **/
cMessage* GPSRNetwLayer::decapsMsg(GPSRPkt *msg) 
{
  cMessage *m = msg->decapsulate();
  m->setControlInfo(new NetwControlInfo(msg->getSrcAddr()));
  // delete the netw packet
  delete msg;
  return m;
}

void GPSRNetwLayer::sendBeacon()
{
  cMessage *msg = new cMessage("no use message",0);
  GPSRPkt *pkt = new GPSRPkt("NET_BEACON_MESSAGE",NET_BEACON_PACKET);

  pkt->setLength(headerLength);
  EV << "headerLength:"<<headerLength<<endl;

  pkt->setSrcAddr(myNetwAddr);
  pkt->setDestAddr(-1);
  pkt->setSrcLoc(LOC(x,y));
  pkt->setControlInfo( new MacControlInfo(L2BROADCAST));
  pkt->encapsulate(msg);
  sendDown(pkt);

}
/**
 * Encapsulates the received ApplPkt into a GPSRPkt and set all needed
 * header fields.
 **/
GPSRPkt* GPSRNetwLayer::encapsMsg(cMessage *msg) {    
  int macAddr;
  int netwAddr;

  EV <<"in encaps...\n";

  //GPSRPkt *pkt = new GPSRPkt(msg->name(),msg->kind());
  GPSRPkt *pkt;
  if(msg->kind() == APPL_CREATE_LINK_PACKET){
    pkt = new GPSRPkt( "NET_CREATE_LINK_PACKET" , NET_CREATE_LINK_PACKET);
  }else{
    pkt = new GPSRPkt( "NET_DATA_PACKET" , NET_DATA_PACKET);
  }

  pkt->setLength(headerLength);
  NetwControlInfo* cInfo = dynamic_cast<NetwControlInfo*>(msg->removeControlInfo());

  if(cInfo == 0){
    EV << "warning: Application layer did not specifiy a destination L3 address\n"
       << "\tusing broadcast address instead\n";
    netwAddr = L3BROADCAST;
  } else {
    EV <<"CInfo removed, netw addr="<< cInfo->getNetwAddr()<<endl;
    netwAddr = cInfo->getNetwAddr();
    delete cInfo;
  }

  // check if the packet is to self, if it is ,send to myself
  pkt->setMode(GREEDY_MODE);    
  pkt->setSrcAddr(myNetwAddr);
  pkt->setSrcLoc(LOC(x,y));
  pkt->setDestAddr(0);
  pkt->setDestLoc(netwAddr);

  EV << " netw "<< myNetwAddr << " sending packet" <<endl;
  if(netwAddr == L3BROADCAST) {
    EV << "sendDown: nHop=L3BROADCAST -> message has to be broadcasted"
       << " -> set destMac=L2BROADCAST\n";
    macAddr = L2BROADCAST;
  }else if(netwAddr == LOC(x,y)){ // if the packet is self to self, send it up driectly
    sendUp(msg);
    return NULL;
  }else{
    EV <<"sendDown: get the MAC address\n";
    // 在这里路由
    int nextHopAddr; 
    int destx,desty;
    destx = GETX(netwAddr);
    desty = GETY(netwAddr);
    
    // if the msg is for myself,then delete it
    if(x == destx && y == desty){
      delete msg;
      delete pkt;
      return NULL;
    }
    nextHopAddr = greedyForwarding(destx,desty);
    //nextHopAddr = myNetwAddr;
    if(nextHopAddr == myNetwAddr){ // if can not use greedy forwarding
      nextHopAddr = enterPerimeterMode(pkt);
    }

    pkt->setDestAddr(nextHopAddr);

    printf("in %d(%d,%d):\n",myNetwAddr,x,y);
    printf("\tdest(%d,%d)\n",destx,desty);
    printf("\tnext(%d)\n",nextHopAddr);
    macAddr = arp->getMacAddr(nextHopAddr); // 发往下一跳
    // the mac layer has something strange, so send to every neighbor
    // when the neighbor receive the packet, check the DestAddr if is his,
    // if not then delete the packet, else routing it 
    // macAddr = L2BROADCAST;	
    printf("\tmac(%d)\n\n",macAddr);
  }

  pkt->setControlInfo(new MacControlInfo(macAddr));
    
  //encapsulate the application packet
  pkt->encapsulate(msg);
  EV <<" pkt encapsulated\n";
  return pkt;
}

/**
 * Redefine this function if you want to process messages from lower
 * layers before they forwarded to upper layers
 *
 *
 * If you want to forward the message to upper layers please use
 * @ref sendUp which will take care of decapsulation and thelike
 **/
void GPSRNetwLayer::handleLowerMsg(cMessage* msg)
{
  CreateLinkPkt *msgup,*p;
  GPSRPkt *m = static_cast<GPSRPkt *>(msg);
  p = (CreateLinkPkt *)(msg);
  int nextHopAddr;
  int pktCount;

  if(dead){
    delete msg;
    return;
  }
  EV << " handling packet from " << m->getSrcAddr() << endl;
  switch(msg->kind()){

  case NET_BEACON_PACKET:{
    updateRouteTable(m);	// while beacon arrived, update the route table
    delete msg;
  }break;
  case NET_CREATE_LINK_PACKET:
    pktCount = p->getCount();
    p->setCount(pktCount + 1);
    EV << "COUNT:" << pktCount << endl;
  case NET_DATA_PACKET:{
    // check the packet if it is for me, if not then delete it
    // do this because the mac module has something strange:(
    int destAddr = m->getDestAddr();
    if(destAddr != myNetwAddr){
      delete m;
      return;
    }
    // if is for me, get ready of it
    int destLoc = m->getDestLoc();
    if(destLoc == LOC(x,y)){	// if the packet is for me, then send it up
      sendUp(decapsMsg(m));
      return;
    }
    nextHopAddr = routeMsg(m);
    if(msg->kind() == NET_CREATE_LINK_PACKET){
      // send a APPL_GOT_CREATE_LINK_PACKET to the application layer
      // packet class is CreateLinkPkt, see CreateLinkPkt.msg
      int nextx,nexty;
      // get the next hop's x,y coordination
      std::list<Node>::iterator it;
      for(it = routeTable.begin(); it != routeTable.end(); it ++){
         if(it->addr == nextHopAddr){
  	         nextx = it->x;
		 nexty = it->y;
	      }
      }
      msgup = new CreateLinkPkt("APPL_GOT_CREATE_LINK_PACKET", APPL_GOT_CREATE_LINK_PACKET);
      msgup->setNextx(nextx);
      msgup->setNexty(nexty);
      msgup->setTime(simTime());
      if(pktCount % indexLength == 0){
	      msgup->setEntry(1);
      }else{
	      msgup->setEntry(0);
      }
      sendUp(msgup);
    }
  }break;
  default:
    sendUp(decapsMsg(m));	// if the message not regionised, send it up to Applcation Layer
  }
}

/**
 * Redefine this function if you want to process messages from upper
 * layers before they are send to lower layers.
 *
 * For the GPSRNetwLayer we just use the destAddr of the network
 * message as a nextHop
 *
 * To forward the message to lower layers after processing it please
 * use @ref sendDown. It will take care of anything needed
 **/
void GPSRNetwLayer::handleUpperMsg(cMessage* msg)
{


  // send down while stable
  if(dead){
    delete msg;
    return;
  }
  if(stable){

    GPSRPkt *enmsg = encapsMsg(msg);
    if(msg->kind() == APPL_CREATE_LINK_PACKET){ // the first node
      CreateLinkPkt *msgup;

      int destAddr = enmsg->getDestAddr();
      int nextx,nexty;
      for(std::list<Node>::iterator it = routeTable.begin(); it != routeTable.end(); it++){
	     if(destAddr == it->addr){
	       nextx = it->x;
	       nexty = it->y;
	     }
      }
      msgup = new CreateLinkPkt("APPL_GOT_CREATE_LINK_PACKET", APPL_GOT_CREATE_LINK_PACKET);
      msgup->setNextx(nextx);
      msgup->setNexty(nexty);
      msgup->setTime(simTime());
	   msgup->setEntry(1);
	   sendUp(msgup);
    }
    if(enmsg != NULL){	// the message is for myself
      sendDown(enmsg);
	}
  }else{
    delete msg;
  }
}


int GPSRNetwLayer::greedyForwarding(int destx,int desty)
{

  int nextHopAddr;
  list<Node>::iterator it,who;
  int meToDest;
  int minDistance;
  
  // 如果目标在路由表中，直接发送
  // 否则选择离目标最近的邻居作下一跳
  // 如果路由表中的节点都比本节点距止目标远，使用另一种算法

  meToDest = DISTANCE(x,y,destx,desty);
  if(meToDest == 0){
    return myNetwAddr;
  }
  minDistance = meToDest;
  for(it = routeTable.begin(); it != routeTable.end(); it ++){
    int tmpDistance;
    tmpDistance= DISTANCE(it->x,it->y,destx,desty);
    if(tmpDistance < minDistance){
      minDistance = tmpDistance;
      nextHopAddr = it->addr;
    }
    //printf("route:%d(%d,%d)\n",nextHopAddr,it->x,it->y);
    //x7fflush(stdout);
  }
  return nextHopAddr;
}

void GPSRNetwLayer::updateRouteTable(GPSRPkt *pkt)
{
    int x,y;
    int addr,loc;
    addr = pkt->getSrcAddr();
    loc = pkt->getSrcLoc();

    x = GETX(loc);
    y = GETY(loc);		// get the source's position

    bool find = false;

    list<Node>::iterator it;
    for(it = routeTable.begin(); it != routeTable.end(); it++){
      if(it->addr == addr){	// 如果路由表中已经有这项则更新它
	      it->x = x;
	      it->y = y;
	      it->watchDog = maxWatchDog;
	      find = true;
	      break;
      }
    }

    if(!find){		// 如果没有则加入这一项
      Node node;
      node.addr = addr;
      node.x = x;
      node.y = y;
      node.watchDog = maxWatchDog;
      routeTable.push_back(node);

      // insert the node to planarizedTable
      planarizedTable.push_back(node);
      planarizedGraph();
    }
// the follow lines print the debug information of the route table
#ifdef DEBUG
    EV << "got a beacon message from address "<<pkt->getSrcAddr()<<endl;
    for(it = routeTable.begin(); it != routeTable.end(); it++){
      EV <<"host["<<it->addr<<"] ("<<it->x<<","<<it->y<<")"<<endl;
    }
#endif
}

// route the msg to the next hop
// return the next hop's address
int GPSRNetwLayer::routeMsg(GPSRPkt *pkt)
{
  int destAddr = pkt->getDestAddr();
  int destLoc = pkt->getDestLoc();
  int destx = GETX(destLoc);
  int desty = GETY(destLoc);	// get the dest address and location of the packet

  int mode = pkt->getMode();
  int macAddr;

  if(mode == GREEDY_MODE){
    printf("CREEDY MODE:");
    if(destLoc == LOC(x,y) ||						\
       destAddr == L3BROADCAST){ // test if i was the destiation
      sendUp(decapsMsg(pkt));
    }else{
      int nHopAddr = greedyForwarding(destx,desty);
      if(nHopAddr != myNetwAddr){ // can greedy forwarding
        sendtoNextHop(pkt,nHopAddr);
        return nHopAddr;
      } else {
	     //can not greedy forwarding, entering perimeter forwarding
	     // while entering perimeter mode, we need to set something 
	     // see the paper:)
	     int nHopAddr = enterPerimeterMode(pkt);
	     sendtoNextHop(pkt,nHopAddr);
	     return nHopAddr;
      }
    }
  } else if (mode == PERIMETER_MODE){
    printf("PERIMETER MODE:");
    // 边界模式下
    int nHopAddr = perimeterForwarding(destx,desty);
    int nx,ny;
    // the following get the location of the next hop
    for(list<Node>::iterator it = routeTable.begin(); it != routeTable.end(); it++){
      if(it->addr == nHopAddr){
   	nx = it->x;
   	ny = it->y;
      }
    }
    int lpx = GETX(pkt->getLpAddr());
    int lpy = GETY(pkt->getLpAddr());

    if(DISTANCE(nx,ny,destx,desty) <= DISTANCE(lpx,lpy,destx,desty)){
      // 恢复greedy模式
      pkt->setMode(GREEDY_MODE);
    }
    
    sendtoNextHop(pkt,nHopAddr);
    return nHopAddr;
  }else{
    printf("wrong mode");
  }
  return 0;
}


void GPSRNetwLayer::sendtoNextHop(GPSRPkt *pkt, int nextHopAddr)
{
  int macAddr = arp->getMacAddr(nextHopAddr);
  qtime.record(1);		// record one times for packet routeing
  printf("in %d send to %d(mac %d)\n",myNetwAddr,nextHopAddr,macAddr);
  MacControlInfo* cInfo = dynamic_cast<MacControlInfo*>(pkt->removeControlInfo());
  pkt->setControlInfo(new MacControlInfo(macAddr));
  pkt->setDestAddr(nextHopAddr);//this is important, is dest address not reset, the next
  //router will drop the packet, that is why packet only route twice at first
  sendDown(pkt);
}

int GPSRNetwLayer::enterPerimeterMode(GPSRPkt *pkt)
{
  pkt->setLpAddr(myNetwAddr);
  pkt->setLfAddr(myNetwAddr);
  pkt->setMode(PERIMETER_MODE);
  int destx = GETX(pkt->getDestAddr());
  int desty = GETY(pkt->getDestAddr());
  int nextHop = perimeterForwarding(destx,desty);
  pkt->setE0vectex1(myNetwAddr);
  pkt->setE0vectex2(nextHop);
  return nextHop;
}

// 遍历平面图中与当前节点相邻的每一个节点，找到最右边的（右手法则）
int GPSRNetwLayer::perimeterForwarding(int destx,int desty)
{
  list<Node>::iterator it;
  int nextHop = -1000000;
  int maxMul = -1000000;

  int sdx = destx - x;
  int sdy = desty - y;		// 计算自已到目的的向量

  int snx,sny;			// 邻节点到自己的向量
  for(it = planarizedTable.begin(); it != planarizedTable.end(); it ++){
    snx = it->x - x;
    sny = it->y - y;
    // 计算向量的叉积,取叉积最大的
    // 要跟向量的点积区分开来
    int tmp = snx * sdy - sny * sdx;
    if(tmp > maxMul){
      maxMul = tmp;
      nextHop = it->addr;
    }
  }
  return nextHop;
}


#define NODE_DISTANCE(sta,end) DISTANCE((sta)->x,(sta)->y,(end)->x,(end)->y)

void GPSRNetwLayer::planarizedGraph()
{
  list<Node>::iterator v;
  list<Node>::iterator w;
  list<Node>::iterator i;

  Node self;			// the node of myself
  self.x = x;
  self.y = y;

  // make the graph RNG
  for(v = planarizedTable.begin(); v != planarizedTable.end(); ){
    list<Node>::iterator tmp = v;
    tmp ++;
    for(w = planarizedTable.begin(); w != planarizedTable.end(); w++){

      if( v == w ) continue;
      else{
	      if(NODE_DISTANCE(&self,v) >				\
		 max( NODE_DISTANCE(w,&self), NODE_DISTANCE(w,v)) ){
		//	   delete the self,v edge
		planarizedTable.erase(v);
		break;
	      }
      }
      
    }
    v = tmp;
  }

  // make the graph GG

}

