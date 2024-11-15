#include "util.h"
#include "symbols.h"
#include <string>
#include "GarrysMod/InterfacePointers.hpp"
#include "sourcesdk/baseclient.h"
#include "iserver.h"
#include "module.h"
#include "icommandline.h"
#include "player.h"
#include "detours.h"

GarrysMod::Lua::ILuaInterface* g_Lua;
IVEngineServer* engine;
CGlobalEntityList* Util::entitylist = NULL;
CUserMessages* Util::pUserMessages;

static Symbols::Get_Player func_GetPlayer;
static Symbols::Push_Entity func_PushEntity;
static Symbols::Get_Entity func_GetEntity;
CBasePlayer* Util::Get_Player(int iStackPos, bool bError) // bError = error if not a valid player
{
	if (func_GetPlayer)
	{
		CBasePlayer* ply = func_GetPlayer(iStackPos, bError);
		if (!ply && bError)
			g_Lua->ThrowError("Tried to use a NULL Entity!");
		
		return ply;
	}

	return NULL;
}

IModuleWrapper* Util::pEntityList;
void Util::Push_Entity(CBaseEntity* pEnt)
{
	if (Util::pEntityList->IsEnabled()) // Push_Entity is quiet slow since it has so much overhead.
	{
		auto it = g_pGlobalEntityList.pEntReferences.find(pEnt);
		if (it != g_pGlobalEntityList.pEntReferences.end()) // It should never happen that we don't have a reference... but just in case.
		{
			g_Lua->ReferencePush(it->second);
			return;
		}
	}

	if (func_PushEntity)
		func_PushEntity(pEnt);
}

CBaseEntity* Util::Get_Entity(int iStackPos, bool bError)
{
	if (func_GetEntity)
	{
		CBaseEntity* ent = func_GetEntity(iStackPos, bError);
		if (!ent && bError)
			g_Lua->ThrowError("Tried to use a NULL Entity!");
		
		return ent;
	}

	return NULL;
}

IServer* Util::server;
CBaseClient* Util::GetClientByUserID(int userid)
{
	for (int i = 0; i < Util::server->GetClientCount(); i++)
	{
		IClient* pClient = Util::server->GetClient(i);
		if ( pClient && pClient->GetUserID() == userid)
		{
			return (CBaseClient*)pClient;
		}
	}

	return NULL;
}

IVEngineServer* Util::engineserver = NULL;
IServerGameEnts* Util::servergameents = NULL;
IServerGameClients* Util::servergameclients = NULL;
CBaseClient* Util::GetClientByPlayer(const CBasePlayer* ply)
{
	return Util::GetClientByUserID(Util::engineserver->GetPlayerUserId(((CBaseEntity*)ply)->edict()));
}

CBaseClient* Util::GetClientByIndex(int index)
{
	if (server->GetClientCount() <= index || index < 0)
		return NULL;

	return (CBaseClient*)server->GetClient(index);
}

std::vector<CBaseClient*> Util::GetClients()
{
	std::vector<CBaseClient*> pClients;

	for (int i = 0; i < server->GetClientCount(); i++)
	{
		IClient* pClient = server->GetClient(i);
		pClients.push_back((CBaseClient*)pClient);
	}

	return pClients;
}

CBasePlayer* Util::GetPlayerByClient(CBaseClient* client)
{
	return (CBasePlayer*)servergameents->EdictToBaseEntity(engineserver->PEntityOfEntIndex(client->GetPlayerSlot() + 1));
}

byte Util::g_pCurrentCluster[MAX_MAP_LEAFS / 8];
void Util::ResetClusers()
{
	Q_memset(Util::g_pCurrentCluster, 0, sizeof(Util::g_pCurrentCluster));
}

Symbols::CM_Vis func_CM_Vis = NULL;
void Util::CM_Vis(const Vector& orig, int type)
{
	Util::ResetClusers();

	if (func_CM_Vis)
		func_CM_Vis(Util::g_pCurrentCluster, sizeof(Util::g_pCurrentCluster), engine->GetClusterForOrigin(orig), type);
}

bool Util::CM_Vis(byte* cluster, int clusterSize, int clusterID, int type)
{
	if (func_CM_Vis)
		func_CM_Vis(cluster, clusterSize, clusterID, type);
	else
		return false;

	return true;
}


static Symbols::CBaseEntity_CalcAbsolutePosition func_CBaseEntity_CalcAbsolutePosition;
void CBaseEntity::CalcAbsolutePosition(void)
{
	func_CBaseEntity_CalcAbsolutePosition(this);
}

CBaseEntity* Util::GetCBaseEntityFromEdict(edict_t* edict)
{
	return Util::servergameents->EdictToBaseEntity(edict);
}

IGet* Util::get;
CBaseEntityList* g_pEntityList = NULL;
IGameEventManager2* Util::gameeventmanager;
void Util::AddDetour()
{
	if (g_pModuleManager.GetAppFactory())
		engineserver = (IVEngineServer*)g_pModuleManager.GetAppFactory()(INTERFACEVERSION_VENGINESERVER, NULL);
	else
		engineserver = InterfacePointers::VEngineServer();
	Detour::CheckValue("get interface", "IVEngineServer", engineserver != NULL);

	gameeventmanager = (IGameEventManager2*)g_pModuleManager.GetAppFactory()(INTERFACEVERSION_GAMEEVENTSMANAGER2, NULL);
	Detour::CheckValue("get interface", "IGameEventManager", gameeventmanager != NULL);

	SourceSDK::FactoryLoader server_loader("server");
	pUserMessages = Detour::ResolveSymbol<CUserMessages>(server_loader, Symbols::UsermessagesSym);
	Detour::CheckValue("get class", "usermessages", pUserMessages != NULL);

	if (g_pModuleManager.GetAppFactory())
		servergameents = (IServerGameEnts*)g_pModuleManager.GetGameFactory()(INTERFACEVERSION_SERVERGAMEENTS, NULL);
	else
		servergameents = server_loader.GetInterface<IServerGameEnts>(INTERFACEVERSION_SERVERGAMEENTS);
	Detour::CheckValue("get interface", "IServerGameEnts", servergameents != NULL);

	if (g_pModuleManager.GetAppFactory())
		servergameclients = (IServerGameClients*)g_pModuleManager.GetGameFactory()(INTERFACEVERSION_SERVERGAMECLIENTS, NULL);
	else
		servergameclients = server_loader.GetInterface<IServerGameClients>(INTERFACEVERSION_SERVERGAMECLIENTS);
	Detour::CheckValue("get interface", "IServerGameClients", servergameclients != NULL);

	server = InterfacePointers::Server();
	Detour::CheckValue("get class", "IServer", server != NULL);

#ifdef ARCHITECTURE_X86 // We don't use it on 64x, do we. Look into pas_FindInPAS to see how we do it ^^
	g_pEntityList = Detour::ResolveSymbol<CBaseEntityList>(server_loader, Symbols::g_pEntityListSym);
	Detour::CheckValue("get class", "g_pEntityList", g_pEntityList != NULL);
	entitylist = (CGlobalEntityList*)g_pEntityList;

	get = Detour::ResolveSymbol<IGet>(server_loader, Symbols::CGetSym);
	Detour::CheckValue("get class", "IGet", get != NULL);
#endif

	SourceSDK::FactoryLoader engine_loader("engine");
	func_CM_Vis = (Symbols::CM_Vis)Detour::GetFunction(engine_loader.GetModule(), Symbols::CM_VisSym);
	Detour::CheckFunction((void*)func_CM_Vis, "CM_Vis");

	pEntityList = g_pModuleManager.FindModuleByName("entitylist");

	/*
	 * IMPORTANT TODO
	 * 
	 * We now will run in the menu state so if we try to push an entity or so, we may push it in the wrong realm!
	 * How will we handle multiple realms?
	 * 
	 * Idea: Fk menu, if there is a server realm, we'll use it. If not, we wait for one to start.
	 *		We also could introduce a Lua Flag so that modules can register for Menu/Client realm if wanted.
	 *		But I won't really support client. At best only menu.
	 */

#ifndef SYSTEM_WINDOWS
	func_GetPlayer = (Symbols::Get_Player)Detour::GetFunction(server_loader.GetModule(), Symbols::Get_PlayerSym);
	func_PushEntity = (Symbols::Push_Entity)Detour::GetFunction(server_loader.GetModule(), Symbols::Push_EntitySym);
	func_GetEntity = (Symbols::Get_Entity)Detour::GetFunction(server_loader.GetModule(), Symbols::Get_EntitySym);
	func_CBaseEntity_CalcAbsolutePosition = (Symbols::CBaseEntity_CalcAbsolutePosition)Detour::GetFunction(server_loader.GetModule(), Symbols::CBaseEntity_CalcAbsolutePositionSym);
	Detour::CheckFunction((void*)func_CBaseEntity_CalcAbsolutePosition, "CBaseEntity::CalcAbsolutePosition");
	Detour::CheckFunction((void*)func_GetPlayer, "Get_Player");
	Detour::CheckFunction((void*)func_PushEntity, "Push_Entity");
	Detour::CheckFunction((void*)func_GetEntity, "Get_Entity");
#endif
}

static bool g_pShouldLoad = false;
bool Util::ShouldLoad()
{
	if (CommandLine()->FindParm("-holylibexists") && !g_pShouldLoad) // Don't set this manually!
		return false;

	if (g_pShouldLoad)
		return true;

	g_pShouldLoad = true;
	CommandLine()->AppendParm("-holylibexists", "true");

	return true;
}

Get_LuaClass(IRecipientFilter, GarrysMod::Lua::Type::RecipientFilter, "RecipientFilter")
Get_LuaClass(Vector, GarrysMod::Lua::Type::Vector, "Vector")
Get_LuaClass(QAngle, GarrysMod::Lua::Type::Angle, "Angle")
Get_LuaClass(ConVar, GarrysMod::Lua::Type::ConVar, "ConVar")