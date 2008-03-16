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
#include "NetwAddr.h"

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
      //scheduleAt(simTime() + dblrand()*10, delayTimer);
      scheduleAt(simTime() + dblrand()*beaconDelay, beaconTimer); // timer to send beacon
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
      /*
      GPSRPkt *pkt = new GPSRPkt("TEST_MESSAGE_XY",TEST_MESSAGE);
    // we use the host modules index() as a appl address
      pkt->setSrcAddr( myApplAddr() );
      pkt->setLength(headerLength);

      pkt->setControlInfo( new NetwControlInfo(L3BROADCAST) );
      sendDown( pkt );
      sendToXY(msg,200,100);	// send a message to 200,100 for test
      scheduleAt(simTime() + dblrand()*beaconDelay, beaconTimer); // timer to send beacon
      EV << "send a test message to 200,100" << endl;
      */
    }break;
    default:
        EV <<" Unkown selfmessage! -> delete, kind: "<<msg->kind()<<endl;
    }
}

void BlueqApplLayer::handleLowerMsg(cMessage *msg)
{
  GPSRPkt *pkt;
  pkt = static_cast<GPSRPkt *>(msg);
  switch(msg->kind()){

  case BROADCAST_MESSAGE:{
    EV <<"got a broadcast packet from host["<<pkt->getSrcAddr()<<"], sending replay"<<endl;
    sendReply(pkt);
  }break;

  case BROADCAST_REPLY_MESSAGE:{
    EV <<"got a broadcast replay packet from host["<<pkt->getSrcAddr()<<"]"<<endl;
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
    GPSRPkt *pkt = new GPSRPkt("BROADCAST_MESSAGE", BROADCAST_MESSAGE);
    // we use the host modules index() as a appl address
    pkt->setSrcAddr( myApplAddr() );
    pkt->setLength(headerLength);


    // set the control info to tell the network layer the layer 3
    // address;
    pkt->setControlInfo( new NetwControlInfo(L3BROADCAST) );
    sendDown( pkt );
}

// send message to node that who located x,y
void BlueqApplLayer::sendToXY(cMessage *msg, int x, int y)
{
  int destNetwAddr = NETW_ADDR(x,y);
  msg->setControlInfo(new NetwControlInfo(destNetwAddr));
  sendDown(msg);
}

void BlueqApplLayer::sendReply(GPSRPkt *msg) 
{
    double delay;

    delay = uniform(0, 0.01);
}

void BlueqApplLayer::finish() 
{
    BasicApplLayer::finish();
    if(!delayTimer->isScheduled()) delete delayTimer;
}

