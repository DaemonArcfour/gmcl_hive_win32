#include "hive_lua_whitelist.h"

const char* GlobalWhitelist[] = {
	//Empty crap
	"",
	" ",
	"  ",
	"   ",
	// Arbitrary crap
	"achievements.SpawnedProp()",
	"deathScreen()",
	"hook.Run(\"OnUndo\",\"Prop\")",
	"language.Add(\"Undone_e2_spawned_entity\", \"E2 Spawned Entity\")",
	//CAC
	"RunString     ([=[RunString ([==[RunString ([===[timer.Create (\"\x01\", 60, 0, function () return RunString, CompileString, \"ABC\" end) return [====[DRAGON DILDOS]====] ]===])]==])]=], [=[LuaCmdTest]=])",
	"RunStringEx   ([=[RunString ([==[RunString ([===[timer.Create (\"\x02\", 60, 0, function () return RunString, CompileString, \"ABC\" end) return [====[DRAGON DILDOS]====] ]===])]==])]=], [=[LuaCmdTest]=])",
	"CompileString ([=[RunString ([==[RunString ([===[timer.Create (\"\x03\", 60, 0, function () return RunString, CompileString, \"ABC\" end) return [====[DRAGON DILDOS]====] ]===])]==])]=], [=[LuaCmdTest]=])",
	//Compiler
	"Compiler.native = function(self) return self.Scopes[0][\"A\"] end",
	"Compiler.native = function(self) return self.Scopes[0][\"B\"] end",
	"Compiler.native = function(self) return self.Scopes[0][\"Vector\"] end",
	"Compiler.native = function() return 0 end",
	//Safe hooks
	"hook.Run(\"AddHint\",\"PhysgunUnfreeze\",\"0.3\")",
	"hook.Run(\"AddHint\",\"PhysgunUse\",\"8\")"
	"hook.Run(\"AddHint\",\"PhysgunFreeze\",\"2\")"
	"hook.Run(\"AddHint\",\"VehicleView\",\"2\")",
	"hook.Run(\"OnUndo\",\"WireMagnet\")",
	"hook.Run(\"OnUndo\",\"gmod_wire_expression2\")",
	"hook.Run(\"OnUndo\",\"gmod_wire_value\")",
	"hook.Run(\"LimitHit\",\"sents\")",
	"hook.Run(\"OnUndo\",\"SENT\",\"Undone Health Kit\")",
	"RunConsoleCommand(\"M9KGasEffect\", \"0\")",
	"hook.Run(\"OnUndo\",\"SENT\",\"Undone Suit Battery\")",

	// PAC Related
	"local IN = select(1, ...) return nil, time()*250",
	"local IN = select(1, ...) return nil, nil, time()*700",
	"local IN = select(1, ...) return nil, nil, time()*500",
	"\0",

	//Erroring crap
	"_xpcall = xpcall xpcall = function( ... ) end",
	"_pcall = pcall pcall = function( ... ) end",
	"pcall = _pcall xpcall = _xpcall"
};

int WhitelistSize = (sizeof(GlobalWhitelist) / sizeof(GlobalWhitelist[0])) - 1;

bool isWhitelisted(const char* ccString) {
	int check = -5;
	for (int i = 0; i <= WhitelistSize; i++) {
		if (strcmp(ccString, GlobalWhitelist[i]) == 0) {
			check = 0;
			break;
		}
	}
	if (check == 0)
		return true;
	else
		return false;
}