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
#include "Packet.h"

#include <SimpleAddress.h>
#include <list>

using namespace std;

// the link's entry
struct Entry{
  int x,y;
  double time;
};

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
      testTimer = new cMessage("test-timer",APPL_TEST_TIMER);
      queryTimer = new cMessage("query-timer", APPL_QUERY_TIMER);
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
      double timeDelay = simTime() + dblrand() * 100 + 100;
      scheduleAt(timeDelay, testTimer); // timer to send beacon
      timeDelay = simTime() + dblrand() * 100 + 200;
      scheduleAt(timeDelay, queryTimer);
    }
}


void BlueqApplLayer::handleSelfMsg(cMessage *msg)
{
  double timeDelay = simTime() + 1000 + dblrand() * 1000;
  
  switch(msg->kind()){
  case APPL_TEST_TIMER:{
    // use to create the query links
    // printf("start:(%d,%d)\nmy:(%d,%d)\n",link_startx,link_starty,selfx,selfy);
    if(selfx == link_startx && selfy == link_starty){
      CreateLinkPkt *createLinkMsg = new CreateLinkPkt("CreateLinkPkt",APPL_CREATE_LINK_PACKET);
      //createLinkMsg->setLength(1024);
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
  case APPL_QUERY_TIMER:{
    static int query_state;
    if(selfx == query_nodex && selfy == query_nodey){
      switch (query_state){
      case 0:{
  	     QueryPkt *pkt = new QueryPkt( "Query-time", APPL_QUERY_PACKET);
	     pkt->setType(QUERY_TIME);	// type = QUERY_TIME means query time
	     pkt->setQueryAddr(LOC(selfx,selfy));
	     sendToXY(pkt, centerx, centery);
      }break;
      default:break;
      }
      scheduleAt(simTime() + 100, queryTimer);
    }

  }break;
  default:
    EV <<" Unkown self message! -> delete, kind: "<<msg->kind()<<endl;
  }
}

void BlueqApplLayer::handleLowerMsg(cMessage *msg)
{

  switch(msg->kind()){
    
  case APPL_DATA_PACKET:
    EV<< "recv a data message"<< endl;
    delete msg;
    break;
  case APPL_GOT_CREATE_LINK_PACKET:{
    EV << "in the link: " << selfx << "," << selfy << endl;
    CreateLinkPkt *pkt = (CreateLinkPkt *) msg;
    nextx = pkt->getNextx();
    nexty = pkt->getNexty();
    arrTime = pkt->getTime();
    if(pkt->getEntry() != 0){
      //report entry, create a APPL_REPORT_PACKET,and send it to (centerx,centery)
      ReportEntryPkt *pkt = new ReportEntryPkt("APPL_REPORT_PACKET",APPL_REPORT_PACKET);
      pkt->setX(selfx);
      pkt->setY(selfy);
      pkt->setTime(arrTime);
      sendToXY(pkt,centerx,centery);
    }
    EV << "\tnext:" << nextx << "," << nexty << endl;
    EV << "\ttime:" << arrTime << endl;
    EV << "\tentry:" << pkt->getEntry() << endl;
    delete msg;
  }break;
  case APPL_REPORT_PACKET:{
    ReportEntryPkt *pkt = (ReportEntryPkt *) msg;
    struct Entry entry;
    entry.x = pkt->getX();
    entry.y = pkt->getY();
    entry.time = pkt->getTime();
    entryTable.push_back(entry);

    std::list<Entry>::iterator it;
    for(it = entryTable.begin(); it != entryTable.end(); it ++){
      EV << "entry:(" << it->x << "," << it->y << "," << it->time << ")" << endl;
    }
  }break;
  case APPL_QUERY_PACKET:{
    QueryPkt *pkt = (QueryPkt *) (msg);
    if(pkt->getType() == QUERY_TIME){
      ReplayTimePkt *rep = new ReplayTimePkt("replay-time", APPL_REPLAY_PACKET);
      double start_time = entryTable.begin()->time;
      double end_time = entryTable.begin()->time;
      int queryx,queryy;
      EV << entryTable.size() << ":" << endl;
      for(std::list<Entry>::iterator it = entryTable.begin(); it != entryTable.end(); it++){
        if(it->time > end_time){
	       end_time == it->time;
	     }
	     if(it->time < start_time){
	       start_time = it->time;
	     }
	     EV << start_time << "," << end_time << endl;
      }
      queryx = GETX( pkt->getQueryAddr() );
      queryy = GETY( pkt->getQueryAddr() );
      rep->setType( REPLAY_TIME );
      rep->setStart_time(start_time);
      rep->setEnd_time(end_time);
      sendToXY( rep, queryx, queryy);
      EV << "get a QUERY_TIME packet, replay time:" << start_time << "," << end_time <<endl;
      delete msg;
    }
  }break;
  case APPL_REPLAY_PACKET:{
    ReplayTimePkt *pkt = (ReplayTimePkt *) msg;
    if(pkt->getType() == REPLAY_TIME){
      double start_time,end_time;
      start_time = pkt->getStart_time();
      end_time = pkt->getEnd_time();

      EV << "TIME_REPLAY:" << start_time << "," << end_time << endl;
    }
  }break;
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

