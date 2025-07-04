#pragma once
typedef		void	*(__thiscall	*hive_func_RunString)			(void*, const char*, const char*, const char*, bool, bool);
typedef		void	*(__thiscall	*hive_func_RunStringEx)			(void*, char const*, char const*, char const*, bool, bool, bool, bool);
typedef		void	*(__thiscall	*hive_func_CompileString)		(void*, void*, char const&);
typedef		void	*(__thiscall	*hive_func_CreateLuaInterface)	(void*, uchar, bool);
typedef		void	*(__thiscall	*hive_func_CloseLuaInterface)	(void*, void*);
typedef		void	*(__thiscall	*hive_func_CalcView)			(void*, Vector&, QAngle&, float&, float&, float&);
typedef		void	*(__fastcall	*hive_func_RenderCapture)		(void*, void*);
typedef		void	*(__fastcall	*hive_func_SwepPrimaryAttack)	(void*, void*);
typedef		void	 (__thiscall	*hive_func_DrawModelExecute)	(void*, DrawModelState_t const&, ModelRenderInfo_t const&, matrix3x4_t *);
typedef		bool	 (__thiscall	*hive_func_ClientModeCreateMove)(void*, float, GMODCUserCmd*);
typedef		bool	 (__fastcall	*hive_func_CreateMove)			(void*, void*, int, float, bool);
typedef		void	 (__fastcall	*hive_func_FireBullets)			(void*, void*, void*);
typedef		void	 (__fastcall	*hive_func_PaintTraverse)		(void*, void*, VPANEL, bool, bool);
typedef		void	 (__fastcall	*hive_func_FrameStageNotify)	(void*, void*, ClientFrameStage_t);
typedef		bool	 (__fastcall	*hive_func_SetupBones)			(void*, void*, matrix3x4_t*, int, int, float);
typedef		bool	 (__thiscall	*hive_func_CLuaGameModeCallWithArgs)	(void*, int);
typedef		bool	 (__stdcall		*hive_func_CLuaGameModeCallFinish)		(void*, void*);
typedef		void	 (__thiscall	*hive_func_RunCommand)(void*, C_BasePlayerNew*, GMODCUserCmd*, void*);