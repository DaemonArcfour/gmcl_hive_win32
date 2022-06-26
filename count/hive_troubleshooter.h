#pragma once
#include "hive_dependencies.h"

namespace HiveTroubleshooter {
	void Print(std::string message, bool success);
	void PrintError(std::string msg, bool success, bool &write);
	void PrintInitizalization(std::string msg);
	void PrintLUA(std::string msg);
	void Warning();
	bool factoryErrors();
	bool interfaceErrors();
	bool hookErrors();
	bool protectorErrors();
	bool netvarErrors();
}

Color extern HIVE_CON;
Color extern HIVE_WHITE;
Color extern HIVE_RED;
Color extern HIVE_OK;
Color extern HIVE_TROUBLESHOOTER;
Color extern HIVE_LIGHTGREEN;
Color extern HIVE_PURPLE;
Color extern HIVE_LIGHTBLUE;
Color extern HIVE_DARKBLUE;