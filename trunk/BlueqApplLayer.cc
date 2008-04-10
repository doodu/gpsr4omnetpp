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
      testTimer = new cMessage("test-timer",TEST_TIMER_MSG);

      // get paramters
      selfx = par("x");
      selfy = par("y");

      centerx = par("centerx");
      centery = par("centery");

      query_nodex = par("queryx");
      query_nodey = par("queryy");

      link_startx = par("startx");
      link_starty = par("starty");

      link_endx = par("endx");
      link_endy = par("endy");
    }
    else if(stage==1) {
      double timeDelay = simTime() + dblrand() * 100;
      scheduleAt(timeDelay, testTimer); // timer to send beacon
    }
}


void BlueqApplLayer::handleSelfMsg(cMessage *msg)
{
  double timeDelay = simTime() + 1000 + dblrand() * 1000;

  switch(msg->kind())
    {
    case TEST_TIMER_MSG:{
      // use to create the query links
      if(selfx == link_startx && selfy == link_starty){
   	cMessage *createLinkMsg = new cMessage("create-link-message",CREATE_LINK_MSG);
   	sendToXY(createLinkMsg,link_endx,link_endy);
   	EV<<"("<< selfx<<","<<selfy<<") send a create link message to ("
    	  <<link_endx<<","<<link_endy<<")"<<endl;
      }
      /*
      cMessage *dataMsg = new cMessage("data-message", TEST_DATA_MSG);
      sendToXY(dataMsg,centerx,centery);
      
      scheduleAt(timeDelay,testTimer);
      EV << "test timer" << endl;
      //delete msg;
      */
    }break;
    default:
        EV <<" Unkown self message! -> delete, kind: "<<msg->kind()<<endl;
    }
}

void BlueqApplLayer::handleLowerMsg(cMessage *msg)
{
  switch(msg->kind()){
  TEST_DATA_MSG:
    EV<< "recv a data message"<< endl;
    delete msg;
   default:
    EV << "Error! got a packet with unkown kind "<<msg->kind()<<endl;
  }
}

// send message to node that who located x,y
void BlueqApplLayer::sendToXY(cMessage *msg, int x, int y)
{
  int destNetwLoc = LOC(x,y);
  msg->setControlInfo(new NetwControlInfo(destNetwLoc));
  sendDown(msg);
}

void BlueqApplLayer::finish() 
{
    BasicApplLayer::finish();
    if(!testTimer->isScheduled()) delete testTimer;
}

