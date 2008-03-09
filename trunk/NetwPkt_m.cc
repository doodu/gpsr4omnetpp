//
// Generated file, do not edit! Created by opp_msgc 3.3 from NetwPkt.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "NetwPkt_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw new cException("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}
template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw new cException("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

// Automatically supply array (un)packing functions
template<typename T>
void doPacking(cCommBuffer *b, T *t, int n) {
    for (int i=0; i<n; i++)
        doPacking(b,t[i]);
}
template<typename T>
void doUnpacking(cCommBuffer *b, T *t, int n) {
    for (int i=0; i<n; i++)
        doUnpacking(b,t[i]);
}
inline void doPacking(cCommBuffer *, cPolymorphic&) {}
inline void doUnpacking(cCommBuffer *, cPolymorphic&) {}

#define DOPACKING(T,R) \
    inline void doPacking(cCommBuffer *b, T R a) {b->pack(a);}  \
    inline void doPacking(cCommBuffer *b, T *a, int n) {b->pack(a,n);}  \
    inline void doUnpacking(cCommBuffer *b, T& a) {b->unpack(a);}  \
    inline void doUnpacking(cCommBuffer *b, T *a, int n) {b->unpack(a,n);}
#define _
DOPACKING(char,_)
DOPACKING(unsigned char,_)
DOPACKING(bool,_)
DOPACKING(short,_)
DOPACKING(unsigned short,_)
DOPACKING(int,_)
DOPACKING(unsigned int,_)
DOPACKING(long,_)
DOPACKING(unsigned long,_)
DOPACKING(float,_)
DOPACKING(double,_)
DOPACKING(long double,_)
DOPACKING(char *,_)
DOPACKING(const char *,_)
DOPACKING(opp_string,&)
//DOPACKING(std::string,&)
#undef _
#undef DOPACKING


Register_Class(NetwPkt);

NetwPkt::NetwPkt(const char *name, int kind) : cMessage(name,kind)
{
    this->destAddr_var = 0;
    this->srcAddr_var = 0;
    this->ttl_var = 1;
    this->seqNum_var = 0;
}

NetwPkt::NetwPkt(const NetwPkt& other) : cMessage()
{
    unsigned int i;
    setName(other.name());
    operator=(other);
}

NetwPkt::~NetwPkt()
{
    unsigned int i;
}

NetwPkt& NetwPkt::operator=(const NetwPkt& other)
{
    if (this==&other) return *this;
    unsigned int i;
    cMessage::operator=(other);
    this->destAddr_var = other.destAddr_var;
    this->srcAddr_var = other.srcAddr_var;
    this->ttl_var = other.ttl_var;
    this->seqNum_var = other.seqNum_var;
    return *this;
}

void NetwPkt::netPack(cCommBuffer *b)
{
    cMessage::netPack(b);
    doPacking(b,this->destAddr_var);
    doPacking(b,this->srcAddr_var);
    doPacking(b,this->ttl_var);
    doPacking(b,this->seqNum_var);
}

void NetwPkt::netUnpack(cCommBuffer *b)
{
    cMessage::netUnpack(b);
    doUnpacking(b,this->destAddr_var);
    doUnpacking(b,this->srcAddr_var);
    doUnpacking(b,this->ttl_var);
    doUnpacking(b,this->seqNum_var);
}

int NetwPkt::getDestAddr() const
{
    return destAddr_var;
}

void NetwPkt::setDestAddr(int destAddr_var)
{
    this->destAddr_var = destAddr_var;
}

int NetwPkt::getSrcAddr() const
{
    return srcAddr_var;
}

void NetwPkt::setSrcAddr(int srcAddr_var)
{
    this->srcAddr_var = srcAddr_var;
}

int NetwPkt::getTtl() const
{
    return ttl_var;
}

void NetwPkt::setTtl(int ttl_var)
{
    this->ttl_var = ttl_var;
}

unsigned long NetwPkt::getSeqNum() const
{
    return seqNum_var;
}

void NetwPkt::setSeqNum(unsigned long seqNum_var)
{
    this->seqNum_var = seqNum_var;
}

class NetwPktDescriptor : public cStructDescriptor
{
  public:
    NetwPktDescriptor();
    virtual ~NetwPktDescriptor();
    NetwPktDescriptor& operator=(const NetwPktDescriptor& other);
    virtual cPolymorphic *dup() const {return new NetwPktDescriptor(*this);}

    virtual int getFieldCount();
    virtual const char *getFieldName(int field);
    virtual int getFieldType(int field);
    virtual const char *getFieldTypeString(int field);
    virtual const char *getFieldEnumName(int field);
    virtual int getArraySize(int field);

    virtual bool getFieldAsString(int field, int i, char *resultbuf, int bufsize);
    virtual bool setFieldAsString(int field, int i, const char *value);

    virtual const char *getFieldStructName(int field);
    virtual void *getFieldStructPointer(int field, int i);
    virtual sFieldWrapper *getFieldWrapper(int field, int i);
};

Register_Class(NetwPktDescriptor);

NetwPktDescriptor::NetwPktDescriptor() : cStructDescriptor("cMessage")
{
}

NetwPktDescriptor::~NetwPktDescriptor()
{
}

int NetwPktDescriptor::getFieldCount()
{
    return baseclassdesc ? 4+baseclassdesc->getFieldCount() : 4;
}

int NetwPktDescriptor::getFieldType(int field)
{
    if (baseclassdesc) {
        if (field < baseclassdesc->getFieldCount())
            return baseclassdesc->getFieldType(field);
        field -= baseclassdesc->getFieldCount();
    }
    switch (field) {
        case 0: return FT_BASIC;
        case 1: return FT_BASIC;
        case 2: return FT_BASIC;
        case 3: return FT_BASIC;
        default: return FT_INVALID;
    }
}

const char *NetwPktDescriptor::getFieldName(int field)
{
    if (baseclassdesc) {
        if (field < baseclassdesc->getFieldCount())
            return baseclassdesc->getFieldName(field);
        field -= baseclassdesc->getFieldCount();
    }
    switch (field) {
        case 0: return "destAddr";
        case 1: return "srcAddr";
        case 2: return "ttl";
        case 3: return "seqNum";
        default: return NULL;
    }
}

const char *NetwPktDescriptor::getFieldTypeString(int field)
{
    if (baseclassdesc) {
        if (field < baseclassdesc->getFieldCount())
            return baseclassdesc->getFieldTypeString(field);
        field -= baseclassdesc->getFieldCount();
    }
    switch (field) {
        case 0: return "int";
        case 1: return "int";
        case 2: return "int";
        case 3: return "unsigned long";
        default: return NULL;
    }
}

const char *NetwPktDescriptor::getFieldEnumName(int field)
{
    if (baseclassdesc) {
        if (field < baseclassdesc->getFieldCount())
            return baseclassdesc->getFieldEnumName(field);
        field -= baseclassdesc->getFieldCount();
    }
    switch (field) {
        default: return NULL;
    }
}

int NetwPktDescriptor::getArraySize(int field)
{
    if (baseclassdesc) {
        if (field < baseclassdesc->getFieldCount())
            return baseclassdesc->getArraySize(field);
        field -= baseclassdesc->getFieldCount();
    }
    NetwPkt *pp = (NetwPkt *)p;
    switch (field) {
        default: return 0;
    }
}

bool NetwPktDescriptor::getFieldAsString(int field, int i, char *resultbuf, int bufsize)
{
    if (baseclassdesc) {
        if (field < baseclassdesc->getFieldCount())
            return baseclassdesc->getFieldAsString(field,i,resultbuf,bufsize);
        field -= baseclassdesc->getFieldCount();
    }
    NetwPkt *pp = (NetwPkt *)p;
    switch (field) {
        case 0: long2string(pp->getDestAddr(),resultbuf,bufsize); return true;
        case 1: long2string(pp->getSrcAddr(),resultbuf,bufsize); return true;
        case 2: long2string(pp->getTtl(),resultbuf,bufsize); return true;
        case 3: long2string(pp->getSeqNum(),resultbuf,bufsize); return true;
        default: return false;
    }
}

bool NetwPktDescriptor::setFieldAsString(int field, int i, const char *value)
{
    if (baseclassdesc) {
        if (field < baseclassdesc->getFieldCount())
            return baseclassdesc->setFieldAsString(field,i,value);
        field -= baseclassdesc->getFieldCount();
    }
    NetwPkt *pp = (NetwPkt *)p;
    switch (field) {
        case 0: pp->setDestAddr(string2long(value)); return true;
        case 1: pp->setSrcAddr(string2long(value)); return true;
        case 2: pp->setTtl(string2long(value)); return true;
        case 3: pp->setSeqNum(string2long(value)); return true;
        default: return false;
    }
}

const char *NetwPktDescriptor::getFieldStructName(int field)
{
    if (baseclassdesc) {
        if (field < baseclassdesc->getFieldCount())
            return baseclassdesc->getFieldStructName(field);
        field -= baseclassdesc->getFieldCount();
    }
    switch (field) {
        default: return NULL;
    }
}

void *NetwPktDescriptor::getFieldStructPointer(int field, int i)
{
    if (baseclassdesc) {
        if (field < baseclassdesc->getFieldCount())
            return baseclassdesc->getFieldStructPointer(field, i);
        field -= baseclassdesc->getFieldCount();
    }
    NetwPkt *pp = (NetwPkt *)p;
    switch (field) {
        default: return NULL;
    }
}

sFieldWrapper *NetwPktDescriptor::getFieldWrapper(int field, int i)
{
    return NULL;
}

