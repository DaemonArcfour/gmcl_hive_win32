#pragma once
#include "..\..\..\hive_dependencies.h"
typedef int(*CFunc) (lua_State* state);

class CLuaGameCallback;
class HLuaError;
class ILuaObject;
class ILuaInterface;

class ILuaInterface
{
public:
	virtual void*	Top(void) = 0;	// 0
	virtual void*	Push(int) = 0;	// 1
	virtual void*	Pop(int stackpos = 1) = 0;	// 2
	virtual void*	GetTable(int) = 0;	// 3
	virtual void*	GetField(int, char  const*) = 0;	// 4
	virtual void*	SetField(int, char  const*) = 0;	// 5
	virtual void*	CreateTable(void) = 0;	// 6
	virtual void*	SetTable(int) = 0;	// 7
	virtual void*	SetMetaTable(int) = 0;	// 8
	virtual ILuaObject*	GetMetaTable(int) = 0;	// 9
	virtual void*	Call(int, int) = 0;	// 10
	virtual void*	PCall(int, int, int) = 0;	// 11
	virtual void*	Equal(int, int) = 0;	// 12
	virtual void*	RawEqual(int, int) = 0;	// 13
	virtual void*	Insert(int) = 0;	// 14
	virtual void*	Remove(int) = 0;	// 15
	virtual void*	Next(int) = 0;	// 16
	virtual void*	NewUserdata(unsigned int) = 0;	// 17
	virtual void*	ThrowError(char  const*) = 0;	// 18
	virtual void*	CheckType(int, int) = 0;	// 19
	virtual void*	ArgError(int, char  const*) = 0;	// 20
	virtual void*	RawGet(int) = 0;	// 21
	virtual void*	RawSet(int) = 0;	// 22
	virtual const char*	GetString(int, unsigned int *) = 0;	// 23
	virtual double	GetNumber(int) = 0;	// 24
	virtual bool	GetBool(int) = 0;	// 25
	virtual CFunc	GetCFunction(int) = 0;	// 26
	virtual void*	GetUserdata(int) = 0;	// 27
	virtual void	PushNil(void) = 0;	// 28
	virtual void	PushString(char  const*, unsigned int iLen = 0) = 0;	// 29
	virtual void	PushNumber(double) = 0;	// 30
	virtual void	PushBool(bool) = 0;	// 31
	virtual void	PushCFunction(CFunc) = 0;	// 32
	virtual void	PushCClosure(CFunc, int) = 0;	// 33
	virtual void	PushUserdata(void*) = 0;	// 34
	virtual void*	ReferenceCreate(void) = 0;	// 35
	virtual void*	ReferenceFree(int) = 0;	// 36
	virtual void*	ReferencePush(int) = 0;	// 37
	virtual void*	PushSpecial(int) = 0;	// 38
	virtual void*	IsType(int, int) = 0;	// 39
	virtual void*	GetType(int) = 0;	// 40
	virtual void*	GetTypeName(int) = 0;	// 41
	virtual void*	CreateMetaTableType(char  const*, int) = 0;	// 42
	virtual void*	CheckString(int) = 0;	// 43
	virtual void*	CheckNumber(int) = 0;	// 44
	virtual void*	ObjLen(int) = 0;	// 45
	virtual void*	GetAngle(int) = 0; //46
	virtual void*	GetVector(int) = 0; //47
	virtual void*	PushAngle(QAngle const&) = 0;  // 48
	virtual void*	PushVector(Vector const&) = 0; // 49
	virtual void*	SetState(lua_State*) = 0; // 50
	virtual void*	CreateMetaTable(char const*) = 0; // 51
	virtual void* PushMetaTable(int) = 0; // 52
	virtual void* PushUserType(void*, int) = 0; // 53
	virtual void* SetUserType(int, void*) = 0; // 54
	virtual void* Init(void*, bool) = 0; // 55
	virtual void* Shutdown(void) = 0; // 56
	virtual void* Cycle(void) = 0; // 57
	virtual void* Global(void) = 0;  // 58
	virtual void* GetObject(int) = 0; // 59
	virtual void* PushLuaObject(ILuaObject*) = 0; // 60
	virtual void* PushLuaFunction(int(*)(lua_State*)) = 0; // 61
	virtual void* LuaError(char const*, int) = 0; // 62
	virtual void* TypeError(char const*, int) = 0; // 63
	virtual void* CallInternal(int, int) = 0; // 64
	virtual void* CallInternalNoReturns(int) = 0; // 65
	virtual void* CallInternalGetBool(int) = 0; //66
	virtual void* CallInternalGetString(int) = 0; //67
	virtual void* CallInternalGet(int, ILuaObject*) = 0; // 68
	virtual void* NewGlobalTable(char const*) = 0; //69
	virtual void* NewTemporaryObject(void) = 0; // 70
	virtual void* isUserData(int) = 0; // 71
	virtual void* GetMetaTableObject(char const*, int) = 0; // 72
	virtual void* GetMetaTableObject(int) = 0; // 73
	virtual void* GetReturn(int) = 0; // 74
	virtual void* IsServer(void) = 0; // 75
	virtual void* IsClient(void) = 0; // 76
	virtual void* IsDedicatedServer(void) = 0; // 77
	virtual void* DestroyObject(ILuaObject *) = 0; // 78
	virtual void* CreateObject(void) = 0; // 79
	virtual void* SetMember(ILuaObject *, ILuaObject *, ILuaObject *) = 0; // 80
	virtual void* GetNewTable(void) = 0; // 81
	virtual void* SetMember(ILuaObject *, float) = 0; // 82
	virtual void* SetMember(ILuaObject *, float, ILuaObject *) = 0; // 83
	virtual void* SetMember(ILuaObject *, char const*) = 0; // 84
	virtual void* SetMember(ILuaObject *, char const*, ILuaObject *) = 0; // 85
	virtual void* SetIsServer(bool) = 0; // 86
	virtual void* PushLong(long) = 0; // 87
	virtual void* GetFlags(int) = 0; // 88
	virtual void* FindOnObjectsMetaTable(int, int) = 0; // 89
	virtual void* FindObjectOnTable(int, int) = 0; // 90
	virtual void* SetMemberFast(ILuaObject *, int, int) = 0; // 91
	virtual void* RunString(char const*, char const*, char const*, bool, bool) = 0; // 92
	virtual void* IsEqual(ILuaObject *, ILuaObject *) = 0; //93
	virtual void* Error(char const*) = 0; //94
	virtual void* GetStringOrError(int) = 0; //95
	virtual void* RunLuaModule(char const*) = 0; // 96
	virtual void* FindAndRunScript(char const*, bool, bool, char const*, bool) = 0; // 97
	virtual void* SetPathID(char const*) = 0; // 98
	virtual void* GetPathID(void) = 0; // 99
	virtual void* ErrorNoHalt(char const*, ...) = 0; // 100
	virtual void* Msg(char const*, ...) = 0; // 101
	virtual void* PushPath(char const*) = 0; // 102
	virtual void* PopPath(void) = 0; // 103
	virtual void* GetPath(void) = 0; // 104
	virtual void* GetColor(int) = 0; // 105
	virtual void* PushColor(Color) = 0; // 106
	virtual void* GetStack(int, void *) = 0; // 107
	virtual void* GetInfo(char const*, void *) = 0; // 108
	virtual void* GetLocal(void *, int) = 0; // 109
	virtual void* GetUpvalue(int, int) = 0; // 110
	virtual void* RunStringEx(char const*, char const*, char const*, bool, bool, bool, bool) = 0; // 111
	virtual void* GetDataString(int, void **) = 0; //112
	virtual void* ErrorFromLua(char const*, ...) = 0; //113
	virtual void* GetCurrentLocation(void) = 0; //114
	virtual void* MsgColour(Color const&, char const*, ...) = 0; // 115
	virtual void* GetCurrentFile(std::string &) = 0; // 116
	virtual void* CompileString(void*, std::string const&) = 0; // 117
	virtual void* CallFunctionProtected(int, int, bool) = 0; // 118
	virtual void* Require(char const*) = 0; // 119
	virtual void* GetActualTypeName(int) = 0; // 120
	virtual void* PreCreateTable(int, int) = 0; // 121
	virtual void* PushPooledString(int) = 0; // 122
	virtual void* GetPooledString(int) = 0; //123
	virtual void* AddThreadedCall(void *) = 0; // 124
	CLuaGameCallback *m_pCallback;
};

class CLuaGameCallback
{
private:
	virtual ILuaObject* CLuaGameCallback::CreateLuaObject(void) = 0;
	virtual void CLuaGameCallback::DestroyLuaObject(ILuaObject *) = 0;
	virtual void CLuaGameCallback::ErrorPrint(char  const*, bool) = 0;
	virtual void CLuaGameCallback::Msg(char  const*, bool) = 0;
	virtual void CLuaGameCallback::MsgColour(char  const*, Color  const&) = 0;
	virtual void CLuaGameCallback::LuaError(HLuaError *) = 0;
	virtual void CLuaGameCallback::InterfaceCreated(ILuaInterface *) = 0;
};

class CLuaShared
{
public:
	virtual void padding00() = 0; // 0
	//~CLuaShared();
	virtual void *CLuaShared::Init(void * (*)(char  const*, int *), bool, void *, void *); // 1
	virtual void *CLuaShared::Shutdown(void); // 2
	virtual void *CLuaShared::DumpStats(void); // 3
	virtual void *CLuaShared::CreateLuaInterface(unsigned char, bool); // 4
	virtual void *CLuaShared::CloseLuaInterface(ILuaInterface *); // 5

	virtual ILuaInterface *CLuaShared::GetLuaInterface(unsigned char);
	virtual void *CLuaShared::LoadFile(std::string  const&, std::string  const&, bool, bool);
	virtual void *CLuaShared::GetCache(std::string  const&);
	virtual void *CLuaShared::MountLua(char  const*);
	virtual void *CLuaShared::MountLuaAdd(char  const*, char  const*);
	virtual void *CLuaShared::UnMountLua(char  const*);
	virtual void *CLuaShared::SetFileContents(char  const*, char  const*);
	virtual void *CLuaShared::SetLuaFindHook(void *);

};

class ILuaObject {
public:
	virtual void* Set(ILuaObject*);
	virtual void* SetFromStack(int);
	virtual void* UnReference(void);
	virtual void* GetType(void);
	virtual void* GetString(void);
	virtual void* GetFloat(void);
	virtual void* GetInt(void);
	virtual void* GetUserData(void);
	virtual void* SetMember(char const*);
	virtual void* SetMember(char const*, ILuaObject *);
	virtual void* SetMember(char const*, float);
	virtual void* SetMember(char const*, bool);
	virtual void* SetMember(char const*, char const*);
	virtual void* SetMember(char const*, int(*)(lua_State *));
	virtual void* GetMemberBool(char const*, bool);
	virtual void* GetMemberInt(char const*, int);
	virtual void* GetMemberFloat(char const*, float);
	virtual void* GetMemberStr(char const*, char const*);
	virtual void* GetMemberUserData(char const*, void *);
	virtual void* GetMemberUserData(float, void *);
	virtual void* GetMember(char const*, ILuaObject *);
	virtual void* GetMember(ILuaObject *, ILuaObject *);
	virtual void* SetMetaTable(ILuaObject *);
	virtual void* SetUserData(void *);
	virtual void* Push(void);
	virtual void* isNil(void);
	virtual void* isTable(void);
	virtual void* isString(void);
	virtual void* isNumber(void);
	virtual void* isFunction(void);
	virtual void* isUserData(void);
	virtual void* GetMember(float, ILuaObject *);
	virtual void* Remove_Me_1(char const*, void *);
	virtual void* SetMember(float);
	virtual void* SetMember(float, ILuaObject *);
	virtual void* SetMember(float, float);
	virtual void* SetMember(float, bool);
	virtual void* SetMember(float, char const*);
	virtual void* SetMember(float, int(*)(lua_State *));
	virtual void* GetMemberStr(float, char const*);
	virtual void* SetMember(ILuaObject *, ILuaObject *);
	virtual void* GetBool(void);
	virtual void* PushMemberFast(int);
	virtual void* SetMemberFast(int, int);
	virtual void* SetFloat(float);
	virtual void* SetString(char const*);
	virtual void* GetDouble(void);
	virtual void* SetMember_FixKey(char const*, float);
	virtual void* SetMember_FixKey(char const*, char const*);
	virtual void* SetMember_FixKey(char const*, ILuaObject *);
	virtual void* SetMember_FixKey(char const*, double);
	virtual void* SetMember_FixKey(char const*, int);
	virtual void* isBool(void);
	virtual void* SetMemberDouble(char const*, double);
	virtual void* SetMemberNil(char const*);
	virtual void* SetMemberNil(float);
	virtual void* Debug_IsUnreferenced(void);
	virtual void* Init(void);
	virtual void* SetFromGlobal(char const*);
	virtual void* GetStringLen(unsigned int *);
	virtual void* GetMemberUInt(char const*, unsigned int);
	virtual void* SetMember(char const*, unsigned long long);
	virtual void* SetReference(int);
	virtual void* RemoveMember(char const*);
	virtual void* RemoveMember(float);
	virtual void* MemberIsNil(char const*);
	virtual void* SetMemberDouble(float, double);
	virtual void* GetMemberDouble(char const*, double);
};