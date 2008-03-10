/* -*- mode:c++ -*- ********************************************************
 * file:        BurstApplLayer.cc
 *
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
 * description: application layer like the TestApplLayer but sends burst of 
 *               messages
 **************************************************************************/
#include "BlueqApplLayer.h"
#include "NetwControlInfo.h"

#include <SimpleAddress.h>
#include <list>

using namespace std;

Define_Module(BlueqApplLayer);


/**
 * First we have to initialize the module from which we derived ours,
 * in this case BasicModule.
 *
 * Then we will set a timer to indicate the first time we will send a
 * message
 *
 **/
void BlueqApplLayer::initialize(int stage)
{
    BasicApplLayer::initialize(stage);
    if(stage == 0) {
        if(hasPar("burstSize"))
            burstSize = par("burstSize");
        else
            burstSize = 1;
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
        delayTimer = new cMessage( "delay-timer", SEND_BROADCAST_TIMER );
	beaconTimer = new cMessage("beacon-timer", SEND_BEACON_TIMER);
    }
    else if(stage==1) {
      scheduleAt(simTime() + dblrand()*10, delayTimer);
      //scheduleAt(simTime() + dblrand()*beaconDelay, beaconTimer); // timer to send beacon
    }
}


void BlueqApplLayer::handleSelfMsg(cMessage *msg)
{
  switch(msg->kind())
    {
    case SEND_BROADCAST_TIMER:
      for(int i=0; i<burstSize; i++) {
	sendBroadcast();
	EV <<"send broadcast "<<i+1<<endl;
      }
      break;
    case SEND_BEACON_TIMER:{
      sendBeacon();
      scheduleAt(simTime() + dblrand() * beaconDelay, beaconTimer); // timer to send beacon
      
      list<Node>::iterator it;
      for(it = routeTable.begin(); it != routeTable.end(); it++){
	it->watchDog --;
      }
      it = routeTable.begin();

      while(it != routeTable.end()){
	list<Node>::iterator itTemp = it;
	itTemp ++;
	if(it->watchDog <= 0){
	  routeTable.erase(it);
	}
	it = itTemp;
      }
      EV<<"send beacon"<<endl;
    }break;
    default:
        EV <<" Unkown selfmessage! -> delete, kind: "<<msg->kind()<<endl;
    }
}

void BlueqApplLayer::handleLowerMsg(cMessage *msg)
{
  GpsrPkt *pkt;
  pkt = static_cast<GpsrPkt *>(msg);
  switch(msg->kind()){

  case BROADCAST_MESSAGE:{
    EV <<"got a broadcast packet from host["<<pkt->getSrcAddr()<<"], sending replay"<<endl;
    sendReply(pkt);
  }break;

  case BROADCAST_REPLY_MESSAGE:{
    EV <<"got a broadcast replay packet from host["<<pkt->getSrcAddr()<<"]"<<endl;
  }break;

  case BEACON_MESSAGE:{
    int addr = pkt->getSrcAddr();
    int x = pkt->getX();
    int y = pkt->getY();

    bool find = false;

    list<Node>::iterator it;
    for(it = routeTable.begin(); it != routeTable.end(); it++){
      if(it->addr == addr){
	it->x = x;
	it->y = y;
	it->watchDog = maxWatchDog;
	find = true;
	break;
      }
    }
    if(!find){
      Node node;
      node.addr = addr;
      node.x = x;
      node.y = y;
      node.watchDog = maxWatchDog;
      routeTable.push_back(node);
    }
    EV <<"host["<<pkt->getSrcAddr()<<"] beacon ("<<x<<","<<y<<")"<<endl;
    for(it = routeTable.begin(); it != routeTable.end(); it++){
      EV <<"host["<<it->addr<<"] ("<<it->x<<","<<it->y<<")"<<endl;
    }
  }break;

  default:
    EV << "Error! got a packet with unkown kind "<<msg->kind()<<endl;
  }
}

/**
 * This function creates a new broadcast message and sends it down to
 * the network layer
 **/
void BlueqApplLayer::sendBroadcast()
{
    GpsrPkt *pkt = new GpsrPkt("BROADCAST_MESSAGE", BROADCAST_MESSAGE);
    pkt->setNextHopAddr(-1);	// ��һ���ĵ�ַ�����е�neighbors
    // we use the host modules index() as a appl address
    pkt->setSrcAddr( myApplAddr() );
    pkt->setLength(headerLength);


    // set the control info to tell the network layer the layer 3
    // address;
    pkt->setControlInfo( new NetwControlInfo(L3BROADCAST) );
    sendDown( pkt );
}
void BlueqApplLayer::sendBeacon()
{
  GpsrPkt *pkt = new GpsrPkt("BEACON_MESSAGE",BEACON_MESSAGE);

  pkt->setNextHopAddr(-1);	// �㲥

  pkt->setSrcAddr( myApplAddr());
  pkt->setLength(headerLength);
    
  // set the location of my self
  pkt->setX(x);
  pkt->setY(y);

  pkt->setControlInfo( new NetwControlInfo(L3BROADCAST));
  sendDown(pkt);

}

void BlueqApplLayer::sendReply(GpsrPkt *msg) 
{
    double delay;

    delay = uniform(0, 0.01);

    msg->setNextHopAddr(msg->getSrcAddr());
    msg->setSrcAddr(myApplAddr());
    msg->setKind(BROADCAST_REPLY_MESSAGE);
    msg->setName("BROADCAST_REPLY_MESSAGE");
    sendDelayedDown(msg, delay);
    //NOTE: the NetwControl info was already ste by the network layer
    //and stays the same
}

void BlueqApplLayer::finish() 
{
    BasicApplLayer::finish();
    if(!delayTimer->isScheduled()) delete delayTimer;
}