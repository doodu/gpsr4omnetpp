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
    myNetwAddr = this->id();
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

  pkt->setSrcAddr(myNetwAddr);
  pkt->setDestAddr(-1);
  pkt->setSavedx(x);
  pkt->setSavedy(y);
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

  GPSRPkt *pkt = new GPSRPkt(msg->name(), msg->kind());
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

    // ������·��
    int nextHopAddr; 
    int destx,desty;
    destx = pkt->getDstx();
    desty = pkt->getDsty();

    nextHopAddr = greedyForwarding(destx,desty);
    if(nextHopAddr != myNetwAddr){
      macAddr = arp->getMacAddr(nextHopAddr); // ������һ��
    }else{				      // ʹ����һ��·��
      EV <<" recieve a packet that can not be forward by greedy forwarding"<<endl;
      macAddr = L2BROADCAST;	// �㲥��ȥ
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
    x = m->getSavedx();
    y = m->getSavedy();	// �������

    bool find = false;

    list<Node>::iterator it;
    for(it = routeTable.begin(); it != routeTable.end(); it++){
      if(it->addr == addr){	// ���·�ɱ����Ѿ��������������
	      it->x = x;
	      it->y = y;
	      it->watchDog = maxWatchDog;
	      find = true;
	      break;
      }
    }

    if(!find){		// ���û���������һ��
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
  
  // ���Ŀ����·�ɱ��У�ֱ�ӷ���
  // ����ѡ����Ŀ��������ھ�����һ��
  // ���·�ɱ��еĽڵ㶼�ȱ��ڵ��ֹĿ��Զ��ʹ����һ���㷨
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
