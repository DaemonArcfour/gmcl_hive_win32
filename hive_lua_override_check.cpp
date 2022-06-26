#include "hive_lua_override_check.h"
std::vector<COriginalFunction*> LUA_OriginalFunctions;
std::vector<COriginalFunction*> LUA_Functions;

const char* CheckFunctions[] = {
	"debug.getinfo",
	"debug.Trace",
	"debug.debug",
	"debug.getfenv",
	"debug.gethook",
	"debug.getinfo",
	"debug.getlocal",
	"debug.getmetatable",
	"debug.getregistry",
	"debug.getupvalue",
	"debug.setfenv",
	"debug.sethook",
	"debug.setmetatable",
	"debug.traceback",
	"jit.attach",
	"jit.flush",
	"jit.off",
	"jit.on",
	"jit.opt.start",
	"jit.status",
	"jit.util.funcbc",
	"jit.util.funcinfo",
	"jit.util.funck",
	"jit.util.funcuvname",
	"jit.util.ircalladdr",
	"jit.util.traceexitstub",
	"jit.util.traceinfo",
	"jit.util.traceir",
	"jit.util.tracek",
	"jit.util.tracemc",
	"jit.util.tracesnap",
	"hook.Add",
	"hook.Call",
	"hook.GetTable",
	"hook.Remove",
	"hook.Run",
	"http.Fetch",
	"http.Post",
	"HTTP",
	"jit.util.funcinfo",
	"require",
	"render.Capture",
	"render.CapturePixels",
	"pcall",
	"xpcall",
	"net.Start",
	"net.Receive",
	"net.SendToServer",
	"RunString",
	"RunStringEx",
	"CompileString",
	"CompileFile",
	"RunConsoleCommand",
	"concommand.GetTable",
	"file.Read",
	"file.Find",
	"file.Exists",
	"file.Size"
};
const int OF_list_size = (sizeof(CheckFunctions) / sizeof(CheckFunctions[0])) - 1;

void HiveLUA::GetFunctions(ILuaInterface* ClientInterface, std::vector<COriginalFunction*> &Functions) {
	for (int i = 0; i < OF_list_size; i++) {
		int Fields = 0;
		ClientInterface->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB); 
		Fields++;
		std::string CurrentFunction = CheckFunctions[i];
		size_t pos = 0;
		
		std::string LastMember;
		while ((pos = CurrentFunction.find(".")) != std::string::npos) {
			LastMember = CurrentFunction.substr(0, pos);
			ClientInterface->GetField(-1, LastMember.c_str());
			Fields++;
			CurrentFunction.erase(0, pos + 1);
		}
		ClientInterface->GetField(-1, CurrentFunction.c_str());
		_CLuaInterface *state = (_CLuaInterface*)ClientInterface;
		DWORD OriginalAddress = (DWORD)lua_topointer(state->L, -1);
		Fields += 1;
		COriginalFunction* SaveOriginal = new COriginalFunction;
		SaveOriginal->function_name = CheckFunctions[i];
		SaveOriginal->original_address = OriginalAddress;
		Functions.push_back(SaveOriginal);
		ClientInterface->Pop(Fields);
	}
}

void HiveLUA::CompareFunctionLists(std::vector<COriginalFunction*> FListOriginal, std::vector<COriginalFunction*> FListNew, std::vector<COriginalFunction*> &FListOverridden) {
	int FLOrgSz = FListOriginal.size() - 1, FLNewSz = FListNew.size() - 1;
	if (FLOrgSz != FLNewSz)
		return;

	for (int i = 0; i < FLOrgSz; i++) {
		if (FListOriginal.at(i)->original_address != FListNew.at(i)->original_address) {
			COriginalFunction *Overridden = new COriginalFunction;
			Overridden->function_name = FListOriginal.at(i)->function_name;
			Overridden->original_address = FListOriginal.at(i)->original_address;
			Overridden->new_address = FListNew.at(i)->original_address;
			FListOverridden.push_back(Overridden);
		}
	}
}

void HiveLUA::RecycleFunctionList(std::vector<COriginalFunction*> &FunctionList) {
	int FLSize = FunctionList.size() - 1;
	for (int i = 0; i < FLSize; i++) {
		if(FunctionList.at(i))
			delete FunctionList.at(i); // Good Riddance!
	}
	FunctionList.clear();
}

