//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef INETCHANNEL_H
#define INETCHANNEL_H
#ifdef _WIN32
#pragma once
#endif

#include "tier0/platform.h"
#include "inetchannelinfo.h"
#include "tier1/bitbuf.h"

class	IDemoRecorder;
class	INetChannelHandler;
class   INetMessage;
class	INetChannelInfo;
typedef struct netpacket_s netpacket_t;
typedef struct netadr_s	netadr_t;
enum RequestFile_t
{
    DOWNLOADABLE,
    CUSTOMFILE
};



abstract_class INetChannel : public INetChannelInfo
{
public:
    virtual ~INetChannel();
    virtual void SetDataRate(float);
    virtual bool RegisterMessage(INetMessage*);
    virtual bool StartStreaming(unsigned int);
    virtual void ResetStreaming(void);
    virtual void SetTimeout(float);
    virtual void SetDemoRecorder(IDemoRecorder*);
    virtual void SetChallengeNr(unsigned int);
    virtual void Reset(void);
    virtual void Clear(void);
    virtual void Shutdown(char const*);
    virtual void ProcessPlayback(void);
    virtual bool ProcessStream(void);
    virtual void ProcessPacket(netpacket_t*, bool);
    virtual bool SendNetMsg(INetMessage&, bool, bool);
    virtual bool SendData(bf_write&, bool);
    virtual bool SendFile(char const*, unsigned int);
    virtual void DenyFile(unsigned int);
    virtual void RequestFile_OLD(char const*, unsigned int);
    virtual void SetChoked(void);
    virtual int SendDatagram(bf_write*);
    virtual bool Transmit(bool);
    virtual const netadr_t& GetRemoteAddress(void)const;
    virtual INetChannelHandler* GetMsgHandler(void)const;
    virtual int GetDropNumber(void)const;
    virtual int GetSocket(void)const;
    virtual unsigned int GetChallengeNr(void)const;
    virtual void GetSequenceData(int&, int&, int&);
    virtual void SetSequenceData(int, int, int);
    virtual void UpdateMessageStats(int, int);
    virtual bool CanPacket(void)const;
    virtual bool IsOverflowed(void)const;
    virtual bool IsTimedOut(void)const;
    virtual bool HasPendingReliableData(void);
    virtual void SetFileTransmissionMode(bool);
    virtual void SetCompressionMode(bool);
    virtual unsigned int RequestFile(RequestFile_t, unsigned int);
    virtual void SetMaxBufferSize(bool, int, bool);
    virtual bool IsNull(void)const;
    virtual int GetNumBitsWritten(bool);
    virtual void SetInterpolationAmount(float);
    virtual void SetRemoteFramerate(float, float);
    virtual void SetMaxRoutablePayloadSize(int);
    virtual void GetMaxRoutablePayloadSize(void);
    virtual int GetProtocolVersion(void);
};



#endif // INETCHANNEL_H
