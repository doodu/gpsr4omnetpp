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

/* echo node of this network */

struct Node
{
  int addr;
  int x;
  int y;
  int watchDog;			// 当看门狗的值小于或等于0时删除结点
};
class BlueqApplLayer : public BasicApplLayer
{
 public:
  Module_Class_Members( BlueqApplLayer, BasicApplLayer, 0 );
  /** @brief Initialite module parameters*/
  virtual void initialize(int);
  virtual void finish();
    enum GPSR_MSG_TYPES{
	SEND_BROADCAST_TIMER,
	BROADCAST_MESSAGE,
	BROADCAST_REPLY_MESSAGE,

	//for routing
	SEND_BEACON_TIMER,
	REFALSH_ROUTE_TIMER,
	BEACON_MESSAGE, // for location 
	DATA_MESSAGE		  // for data
    }; 

  enum DATA_MSG_MODE{
    GREEDY_MODE,		// greedy forwarding
    PERIMETER_MODE		// perimeter forwarding
  };
protected:

  // routing
  /** @brief Routing Table */
  std::list<Node> routeTable;
  /** @brief beacon packet delay */
  double beaconDelay;
  /** @brief if time > beanconTime then remove the node from routing table */
  int maxWatchDog;
  /** @bref position */
  int x,y;
  cMessage *beaconTimer;

  /** @brief Number of messages to send in a burst*/
  int burstSize;
  cMessage *delayTimer;

 protected:
  /** @brief Handle self messages such as timer... */
  virtual void handleSelfMsg(cMessage*);
  virtual void handleLowerMsg(cMessage*);
  /** @brief send a broadcast packet to all connected neighbors */
  void sendBroadcast();
  /** @brief send a beacon packet to all connected neighbors */
  void sendBeacon();
  /** @brief send a reply to a broadcast message */
  void sendReply(GPSRPkt *msg); 
};

#endif
 
