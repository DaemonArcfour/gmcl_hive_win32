#pragma once
#include "hive_dependencies.h"

class CNetChan
{
public:
	/*0	*/	virtual void* destr() = 0;
	/*1	*/	virtual void* GetName(void)const = 0;
	/*2	*/	virtual void* GetAddress(void)const = 0;
	/*3	*/	virtual void* GetTime(void)const = 0;
	/*4	*/	virtual void* GetTimeConnected(void)const = 0;
	/*5	*/	virtual void* GetBufferSize(void)const = 0;
	/*6	*/	virtual void* GetDataRate(void)const = 0;
	/*7	*/	virtual void* IsLoopback(void)const = 0;
	/*8	*/	virtual void* IsTimingOut(void)const = 0;
	/*9	*/	virtual void* IsPlayback(void)const = 0;
	/*10*/	virtual void* GetLatency(int)const = 0;
	/*11*/	virtual void* GetAvgLatency(int)const = 0;
	/*12*/	virtual void* GetAvgLoss(int)const = 0;
	/*13*/	virtual void* GetAvgChoke(int)const = 0;
	/*14*/	virtual void* GetAvgData(int)const = 0;
	/*15*/	virtual void* GetAvgPackets(int)const = 0;
	/*16*/	virtual void* GetTotalData(int)const = 0;
	/*17*/	virtual void* GetSequenceNr(int)const = 0;
	/*18*/	virtual void* IsValidPacket(int, int)const = 0;
	/*19*/	virtual void* GetPacketTime(int, int)const = 0;
	/*20*/	virtual void* GetPacketBytes(int, int, int)const = 0;
	/*21*/	virtual void* GetStreamProgress(int, int*, int*)const = 0;
	/*22*/	virtual void* GetTimeSinceLastReceived(void)const = 0;
	/*23*/	virtual void* GetCommandInterpolationAmount(int, int)const = 0;
	/*24*/	virtual void* GetPacketResponseLatency(int, int, int*, int*)const = 0;
	/*25*/	virtual void* GetRemoteFramerate(float*, float*)const = 0;
	/*26*/	virtual void* GetTimeoutSeconds(void)const = 0;
		//virtual void* unk1(void)const = 0;
	/*27*/	virtual void SetDataRate(float) = 0;
	/*28*/	virtual bool RegisterMessage(INetMessage*) = 0;
	/*29*/	virtual bool StartStreaming(unsigned int) = 0;
	/*30*/	virtual void ResetStreaming(void) = 0;
	/*31*/	virtual void SetTimeout(float) = 0;
	/*32*/	virtual void SetDemoRecorder(IDemoRecorder*) = 0;
	/*33*/	virtual void SetChallengeNr(unsigned int) = 0;
	/*34*/	virtual void Reset(void) = 0;
	/*35*/	virtual void Clear(void) = 0;
	/*36*/	virtual void Shutdown(char const*) = 0;
	/*37*/	virtual void ProcessPlayback(void) = 0;
	/*38*/	virtual bool ProcessStream(void) = 0;
	/*39*/	virtual void ProcessPacket(netpacket_s*, bool) = 0;
	/*40*/	virtual bool SendNetMsg(void*, bool, bool) = 0;
	/*41*/	virtual bool SendData(bf_write&, bool) = 0;
	/*42*/	virtual bool SendFile(char const*, unsigned int) = 0;
	/*43*/	virtual void DenyFile(unsigned int) = 0;
	/*44*/	virtual void RequestFile_OLD(char const*, unsigned int) = 0;
	/*45*/	virtual void SetChoked(void) = 0;
	/*46*/	virtual int SendDatagram(bf_write*) = 0;
	/*47*/	virtual bool Transmit(bool) = 0;
	/*48*/	virtual const netadr_t &GetRemoteAddress(void)const = 0;
	/*49*/	virtual INetChannelHandler *GetMsgHandler(void)const = 0;
	/*50*/	virtual int GetDropNumber(void)const = 0;
	/*51*/	virtual int GetSocket(void)const = 0;
	/*52*/	virtual unsigned int GetChallengeNr(void)const = 0;
	/*53*/	virtual void GetSequenceData(int&, int&, int&) = 0;
	/*54*/	virtual void SetSequenceData(int, int, int) = 0;
	/*55*/	virtual void UpdateMessageStats(int, int) = 0;
	/*56*/	virtual bool CanPacket(void)const = 0;
	/*57*/	virtual bool IsOverflowed(void)const = 0;
	/*58*/	virtual bool IsTimedOut(void)const = 0;
	/*59*/	virtual bool HasPendingReliableData(void) = 0;
	/*60*/	virtual void SetFileTransmissionMode(bool) = 0;
	/*61*/	virtual void SetCompressionMode(bool) = 0;
	/*62*/	virtual unsigned int RequestFile(RequestFile_t, unsigned int) = 0;
	/*63*/	virtual void SetMaxBufferSize(bool, int, bool) = 0;
	/*64*/	virtual bool IsNull(void)const = 0;
	/*65*/	virtual int GetNumBitsWritten(bool) = 0;
	/*66*/	virtual void SetInterpolationAmount(float) = 0;
	/*67*/	virtual void SetRemoteFramerate(float, float) = 0;
	/*68*/	virtual void SetMaxRoutablePayloadSize(int) = 0;
	/*69*/	virtual int GetMaxRoutablePayloadSize(void) = 0;
	/*70*/	virtual void* GetProtocolVersion(void) = 0;
};

class INetMessage
{
public:
	virtual	~INetMessage() {};

	// Use these to setup who can hear whose voice.
	// Pass in client indices (which are their ent indices - 1).

	virtual void	SetNetChannel(CNetChan* netchan) = 0; // netchannel this message is from/for
	virtual void	SetReliable(bool state) = 0;	// set to true if it's a reliable message

	virtual bool	Process(void) = 0; // calles the recently set handler to process this message

	virtual	bool	ReadFromBuffer(bf_read* buffer) = 0; // returns true if parsing was OK
	virtual	bool	WriteToBuffer(bf_write* buffer) = 0;	// returns true if writing was OK

	virtual bool	IsReliable(void) const = 0;  // true, if message needs reliable handling

	virtual int				GetType(void) const = 0; // returns module specific header tag eg svc_serverinfo
	virtual int				GetGroup(void) const = 0;	// returns net message group of this message
	virtual const char* GetName(void) const = 0;	// returns network message name, eg "svc_serverinfo"
	virtual CNetChan* GetNetChannel(void) const = 0;
	virtual const char* ToString(void) const = 0; // returns a human readable string about message content
private:
	bool m_bReliable;
	CNetChan* m_pChan;

};

class CGModNetMessage : public INetMessage
{
public:
	CGModNetMessage(int iChannel, bf_read* pBuf)
		: m_iChannel(iChannel), m_pBuf(pBuf)
	{}
	virtual int GetType() const { return 18; }
	virtual int GetGroup() const { return INetChannelInfo::GENERIC; }
	//virtual const char* GetName() const { return "clc_GMod_ClientToServer"; }
	//virtual const char* ToString() const { return GetName(); }

	virtual bool WriteToBuffer(bf_write&);
public:
	int m_iChannel;
	bf_read* m_pBuf;
};