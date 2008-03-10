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

#include <list>
#include <math.h>

#define PING_FANG(x) (x)*(x)
#define DISTANCE(x1,y1,x2,y2) PING_FANG((x1) - (x2)) + PING_FANG((y1) - (y2))

#define ADDR(x,y) ((x) << 16 | ((y)&0x0000ffff))
#define GETX(addr) ((addr)>>16)
#define GETY(addr) ((addr) & 0x0000ffff)

using namespace std;

Define_Module(GPSRNetwLayer);

void GPSRNetwLayer::initialize(int stage)
{
  BasicLayer::initialize(stage);
  if(stage == 0){
    x = par("x");
    y = par("y");
    if(hasPar("beaconDelay"))
      beaconDelay = par("beaconDelay");
    else	
      beaconDelay = 10;
    
    
    if(hasPar("maxWatchDog"))
      maxWatchDog = par("maxWatchDog");
    else
      maxWatchDog = 10;
    beaconTimer = new cMessage("beacon-timer", SEND_BEACON_TIMER);
  }
  else if(stage==1){
    headerLength= par("headerLength");
    arp = SimpleArpAccess().get();
    myNetwAddr = ADDR(x,y);
    EV << " myNetwAddr " << myNetwAddr << endl;
    
    scheduleAt(simTime() + dblrand()*beaconDelay, beaconTimer); // timer to send beacon
  }
}

void GPSRNetwLayer::handleSelfMsg(cMessage *msg)
{

  GPSRPkt *pkt = static_cast <GPSRPkt *> (msg);
  switch(msg->kind()){
  case SEND_BEACON_TIMER:
    sendBeacon();
    scheduleAt(simTime() + dblrand() * beaconDelay, beaconTimer); // timer to send beacon
    //    EV << " receive a beacon from addr "<<pkt->getSrcAddr()<<endl;
    break;
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
  GPSRPkt *pkt = new GPSRPkt("BEACON_MESSAGE",BEACON_MESSAGE);

  pkt->setLength(headerLength);
  EV << "headerLength:"<<headerLength<<endl;

  pkt->setSrcAddr(myNetwAddr);
  pkt->setDestAddr(-1);
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

  GPSRPkt *pkt = new GPSRPkt(msg->name(),msg->kind());
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
    
  pkt->setSrcAddr(myNetwAddr);
  pkt->setDestAddr(netwAddr);
  EV << " netw "<< myNetwAddr << " sending packet" <<endl;
  if(netwAddr == L3BROADCAST) {
    EV << "sendDown: nHop=L3BROADCAST -> message has to be broadcasted"
       << " -> set destMac=L2BROADCAST\n";
    macAddr = L2BROADCAST;
  }
  else{
    EV <<"sendDown: get the MAC address\n";

    // 在这里路由
    int nextHopAddr; 
    int destx,desty;
    destx = GETX(netwAddr);
    desty = GETY(netwAddr);

    nextHopAddr = greedyForwarding(destx,desty);
    if(nextHopAddr != myNetwAddr){
      macAddr = arp->getMacAddr(nextHopAddr); // 发往下一跳
    }else{				      // 使用另一种路由
      EV <<" recieve a packet that can not be forward by greedy forwarding"<<endl;
      macAddr = L2BROADCAST;	// 广播出去
    }
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
  GPSRPkt *m = static_cast<GPSRPkt *>(msg);

  EV << " handling packet from " << m->getSrcAddr() << endl;
  switch(msg->kind()){

  case BEACON_MESSAGE:{
    int x,y;
    int addr;
    addr = m->getSrcAddr();

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
    }

    EV << "got a beacon message from address "<<m->getSrcAddr()<<endl;
    for(it = routeTable.begin(); it != routeTable.end(); it++){
      EV <<"host["<<it->addr<<"] ("<<it->x<<","<<it->y<<")"<<endl;
    }
    delete msg;
  }break;

 default:
   sendUp(decapsMsg(m));
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
  sendDown(encapsMsg(msg));
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
  minDistance = meToDest;
  for(it = routeTable.begin(); it != routeTable.end(); it ++){
    int tmpDistance;
    tmpDistance= DISTANCE(it->x,it->y,destx,desty);
    if(tmpDistance < minDistance){
      minDistance = tmpDistance;
      nextHopAddr = it->addr;
    }
  }
  return nextHopAddr;
}
