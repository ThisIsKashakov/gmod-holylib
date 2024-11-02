#include "filesystem_base.h" // Has to be before symbols.h
#include "LuaInterface.h"
#include "symbols.h"
#include "detours.h"
#include "module.h"
#include "lua.h"
#include "player.h"

class CPASModule : public IModule
{
public:
	virtual void LuaInit(bool bServerInit) OVERRIDE;
	virtual void LuaShutdown() OVERRIDE;
	virtual const char* Name() { return "pas"; };
	virtual int Compatibility() { return LINUX32 | LINUX64; };
};

CPASModule g_pPASModule;
IModule* pPASModule = &g_pPASModule;

inline bool TestPAS(const Vector& hearPos)
{
	return Util::engineserver->CheckOriginInPVS(hearPos, Util::g_pCurrentCluster, sizeof(Util::g_pCurrentCluster));
}

LUA_FUNCTION_STATIC(pas_TestPAS)
{
	Vector* orig;
	LUA->CheckType(1, GarrysMod::Lua::Type::Vector);
	if (LUA->IsType(1, GarrysMod::Lua::Type::Vector))
	{
		orig = Get_Vector(1);
	} else {
		CBaseEntity* ent = Util::Get_Entity(1, false);

		orig = (Vector*)&ent->GetAbsOrigin(); // ToDo: This currently breaks the compile.
	}

	Util::CM_Vis(*orig, DVIS_PAS);

	LUA->CheckType(2, GarrysMod::Lua::Type::Vector);
	if (LUA->IsType(2, GarrysMod::Lua::Type::Vector))
	{
		LUA->PushBool(TestPAS(*Get_Vector(2)));
	} else if (Is_EntityList(2)) {
		LUA->CreateTable();
		EntityList* entList = Get_EntityList(2, true);
		for (CBaseEntity*& ent : entList->pEntities)
		{
			Util::Push_Entity(ent);
			LUA->PushBool(TestPAS(ent->GetAbsOrigin()));
			LUA->RawSet(-3);
		}
	} else {
		LUA->CheckType(2, GarrysMod::Lua::Type::Entity);
		CBaseEntity* ent = Util::Get_Entity(2, false);

		LUA->PushBool(TestPAS(ent->GetAbsOrigin()));
	}

	return 1;
}

LUA_FUNCTION_STATIC(pas_CheckBoxInPAS)
{
	Vector* mins = Get_Vector(1, true);
	Vector* maxs = Get_Vector(2, true);
	Vector* orig = Get_Vector(3, true);

	Util::CM_Vis(*orig, DVIS_PAS);

	LUA->PushBool(Util::engineserver->CheckBoxInPVS(*mins, *maxs, Util::g_pCurrentCluster, sizeof(Util::g_pCurrentCluster)));
	return 1;
}

LUA_FUNCTION_STATIC(pas_FindInPAS)
{
	VPROF_BUDGET("pas.FindInPAS", VPROF_BUDGETGROUP_HOLYLIB);

	Vector* orig;
	LUA->CheckType(1, GarrysMod::Lua::Type::Vector);
	if (LUA->IsType(1, GarrysMod::Lua::Type::Vector))
	{
		orig = Get_Vector(1);
	}
	else {
		LUA->CheckType(1, GarrysMod::Lua::Type::Entity);
		CBaseEntity* ent = Util::Get_Entity(1, false);

		orig = (Vector*)&ent->GetAbsOrigin(); // ToDo: This currently breaks the compile.
	}

	Util::CM_Vis(*orig, DVIS_PAS);

	LUA->CreateTable();
	int idx = 0;
	if (Util::pEntityList->IsEnabled())
	{
		for (CBaseEntity*& pEnt : g_pGlobalEntityList.pEntities)
		{
			if (Util::engineserver->CheckOriginInPVS(pEnt->GetAbsOrigin(), Util::g_pCurrentCluster, sizeof(Util::g_pCurrentCluster)))
			{
				++idx;
				LUA->PushNumber(idx);
				// Since it should be a bit more rare that ALL entities are pushed we don't directly loop thru the map itself to benefit from the vector's performance.
				Util::Push_Entity(pEnt);
				LUA->RawSet(-3);
			}
		}
		return 1;
	}

#if ARCHITECTURE_IS_X86
	CBaseEntity* pEnt = Util::entitylist->FirstEnt();
	while (pEnt != NULL)
	{
		if (Util::engineserver->CheckOriginInPVS(pEnt->GetAbsOrigin(), Util::g_pCurrentCluster, sizeof(Util::g_pCurrentCluster)))
		{
			++idx;
			LUA->PushNumber(idx);
			Util::Push_Entity(pEnt);
			LUA->RawSet(-3);
		}

		pEnt = Util::entitylist->NextEnt(pEnt);
	}
#else
	edict_t* pWorldEdict = Util::engineserver->PEntityOfEntIndex(0);
	for(int i=0; i<MAX_EDICTS; ++i)
	{
		edict_t* pEdict = &pWorldEdict[i]; // Let's hope this works.... It's used in CServerGameEnts::CheckTransmit as an optimization.
		CBaseEntity* pEnt = Util::GetCBaseEntityFromEdict(pEdict);
		if (!pEnt)
			continue;

		if (Util::engineserver->CheckOriginInPVS(pEnt->GetAbsOrigin(), Util::g_pCurrentCluster, sizeof(Util::g_pCurrentCluster)))
		{
			++idx;
			LUA->PushNumber(idx);
			Util::Push_Entity(pEnt);
			LUA->RawSet(-3);
		}
	}
#endif

	return 1;
}

void CPASModule::LuaInit(bool bServerInit)
{
	if (bServerInit)
		return;

	Util::StartTable();
		Util::AddFunc(pas_TestPAS, "TestPAS");
		Util::AddFunc(pas_CheckBoxInPAS, "CheckBoxInPAS");
		Util::AddFunc(pas_FindInPAS, "FindInPAS");
	Util::FinishTable("pas");
}

void CPASModule::LuaShutdown()
{
	Util::NukeTable("pas");
}