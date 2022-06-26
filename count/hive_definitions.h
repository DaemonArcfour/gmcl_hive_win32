#pragma once
typedef		void	*(__thiscall	*hive_func_RunString)			(void*, const char*, const char*, const char*, bool, bool);
typedef		void	*(__thiscall	*hive_func_RunStringEx)			(void*, char const*, char const*, char const*, bool, bool, bool, bool);
typedef		void	*(__thiscall	*hive_func_CompileString)		(void*, void*, char const&);
typedef		void	*(__thiscall	*hive_func_CreateLuaInterface)	(void *, uchar, bool);
typedef		void	*(__thiscall	*hive_func_CloseLuaInterface)	(void*, void*);
typedef		bool	 (__thiscall	*hive_func_CreateMove)			(void*, float, GMODCUserCmd*);