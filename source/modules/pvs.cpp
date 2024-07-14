#include "module.h"
#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/InterfacePointers.hpp>
#include "detours.h"
#include "util.h"
#include "lua.h"
#include "eiface.h"
#include "unordered_map"
#include "player.h"

class CPVSModule : public IModule
{
public:
	virtual void Init(CreateInterfaceFn* appfn, CreateInterfaceFn* gamefn);
	virtual void LuaInit(bool bServerInit);
	virtual void LuaShutdown();
	virtual void InitDetour(bool bPreServer);
	virtual void Think(bool simulating);
	virtual void Shutdown();
	virtual const char* Name() { return "pvs"; };
};

CPVSModule g_pPVSModule;
IModule* pPVSModule = &g_pPVSModule;

static int currentPVSSize = -1;
static unsigned char* currentPVS = NULL;
static int mapPVSSize = -1;
static IVEngineServer* engineserver = NULL;
static Detouring::Hook detour_CGMOD_Player_SetupVisibility;
static void hook_CGMOD_Player_SetupVisibility(void* ent, unsigned char* pvs, int pvssize)
{
	currentPVS = pvs;
	currentPVSSize = pvssize;

	detour_CGMOD_Player_SetupVisibility.GetTrampoline<Symbols::CGMOD_Player_SetupVisibility>()(ent, pvs, pvssize);

	currentPVS = NULL;
	currentPVSSize = -1;
}

static IServerGameEnts* servergameents = NULL;
static std::vector<edict_t*> g_pAddEntityToPVS;
static std::unordered_map<edict_t*, int> g_pOverrideStateFlag;
static Detouring::Hook detour_CServerGameEnts_CheckTransmit;
static void hook_CServerGameEnts_CheckTransmit(void* gameents, CCheckTransmitInfo *pInfo, const unsigned short *pEdictIndices, int nEdicts)
{
	for (edict_t* ent : g_pAddEntityToPVS)
	{
		servergameents->EdictToBaseEntity(ent)->SetTransmit(pInfo, true);
	}
	
	static std::unordered_map<edict_t*, int> pOriginalFlags;
	for (auto&[ent, flag] : g_pOverrideStateFlag)
	{
		pOriginalFlags[ent] = ent->m_fStateFlags;
		Msg("Overriding ent(%i) flags for snapshot (%i -> %i)\n", ent->m_EdictIndex, ent->m_fStateFlags, flag);
		ent->m_fStateFlags = flag;
	}

	detour_CServerGameEnts_CheckTransmit.GetTrampoline<Symbols::CServerGameEnts_CheckTransmit>()(gameents, pInfo, pEdictIndices, nEdicts);

	for (auto&[ent, flag] : pOriginalFlags)
	{
		ent->m_fStateFlags = flag;
	}
	pOriginalFlags.clear();
	g_pAddEntityToPVS.clear();
	g_pOverrideStateFlag.clear();
}

Vector* Get_Vector(int iStackPos)
{
	return g_Lua->GetUserType<Vector>(iStackPos, GarrysMod::Lua::Type::Vector);
}

Symbols::Get_Entity func_Get_Entity;
CBaseEntity* Get_Entity(int iStackPos)
{
	return func_Get_Entity(iStackPos, false);
}

edict_t* GetEdictOfEnt(CBaseEntity* ent)
{
	return servergameents->BaseEntityToEdict(ent);
}

LUA_FUNCTION_STATIC(pvs_ResetPVS)
{
	if (!currentPVS)
		LUA->ThrowError("pvs: tried to call pvs.ResetPVS with no active PVS!");

	engineserver->ResetPVS(currentPVS, currentPVSSize);

	return 0;
}

LUA_FUNCTION_STATIC(pvs_CheckOriginInPVS)
{
	Vector* vec = Get_Vector(1);

	if (!currentPVS)
		LUA->ThrowError("pvs: tried to call pvs.CheckOriginInPVS with no active PVS!");

	LUA->PushBool(engineserver->CheckOriginInPVS(*vec, currentPVS, currentPVSSize));

	return 1;
}

LUA_FUNCTION_STATIC(pvs_AddOriginToPVS)
{
	Vector* vec = Get_Vector(1);

	if (!currentPVS)
		LUA->ThrowError("pvs: tried to call pvs.AddOriginToPVS with no active PVS!");

	engineserver->AddOriginToPVS(*vec);

	return 0;
}

LUA_FUNCTION_STATIC(pvs_GetClusterCount)
{
	LUA->PushNumber(engineserver->GetClusterCount());

	return 1;
}

LUA_FUNCTION_STATIC(pvs_GetClusterForOrigin)
{
	Vector* vec = Get_Vector(1);

	LUA->PushNumber(engineserver->GetClusterForOrigin(*vec));

	return 1;
}

LUA_FUNCTION_STATIC(pvs_CheckAreasConnected)
{
	int area1 = LUA->CheckNumber(1);
	int area2 = LUA->CheckNumber(2);

	LUA->PushBool(engineserver->CheckAreasConnected(area1, area2));

	return 1;
}

LUA_FUNCTION_STATIC(pvs_GetArea)
{
	Vector* vec = Get_Vector(1);

	LUA->PushNumber(engineserver->GetArea(*vec));

	return 1;
}

LUA_FUNCTION_STATIC(pvs_GetPVSForCluster)
{
	int cluster = LUA->CheckNumber(1);

	if (!currentPVS)
		LUA->ThrowError("pvs: tried to call pvs.GetPVSForCluster with no active PVS!");

	engineserver->ResetPVS(currentPVS, currentPVSSize);
	engineserver->GetPVSForCluster(cluster, currentPVSSize, currentPVS);

	return 0;
}

LUA_FUNCTION_STATIC(pvs_CheckBoxInPVS)
{
	Vector* vec1 = Get_Vector(1);
	Vector* vec2 = Get_Vector(2);

	LUA->PushBool(engine->CheckBoxInPVS(*vec1, *vec2, currentPVS, currentPVSSize));

	return 1;
}

LUA_FUNCTION_STATIC(pvs_AddEntityToPVS)
{
	CBaseEntity* ent = Get_Entity(1);
	if (!ent)
		LUA->ThrowError("Tried to use a NULL Entity!");

	edict_t* edict = GetEdictOfEnt(ent);
	if (edict)
		g_pAddEntityToPVS.push_back(edict);
	else
		LUA->ThrowError("Failed to get edict?");

	return 0;
}

#define LUA_FL_EDICT_DONTSEND 1 << 1
#define LUA_FL_EDICT_ALWAYS 1 << 2
#define LUA_FL_EDICT_PVSCHECK 1 << 3
#define LUA_FL_EDICT_FULLCHECK 1 << 4
LUA_FUNCTION_STATIC(pvs_OverrideStateFlags)
{
	CBaseEntity* ent = Get_Entity(1);
	if (!ent)
		LUA->ThrowError("Tried to use a NULL Entity!");

	edict_t* edict = GetEdictOfEnt(ent);
	if (!edict)
		LUA->ThrowError("Failed to get edict?");

	int flags = LUA->CheckNumber(2);
	bool force = LUA->GetBool(3);
	int newFlags = flags;
	if (!force)
	{
		newFlags = edict->m_fStateFlags;
		newFlags = newFlags & ~FL_EDICT_DONTSEND;
		newFlags = newFlags & ~FL_EDICT_ALWAYS;
		newFlags = newFlags & ~FL_EDICT_PVSCHECK;
		newFlags = newFlags & ~FL_EDICT_FULLCHECK;

		if (flags & LUA_FL_EDICT_DONTSEND)
			newFlags |= FL_EDICT_DONTSEND;

		if (flags & LUA_FL_EDICT_ALWAYS)
			newFlags |= FL_EDICT_ALWAYS;

		if (flags & LUA_FL_EDICT_PVSCHECK)
			newFlags |= FL_EDICT_PVSCHECK;

		if (flags & LUA_FL_EDICT_FULLCHECK)
			newFlags |= FL_EDICT_FULLCHECK;
	}

	g_pOverrideStateFlag[edict] = newFlags;

	return 0;
}

LUA_FUNCTION_STATIC(pvs_SetStateFlags)
{
	CBaseEntity* ent = Get_Entity(1);
	if (!ent)
		LUA->ThrowError("Tried to use a NULL Entity!");

	edict_t* edict = GetEdictOfEnt(ent);
	if (!edict)
		LUA->ThrowError("Failed to get edict?");

	int flags = LUA->CheckNumber(2);
	bool force = LUA->GetBool(3);
	int newFlags = flags;
	if (!force)
	{
		newFlags = edict->m_fStateFlags;
		newFlags = newFlags & ~FL_EDICT_DONTSEND;
		newFlags = newFlags & ~FL_EDICT_ALWAYS;
		newFlags = newFlags & ~FL_EDICT_PVSCHECK;
		newFlags = newFlags & ~FL_EDICT_FULLCHECK;

		if (flags & LUA_FL_EDICT_DONTSEND)
			newFlags |= FL_EDICT_DONTSEND;

		if (flags & LUA_FL_EDICT_ALWAYS)
			newFlags |= FL_EDICT_ALWAYS;

		if (flags & LUA_FL_EDICT_PVSCHECK)
			newFlags |= FL_EDICT_PVSCHECK;

		if (flags & LUA_FL_EDICT_FULLCHECK)
			newFlags |= FL_EDICT_FULLCHECK;
	}

	edict->m_fStateFlags = newFlags;

	return 0;
}

LUA_FUNCTION_STATIC(pvs_GetStateFlags)
{
	CBaseEntity* ent = Get_Entity(1);
	if (!ent)
		LUA->ThrowError("Tried to use a NULL Entity!");

	edict_t* edict = GetEdictOfEnt(ent);
	if (!edict)
		LUA->ThrowError("Failed to get edict?");

	int flags = edict->m_fStateFlags;
	int newFlags = flags;
	bool force = LUA->GetBool(3);
	if (!force)
	{
		newFlags = 0;
		if (flags & FL_EDICT_DONTSEND)
			newFlags |= LUA_FL_EDICT_DONTSEND;

		if (flags & FL_EDICT_ALWAYS)
			newFlags |= LUA_FL_EDICT_ALWAYS;

		if (flags & FL_EDICT_PVSCHECK)
			newFlags |= LUA_FL_EDICT_PVSCHECK;

		if (flags & FL_EDICT_FULLCHECK)
			newFlags |= LUA_FL_EDICT_FULLCHECK;
	}

	LUA->PushNumber(newFlags);

	return 1;
}

void CPVSModule::Init(CreateInterfaceFn* appfn, CreateInterfaceFn* gamefn)
{
	engineserver = (IVEngineServer*)appfn[0](INTERFACEVERSION_VENGINESERVER, NULL);
	Detour::CheckValue("get interface", "IVEngineServer", engineserver != NULL);

	servergameents = (IServerGameEnts*)gamefn[0](INTERFACEVERSION_SERVERGAMEENTS, NULL);
	Detour::CheckValue("get interface", "IServerGameEnts", servergameents != NULL);
}

void CPVSModule::LuaInit(bool bServerInit)
{
	if ( bServerInit ) { return; }

	mapPVSSize = ceil(engineserver->GetClusterCount() / 8.0f);

	Util::StartTable();
		Util::AddFunc(pvs_ResetPVS, "ResetPVS");
		Util::AddFunc(pvs_CheckOriginInPVS, "CheckOriginInPVS");
		Util::AddFunc(pvs_AddOriginToPVS, "AddOriginToPVS");
		Util::AddFunc(pvs_GetClusterCount, "GetClusterCount");
		Util::AddFunc(pvs_GetClusterForOrigin, "GetClusterForOrigin");
		Util::AddFunc(pvs_CheckAreasConnected, "CheckAreasConnected");
		Util::AddFunc(pvs_GetArea, "GetArea");
		Util::AddFunc(pvs_GetPVSForCluster, "GetPVSForCluster");
		Util::AddFunc(pvs_CheckBoxInPVS, "CheckBoxInPVS");
		Util::AddFunc(pvs_AddEntityToPVS, "AddEntityToPVS");
		Util::AddFunc(pvs_OverrideStateFlags, "OverrideStateFlags");
		Util::AddFunc(pvs_SetStateFlags, "SetStateFlags");
		Util::AddFunc(pvs_GetStateFlags, "GetStateFlags");

		g_Lua->PushNumber(LUA_FL_EDICT_DONTSEND);
		g_Lua->SetField(-2, "FL_EDICT_DONTSEND");

		g_Lua->PushNumber(LUA_FL_EDICT_ALWAYS);
		g_Lua->SetField(-2, "FL_EDICT_ALWAYS");

		g_Lua->PushNumber(LUA_FL_EDICT_PVSCHECK);
		g_Lua->SetField(-2, "FL_EDICT_PVSCHECK");

		g_Lua->PushNumber(LUA_FL_EDICT_FULLCHECK);
		g_Lua->SetField(-2, "FL_EDICT_FULLCHECK");
	Util::FinishTable("pvs");
}

void CPVSModule::LuaShutdown()
{
	g_Lua->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		g_Lua->PushNil();
		g_Lua->SetField(-2, "pvs");
	g_Lua->Pop(1);
}

void CPVSModule::InitDetour(bool bPreServer)
{
	if ( bPreServer ) { return; }

	SourceSDK::ModuleLoader server_loader("server_srv");
	Detour::Create(
		&detour_CGMOD_Player_SetupVisibility, "CGMOD_Player::SetupVisibility",
		server_loader.GetModule(), Symbols::CGMOD_Player_SetupVisibilitySym,
		(void*)hook_CGMOD_Player_SetupVisibility, m_pID
	);

	Detour::Create(
		&detour_CServerGameEnts_CheckTransmit, "CServerGameEnts::CheckTransmit",
		server_loader.GetModule(), Symbols::CServerGameEnts_CheckTransmitSym,
		(void*)hook_CServerGameEnts_CheckTransmit, m_pID
	);

	func_Get_Entity = (Symbols::Get_Entity)Detour::GetFunction(server_loader.GetModule(), Symbols::Get_EntitySym);
	Detour::CheckFunction(func_Get_Entity, "Get_Entity");
}

void CPVSModule::Think(bool bSimulating)
{
}

void CPVSModule::Shutdown()
{
	Detour::Remove(m_pID);
}