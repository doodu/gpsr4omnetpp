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

bool entry_comp(Entry &e1, Entry &e2){
  if(e1.time > e2.time) return false;
  else return true;
}
Define_Module(BlueqApplLayer);


/**
 * First we have to initialize the module from which we derived ours,
 * in this case BasicModule.
 *
 * Then we will set a timer to indicate the first time we will send a
 * message
 *
 **/

#define IS_LINK_START_NODE (selfx == link_startx && selfy == link_starty)
#define IS_LINK_END_NODE (selfx == link_endx && selfy == link_endy)
#define IS_CENTER_NODE (selfx = centerx && selfy == centery)
#define IS_QUERY_NODE (selfx = query_nodex && selfy  == query_nodey)

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
    if(IS_LINK_START_NODE){
      CreateLinkPkt *createLinkMsg = new CreateLinkPkt("CreateLinkPkt",APPL_CREATE_LINK_PACKET);
      //createLinkMsg->setLength(1024);
      sendToXY(createLinkMsg,link_endx,link_endy);
      sendEntryReport(simTime());
      EV<<"("<< selfx<<","<<selfy<<") send a create link message to ("<<link_endx<<","<<link_endy<<")"<<endl;
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
      case 0:{			// query the start time and the end time
  	     QueryPkt *pkt = new QueryPkt( "Query-time", APPL_QUERY_PACKET);
	     pkt->setType(QUERY_TIME);	// type = QUERY_TIME means query time
	     pkt->setQueryAddr(LOC(selfx,selfy));
	     sendToXY(pkt, centerx, centery);
	     query_state = 1;
      }break;
      case 1:{			// generate queries and send to the center node
	     QueryPkt *pkt = new QueryPkt( "Query-link", APPL_QUERY_PACKET);
	     pkt->setType(QUERY_LINK_TO_CENTER);
	     pkt->setQueryAddr(LOC(selfx,selfy));

	     // generate the start_time and end_time
	     double r1;
	     double r2;
	     double r3;
	     r1 = dblrand();
	     r2 = dblrand();
	     if(r1 > r2){	// make sure r1 > r2
	       r3 = r1;
	       r1 = r2;
	       r2 = r3;
	     }
	     double query_start_time,query_end_time;
	     query_start_time = start_time + r1 * (end_time - start_time);
	     query_end_time = start_time + r2 * (end_time - start_time);
	     pkt->setStart_time(query_start_time);
	     pkt->setEnd_time(query_end_time);
	     sendToXY(pkt, centerx, centery);

      }break;
      default:break;
      }
      static int count = 0;
      if(count < 100){		// create 1000 query
	     scheduleAt(simTime() + 100, queryTimer);
	     count ++;
      }
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
  case APPL_CREATE_LINK_PACKET:{
    nextx = selfx;
    nexty = selfy;
    arrTime = simTime();
  }break;
  case APPL_GOT_CREATE_LINK_PACKET:{
    EV << "in the link: " << selfx << "," << selfy << endl;
    CreateLinkPkt *pkt = (CreateLinkPkt *) msg;
    nextx = pkt->getNextx();
    nexty = pkt->getNexty();
    arrTime = pkt->getTime();
    if(pkt->getEntry() != 0){
      sendEntryReport(arrTime);
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
    entryTable.sort(entry_comp);
    for(it = entryTable.begin(); it != entryTable.end(); it ++){
      EV << "entry:(" << it->x << "," << it->y << "," << it->time << ")" << endl;
    }
  }break;
  case APPL_QUERY_PACKET:{
    QueryPkt *pkt = (QueryPkt *) (msg);
    if(pkt->getType() == QUERY_TIME){
      ReplayTimePkt *rep = new ReplayTimePkt("replay-time", APPL_REPLAY_PACKET);
      start_time = entryTable.begin()->time;
      end_time = entryTable.begin()->time;
      int queryx,queryy;
      EV << entryTable.size() << ":" << endl;
      for(std::list<Entry>::iterator it = entryTable.begin(); it != entryTable.end(); it++){
        if(it->time > end_time){
	       end_time = it->time;
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
    }else if(pkt->getType() == QUERY_LINK_TO_CENTER){
      int startNodeX = entryTable.begin()->x;
      int startNodeY = entryTable.begin()->y;
      if(IS_CENTER_NODE){ // if i'am the center node, route it to the index node
	     for(std::list<Entry>::iterator it = entryTable.begin(); it != entryTable.end(); it ++){
	       if(it->time > pkt->getStart_time()){
		 break;
	       }
	       startNodeX = it->x;
	       startNodeY = it->y;
	     }
	     pkt->setType(QUERY_LINK);
	     sendToXY(pkt,startNodeX,startNodeY); // send to one of 
      }
    }else if(pkt->getType() == QUERY_LINK){
       if(arrTime < pkt->getStart_time()){
	     sendToXY(pkt, nextx, nexty);
      }else if(arrTime >= pkt->getStart_time() && arrTime <= pkt->getEnd_time()){ // add to the dest_link
	     int arrSize;
	     arrSize = pkt->getDest_linkArraySize();
	     pkt->setDest_linkArraySize(arrSize + 1);
	     pkt->setDest_link(arrSize, LOC(selfx,selfy));
	     sendToXY(pkt, nextx, nexty);
      }else if (arrTime >= pkt->getEnd_time()){
	     int queryAddr = pkt->getQueryAddr();
	     pkt->setType(QUERY_REPLAY);
	     sendToXY(pkt, GETX(queryAddr), GETY(queryAddr));
      }else{
	     delete pkt;
      }
      EV << "QUERY LINK:"<< pkt->getStart_time() << "--" << pkt->getEnd_time() << endl;
    }else if(pkt->getType() == QUERY_REPLAY){
      double d = simTime()-pkt->creationTime();
      qtime.record(d);
      int len = pkt->getDest_linkArraySize();
      EV << "QUERY_REPLAY:"<<endl;
      for(int i = 0; i<len; i++){
	     int loc = pkt->getDest_link(i);
	     EV << GETX(loc) << "," << GETY(loc) <<endl;
      }
    }else{
      EV << "recive a query packet: unknow type" << endl;
    }
  }break;
  case APPL_REPLAY_PACKET:{
    ReplayTimePkt *pkt = (ReplayTimePkt *) msg;
    if(pkt->getType() == REPLAY_TIME){
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
  msg->removeControlInfo();
  msg->setControlInfo(new NetwControlInfo(destNetwLoc));
  sendDown(msg);
}

void BlueqApplLayer::sendEntryReport(double time)
{
  //report entry, create a APPL_REPORT_PACKET,and send it to (centerx,centery)
  ReportEntryPkt *pkt = new ReportEntryPkt("APPL_REPORT_PACKET",APPL_REPORT_PACKET);
  pkt->setX(selfx);
  pkt->setY(selfy);
  pkt->setTime(time);
  sendToXY(pkt,centerx,centery);
}
void BlueqApplLayer::finish() 
{
    BasicApplLayer::finish();
    if(!testTimer->isScheduled()) delete testTimer;
}

