#include "module.h"
#include <GarrysMod/Lua/Interface.h>
#include "lua.h"
#include "Bootil/Bootil.h"
#include <chrono>

class CSysTimerModule : public IModule
{
public:
	virtual void LuaInit(bool bServerInit) OVERRIDE;
	virtual void LuaShutdown() OVERRIDE;
	virtual void Think(bool bSimulating) OVERRIDE;
	virtual const char* Name() { return "systimer"; };
	virtual int Compatibility() { return LINUX32 | LINUX64 | WINDOWS32 | WINDOWS64; };
};

static CSysTimerModule g_pSysTimerModule;
IModule* pSysTimerModule = &g_pSysTimerModule;

struct ILuaTimer
{
	int function;
	const char* identifier; // Verify: Do we need to keep a reference so that gc won't collect it?
	double delay = 0;
	double repetitions = 0;
	bool active = true;
	bool simple = false; // true if it's a timer.Simple. Do I even use it? :|
	double next_run = 0;
	double next_run_time = 0;
	bool markdelete = false;
};

double GetTime()
{
	return std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count() / 1000;
}

std::vector<ILuaTimer*> g_pLuaTimers;
ILuaTimer* FindTimer(const char* name)
{
	for (ILuaTimer* timer : g_pLuaTimers)
	{
		if (!timer->simple && strcmp(name, timer->identifier) == 0)
		{
			return timer;
		}
	}

	return nullptr;
}

void RemoveTimers()
{
	std::vector<ILuaTimer*> timers;
	for (ILuaTimer* timer : g_pLuaTimers)
	{
		if (timer->markdelete)
		{
			delete timer;
		} else {
			timers.push_back(timer);
		}
	}
}

LUA_FUNCTION_STATIC(timer_Adjust)
{
	const char* name = LUA->CheckString(1);
	double delay = LUA->CheckNumber(2);

	ILuaTimer* timer = FindTimer(name);
	if (timer) {
		timer->delay = delay;
		if (LUA->IsType(3, GarrysMod::Lua::Type::Number))
			timer->repetitions = LUA->GetNumber(3);

		if (LUA->IsType(4, GarrysMod::Lua::Type::Function)) {
			LUA->ReferenceFree(timer->function);
			LUA->Push(4);
			timer->function = LUA->ReferenceCreate();
			LUA->Pop();
		}

		LUA->PushBool(true);
	} else
		LUA->PushBool(false);

	return 1;
}

LUA_FUNCTION_STATIC(timer_Check)
{
	return 0;
}

LUA_FUNCTION_STATIC(timer_Create)
{
	const char* name = LUA->CheckString(1);
	double delay = LUA->CheckNumber(2);
	double repetitions = LUA->CheckNumber(3);
	LUA->CheckType(4, GarrysMod::Lua::Type::Function);

	ILuaTimer* timer = FindTimer(name); // Reuse existing timer
	if (!timer)
		timer = new ILuaTimer;

	LUA->Push(4);
	timer->function = LUA->ReferenceCreate();
	LUA->Pop();

	timer->identifier = name;
	timer->delay = delay;
	timer->repetitions = repetitions;
	timer->next_run = delay;
	timer->next_run_time = GetTime() + delay;
	g_pLuaTimers.push_back(timer);

	return 0;
}

LUA_FUNCTION_STATIC(timer_Exists)
{
	const char* name = LUA->CheckString(1);

	LUA->PushBool(FindTimer(name) != nullptr);

	return 1;
}

LUA_FUNCTION_STATIC(timer_Pause)
{
	const char* name = LUA->CheckString(1);

	ILuaTimer* timer = FindTimer(name);
	if (timer) {
		if (timer->active) {
			timer->active = false;
			LUA->PushBool(true);
		} else
			LUA->PushBool(false);
	} else
		LUA->PushBool(false);

	return 1;
}

LUA_FUNCTION_STATIC(timer_Remove)
{
	const char* name = LUA->CheckString(1);

	ILuaTimer* timer = FindTimer(name);
	if (timer) {
		timer->markdelete = true;
		RemoveTimers();
	}

	return 0;
}

LUA_FUNCTION_STATIC(timer_RepsLeft)
{
	const char* name = LUA->CheckString(1);

	ILuaTimer* timer = FindTimer(name);
	if (timer) {
		if (timer->repetitions > 0)
			LUA->PushNumber(timer->repetitions);
		else
			LUA->PushNumber(0);
	} else
		LUA->PushNumber(0);

	return 1;
}

LUA_FUNCTION_STATIC(timer_Simple)
{
	double delay = LUA->CheckNumber(1);
	LUA->CheckType(2, GarrysMod::Lua::Type::Function);

	ILuaTimer* timer = new ILuaTimer;
	LUA->Push(2);
	timer->function = LUA->ReferenceCreate();
	LUA->Pop();

	timer->simple = true;
	timer->delay = delay;
	timer->repetitions = 1;
	timer->next_run = delay;
	timer->next_run_time = GetTime() + delay;
	g_pLuaTimers.push_back(timer);

	return 0;
}

LUA_FUNCTION_STATIC(timer_Start)
{
	const char* name = LUA->CheckString(1);

	ILuaTimer* timer = FindTimer(name);
	if (timer) {
		if (!timer->active) {
			timer->active = true;
			timer->next_run_time = GetTime() + timer->delay;
			LUA->PushBool(true);
		} else
			LUA->PushBool(false);
	} else
		LUA->PushBool(false);

	return 1;
}

LUA_FUNCTION_STATIC(timer_Stop)
{
	const char* name = LUA->CheckString(1);

	ILuaTimer* timer = FindTimer(name);
	if (timer) {
		if (timer->active) {
			timer->active = false;
			timer->next_run = timer->delay;
			LUA->PushBool(true);
		} else
			LUA->PushBool(false);
	} else
		LUA->PushBool(false);

	return 1;
}

LUA_FUNCTION_STATIC(timer_TimeLeft)
{
	const char* name = LUA->CheckString(1);

	ILuaTimer* timer = FindTimer(name);
	if (timer)
		LUA->PushNumber(timer->next_run);
	else
		LUA->PushNumber(0);

	return 1;
}

LUA_FUNCTION_STATIC(timer_Toggle)
{
	const char* name = LUA->CheckString(1);

	ILuaTimer* timer = FindTimer(name);
	if (timer) {
		timer->active = !timer->active;
		timer->next_run_time = GetTime() + timer->next_run;
		LUA->PushBool(timer->active);
	} else
		LUA->PushBool(false);

	return 1;
}

LUA_FUNCTION_STATIC(timer_UnPause)
{
	const char* name = LUA->CheckString(1);

	ILuaTimer* timer = FindTimer(name);
	if (timer) {
		timer->active = true;
		timer->next_run_time = GetTime() + timer->next_run;
		LUA->PushBool(true);
	} else
		LUA->PushBool(false);

	return 1;
}

void CSysTimerModule::LuaInit(bool bServerInit)
{
	if (!bServerInit)
	{
		Util::StartTable();
			Util::AddFunc(timer_Adjust, "Adjust");
			Util::AddFunc(timer_Check, "Check");
			Util::AddFunc(timer_Create, "Create");
			Util::AddFunc(timer_Remove, "Destroy");
			Util::AddFunc(timer_Exists, "Exists");
			Util::AddFunc(timer_Pause, "Pause");
			Util::AddFunc(timer_Remove, "Remove");
			Util::AddFunc(timer_RepsLeft, "RepsLeft");
			Util::AddFunc(timer_Simple, "Simple");
			Util::AddFunc(timer_Start, "Start");
			Util::AddFunc(timer_Stop, "Stop");
			Util::AddFunc(timer_TimeLeft, "TimeLeft");
			Util::AddFunc(timer_Toggle, "Toggle");
			Util::AddFunc(timer_UnPause, "UnPause");
		Util::FinishTable("systimer");
	}
}

void CSysTimerModule::LuaShutdown()
{
	for (ILuaTimer* timer : g_pLuaTimers)
		delete timer;

	g_pLuaTimers.clear(),

	g_Lua->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		g_Lua->PushNil();
		g_Lua->SetField(-2, "systimer");
	g_Lua->Pop(1);
}

void CSysTimerModule::Think(bool simulating) // Should also be called while hibernating so we should be fine.
{
	double time = GetTime();
	for (ILuaTimer* timer : g_pLuaTimers)
	{
		if (!timer->active)
			continue;
		
		timer->next_run = timer->next_run_time - time;
		if (timer->next_run <= 0)
		{
			timer->next_run_time = time + timer->delay;

			g_Lua->ReferencePush(timer->function);
			g_Lua->PCall(0, 0, 0);

			if (timer->repetitions == 1)
				timer->markdelete = true;
			else
				timer->repetitions--;
		}
	}

	RemoveTimers();
}