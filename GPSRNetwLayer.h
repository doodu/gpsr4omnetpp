/* -*- mode:c++ -*- ********************************************************
 * file:        GPSRNetwLayer.h
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
 * description: network layer: general class for the network layer
 *              subclass to create your own network layer
 **************************************************************************/


#ifndef GPSR_NETW_LAYER_H
#define GPSR_NETW_LAYER_H

#include <omnetpp.h>

#include <BasicLayer.h>

#include "SimpleArp.h"
#include "GPSRPkt_m.h"
#include "SimpleAddress.h"
#include <list>

/**
 * @brief Simple class for the network layer
 * 
 * @ingroup netwLayer
 * @author Daniel Willkomm
 **/
struct Node
{
  int addr;
  int x;
  int y;
  int watchDog;			// 当看门狗的值小于或等于0时删除结点
};
class GPSRNetwLayer : public BasicLayer
{

  protected:
    /**
     * @brief Length of the GPSRPkt header 
     * Read from omnetpp.ini 
     **/
    int headerLength;
    
    /** @brief Pointer to the arp module*/
    SimpleArp* arp;
    
    /** @brief cached variable of my networ address */
    int myNetwAddr;

  // for routing 
  enum GPSR_MSG_TYPES{
    SEND_BEACON_TIMER,
    BEACON_MESSAGE, // for location 
    DATA_MESSAGE		  // for data
  }; 

  enum DATA_MSG_MODE{
    GREEDY_MODE,		// greedy forwarding
    PERIMETER_MODE		// perimeter forwarding
  };
    
public:
    Module_Class_Members(GPSRNetwLayer,BasicLayer,0);

    /** @brief Initialization of the module and some variables*/
    virtual void initialize(int);
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
    /** 
     * @name Handle Messages
     * @brief Functions to redefine by the programmer
     *
     * These are the functions provided to add own functionality to your
     * modules. These functions are called whenever a self message or a
     * data message from the upper or lower layer arrives respectively.
     *
     **/
    /*@{*/
    
    /** @brief Handle messages from upper layer */
    virtual void handleUpperMsg(cMessage* msg);
    
    /** @brief Handle messages from lower layer */
    virtual void handleLowerMsg(cMessage* msg);

    /** @brief Handle self messages */
    virtual void handleSelfMsg(cMessage* msg);

    /** @brief Handle control messages from lower layer */
    virtual void handleLowerControl(cMessage* msg) {
        EV << "deleting control message from lower layer\n";
        delete msg;
    }

    /*@}*/

    /** @brief decapsulate higher layer message from GPSRPkt */
    virtual cMessage* decapsMsg(GPSRPkt*);  

    /** @brief Encapsulate higher layer packet into an GPSRPkt*/
    virtual GPSRPkt* encapsMsg(cMessage*);  
  /** @brief send beacon packet */
  void sendBeacon();

  int greedyForwarding(int x,int y);

  int perimerterForwarding(int x,int y);

  /** @brief update the route table with the beacon message */
  void updateRouteTable(GPSRPkt *pkt);

  /** @brief route the packet to network */
  void routeMsg(GPSRPkt *pkt);
};

#endif
