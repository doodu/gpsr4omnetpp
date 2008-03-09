//
// Generated file, do not edit! Created by opp_msgc 3.3 from GPSRPkt.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "GPSRPkt_m.h"

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


Register_Class(GPSRPkt);

GPSRPkt::GPSRPkt(const char *name, int kind) : cMessage(name,kind)
{
    this->destAddr_var = 0;
    this->srcAddr_var = 0;
    this->ttl_var = 1;
    this->seqNum_var = 0;
    this->savedx_var = 0;
    this->savedy_var = 0;
    this->dstx_var = 0;
    this->dsty_var = 0;
}

GPSRPkt::GPSRPkt(const GPSRPkt& other) : cMessage()
{
    unsigned int i;
    setName(other.name());
    operator=(other);
}

GPSRPkt::~GPSRPkt()
{
    unsigned int i;
}

GPSRPkt& GPSRPkt::operator=(const GPSRPkt& other)
{
    if (this==&other) return *this;
    unsigned int i;
    cMessage::operator=(other);
    this->destAddr_var = other.destAddr_var;
    this->srcAddr_var = other.srcAddr_var;
    this->ttl_var = other.ttl_var;
    this->seqNum_var = other.seqNum_var;
    this->savedx_var = other.savedx_var;
    this->savedy_var = other.savedy_var;
    this->dstx_var = other.dstx_var;
    this->dsty_var = other.dsty_var;
    return *this;
}

void GPSRPkt::netPack(cCommBuffer *b)
{
    cMessage::netPack(b);
    doPacking(b,this->destAddr_var);
    doPacking(b,this->srcAddr_var);
    doPacking(b,this->ttl_var);
    doPacking(b,this->seqNum_var);
    doPacking(b,this->savedx_var);
    doPacking(b,this->savedy_var);
    doPacking(b,this->dstx_var);
    doPacking(b,this->dsty_var);
}

void GPSRPkt::netUnpack(cCommBuffer *b)
{
    cMessage::netUnpack(b);
    doUnpacking(b,this->destAddr_var);
    doUnpacking(b,this->srcAddr_var);
    doUnpacking(b,this->ttl_var);
    doUnpacking(b,this->seqNum_var);
    doUnpacking(b,this->savedx_var);
    doUnpacking(b,this->savedy_var);
    doUnpacking(b,this->dstx_var);
    doUnpacking(b,this->dsty_var);
}

int GPSRPkt::getDestAddr() const
{
    return destAddr_var;
}

void GPSRPkt::setDestAddr(int destAddr_var)
{
    this->destAddr_var = destAddr_var;
}

int GPSRPkt::getSrcAddr() const
{
    return srcAddr_var;
}

void GPSRPkt::setSrcAddr(int srcAddr_var)
{
    this->srcAddr_var = srcAddr_var;
}

int GPSRPkt::getTtl() const
{
    return ttl_var;
}

void GPSRPkt::setTtl(int ttl_var)
{
    this->ttl_var = ttl_var;
}

unsigned long GPSRPkt::getSeqNum() const
{
    return seqNum_var;
}

void GPSRPkt::setSeqNum(unsigned long seqNum_var)
{
    this->seqNum_var = seqNum_var;
}

int GPSRPkt::getSavedx() const
{
    return savedx_var;
}

void GPSRPkt::setSavedx(int savedx_var)
{
    this->savedx_var = savedx_var;
}

int GPSRPkt::getSavedy() const
{
    return savedy_var;
}

void GPSRPkt::setSavedy(int savedy_var)
{
    this->savedy_var = savedy_var;
}

int GPSRPkt::getDstx() const
{
    return dstx_var;
}

void GPSRPkt::setDstx(int dstx_var)
{
    this->dstx_var = dstx_var;
}

int GPSRPkt::getDsty() const
{
    return dsty_var;
}

void GPSRPkt::setDsty(int dsty_var)
{
    this->dsty_var = dsty_var;
}

class GPSRPktDescriptor : public cStructDescriptor
{
  public:
    GPSRPktDescriptor();
    virtual ~GPSRPktDescriptor();
    GPSRPktDescriptor& operator=(const GPSRPktDescriptor& other);
    virtual cPolymorphic *dup() const {return new GPSRPktDescriptor(*this);}

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

Register_Class(GPSRPktDescriptor);

GPSRPktDescriptor::GPSRPktDescriptor() : cStructDescriptor("cMessage")
{
}

GPSRPktDescriptor::~GPSRPktDescriptor()
{
}

int GPSRPktDescriptor::getFieldCount()
{
    return baseclassdesc ? 8+baseclassdesc->getFieldCount() : 8;
}

int GPSRPktDescriptor::getFieldType(int field)
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
        case 4: return FT_BASIC;
        case 5: return FT_BASIC;
        case 6: return FT_BASIC;
        case 7: return FT_BASIC;
        default: return FT_INVALID;
    }
}

const char *GPSRPktDescriptor::getFieldName(int field)
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
        case 4: return "savedx";
        case 5: return "savedy";
        case 6: return "dstx";
        case 7: return "dsty";
        default: return NULL;
    }
}

const char *GPSRPktDescriptor::getFieldTypeString(int field)
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
        case 4: return "int";
        case 5: return "int";
        case 6: return "int";
        case 7: return "int";
        default: return NULL;
    }
}

const char *GPSRPktDescriptor::getFieldEnumName(int field)
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

int GPSRPktDescriptor::getArraySize(int field)
{
    if (baseclassdesc) {
        if (field < baseclassdesc->getFieldCount())
            return baseclassdesc->getArraySize(field);
        field -= baseclassdesc->getFieldCount();
    }
    GPSRPkt *pp = (GPSRPkt *)p;
    switch (field) {
        default: return 0;
    }
}

bool GPSRPktDescriptor::getFieldAsString(int field, int i, char *resultbuf, int bufsize)
{
    if (baseclassdesc) {
        if (field < baseclassdesc->getFieldCount())
            return baseclassdesc->getFieldAsString(field,i,resultbuf,bufsize);
        field -= baseclassdesc->getFieldCount();
    }
    GPSRPkt *pp = (GPSRPkt *)p;
    switch (field) {
        case 0: long2string(pp->getDestAddr(),resultbuf,bufsize); return true;
        case 1: long2string(pp->getSrcAddr(),resultbuf,bufsize); return true;
        case 2: long2string(pp->getTtl(),resultbuf,bufsize); return true;
        case 3: long2string(pp->getSeqNum(),resultbuf,bufsize); return true;
        case 4: long2string(pp->getSavedx(),resultbuf,bufsize); return true;
        case 5: long2string(pp->getSavedy(),resultbuf,bufsize); return true;
        case 6: long2string(pp->getDstx(),resultbuf,bufsize); return true;
        case 7: long2string(pp->getDsty(),resultbuf,bufsize); return true;
        default: return false;
    }
}

bool GPSRPktDescriptor::setFieldAsString(int field, int i, const char *value)
{
    if (baseclassdesc) {
        if (field < baseclassdesc->getFieldCount())
            return baseclassdesc->setFieldAsString(field,i,value);
        field -= baseclassdesc->getFieldCount();
    }
    GPSRPkt *pp = (GPSRPkt *)p;
    switch (field) {
        case 0: pp->setDestAddr(string2long(value)); return true;
        case 1: pp->setSrcAddr(string2long(value)); return true;
        case 2: pp->setTtl(string2long(value)); return true;
        case 3: pp->setSeqNum(string2long(value)); return true;
        case 4: pp->setSavedx(string2long(value)); return true;
        case 5: pp->setSavedy(string2long(value)); return true;
        case 6: pp->setDstx(string2long(value)); return true;
        case 7: pp->setDsty(string2long(value)); return true;
        default: return false;
    }
}

const char *GPSRPktDescriptor::getFieldStructName(int field)
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

void *GPSRPktDescriptor::getFieldStructPointer(int field, int i)
{
    if (baseclassdesc) {
        if (field < baseclassdesc->getFieldCount())
            return baseclassdesc->getFieldStructPointer(field, i);
        field -= baseclassdesc->getFieldCount();
    }
    GPSRPkt *pp = (GPSRPkt *)p;
    switch (field) {
        default: return NULL;
    }
}

sFieldWrapper *GPSRPktDescriptor::getFieldWrapper(int field, int i)
{
    return NULL;
}

