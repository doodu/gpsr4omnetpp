/* -*- mode:c++ -*- ********************************************************
 * file:        BurstApplLayer.h
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


#ifndef BLUEQ_APPL_LAYER_H
#define BLUEQ_APPL_LAYER_H

#include "GPSRPkt_m.h"
#include "BasicApplLayer.h"
#include <list>
/**
 * @brief Application layer to test lower layer implementations
 *
 * This application layer does exactly the same as the
 * TestApplLayer. The only difference is that is sends a burst of
 * broadcast messages instead of just one. The burst size is specified
 * in burstSize and can be set in omnetpp.ini
 *
 * @sa TestApplALyer
 * @ingroup applLayer
 * @author BlueQ
 **/


class BlueqApplLayer : public BasicApplLayer
{
 public:
  Module_Class_Members( BlueqApplLayer, BasicApplLayer, 0 );
  /** @brief Initialite module parameters*/
  virtual void initialize(int);
  virtual void finish();
  enum APPL_MSG_TYPES{
    TEST_TIMER_MSG,
    
    TEST_DATA_MSG,
    CREATE_LINK_MSG,		// use this packet to create the test link
    QUERY_MSG,			// the query message
    NULL_MSG
  }; 
protected:
  cMessage *testTimer;
  // the parameters form omnetpp.ini
  int centerx,centery;		// the center node's location
  int selfx,selfy;			// the location of myself
  int link_startx,link_starty,link_endx,link_endy; // use to produce the link
  int query_nodex,query_nodey;			   // the query node's location
protected:
  /** @brief Handle self messages such as timer... */
  virtual void handleSelfMsg(cMessage*);
  virtual void handleLowerMsg(cMessage*);
  /** @brief send message to the node which located at x,y*/
  void sendToXY(cMessage* msg, int x, int y);
};

#endif
 
