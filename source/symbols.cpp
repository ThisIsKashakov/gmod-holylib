#include "detours.h"
namespace Symbols
{
	//---------------------------------------------------------------------------------
	// Purpose: All Base Symbols
	//---------------------------------------------------------------------------------
	const std::vector<Symbol> InitLuaClassesSym = {
		Symbol::FromName("_Z14InitLuaClassesP13ILuaInterface"),
		Symbol::FromSignature("\x48\x8B\x05****\x48\x85\xC0**\x8B\x50\x10\x85\xD2**\x55\x48\x89\xE5\x53\x31\xDB******\x48\x8B*\x48\x8B\x3C\xD8\xE8\xD4"), // 48 8B 05 ?? ?? ?? ?? 48 85 C0 ?? ?? 8B 50 10 85 D2 ?? ?? 55 48 89 E5 53 31 DB ?? ?? ?? ?? ?? ?? 48 8B ?? 48 8B 3C D8 E8 D4
	};

	const std::vector<Symbol> CLuaInterface_ShutdownSym = {
		Symbol::FromName("_ZN13CLuaInterface8ShutdownEv"),
		Symbol::FromSignature("\x55\x48\x89\xE5\x41\x55\x41\x54\x49\x89\xFC\x53\x4D\x8D"), // 55 48 89 E5 41 55 41 54 49 89 FC 53 4D 8D
	};

	const std::vector<Symbol> Get_PlayerSym = {
		Symbol::FromName("_Z10Get_Playerib"),
		Symbol::FromSignature("\x55\x48\x89\xE5\x41\x54\x41\x89\xF4\x53\x40\x0F\xB6\xF6\xE8\xED\xF3"), // 55 48 89 E5 41 54 41 89 F4 53 40 0F B6 F6 E8 ED F3
	};

	const std::vector<Symbol> Push_EntitySym = {
		Symbol::FromName("_Z11Push_EntityP11CBaseEntity"),
		Symbol::FromSignature("\x55\x48\x89\xE5\x41\x54\x53\x48\x83\xEC\x20\x48\x8B\x1D"), // 55 48 89 E5 41 54 53 48 83 EC 20 48 8B 1D
	};

	const std::vector<Symbol> Get_EntitySym = {
		Symbol::FromName("_Z10Get_Entityib"),
		Symbol::FromSignature("\x55\x48\x89\xE5\x41\x55\x41\x89\xF5\x41\x54\x41\x89\xFC\x53"), // 55 48 89 E5 41 55 41 89 F5 41 54 41 89 FC 53
	};

	//---------------------------------------------------------------------------------
	// Purpose: holylib Symbols
	//---------------------------------------------------------------------------------
	const std::vector<Symbol> CServerGameDLL_ShouldHideServerSym = {
		Symbol::FromName("_ZN14CServerGameDLL16ShouldHideServerEv"),
		Symbol::FromSignature("\x48\x8B\x3D\xC1\x9B\x18\x01\x55\x48\x89\xE5"), // 48 8B 3D C1 9B 18 01 55 48 89 E5
	};

	//---------------------------------------------------------------------------------
	// Purpose: gameevent Symbols
	//---------------------------------------------------------------------------------
	const std::vector<Symbol> CBaseClient_ProcessListenEventsSym = {
		Symbol::FromName("_ZN11CBaseClient19ProcessListenEventsEP16CLC_ListenEvents"),
		// FUCK. The symbol for 64x is broken or I got the WRONG ONE. WHY DOES CHLTVClient have the same SHIT and SAME vprof name :(
		Symbol::FromSignature("\x55\x48\x89\xE5\x41\x56\x41\x55\x41\x54\x49\x89\xF4\x53\x48\x8B\x1D****\x8B\x93\x0C\x10\x00\x00\x85\xD2\x41\x0F\x95\xC5******\x41\x0F\xB6\x74\x24\x20\x49"), // 55 48 89 E5 41 56 41 55 41 54 49 89 F4 53 48 8B 1D ?? ?? ?? ?? 8B 93 0C 10 00 00 85 D2 41 0F 95 C5 ?? ?? ?? ?? ?? ?? 41 0F B6 74 24 20 49
	};

	const std::vector<Symbol> CGameEventManager_AddListenerSym = { // Fk this. No 64x
		Symbol::FromName("_ZN17CGameEventManager11AddListenerEPvP20CGameEventDescriptori"),
	};

	//---------------------------------------------------------------------------------
	// Purpose: serverplugin Symbols
	//---------------------------------------------------------------------------------
	const std::vector<Symbol> CPlugin_LoadSym = {
		Symbol::FromName("_ZN7CPlugin4LoadEPKc"),
		Symbol::FromSignature("\x55\x48\x89\xE5\x41\x56\x49\x89\xFE\x41\x55\x41\x54\x49\x89\xF4\xBE\x2E"), // 55 48 89 E5 41 56 49 89 FE 41 55 41 54 49 89 F4 BE 2E
	};

	//---------------------------------------------------------------------------------
	// Purpose: sourcetv Symbols
	//---------------------------------------------------------------------------------
	const std::vector<Symbol> CHLTVClient_ProcessGMod_ClientToServerSym = {
		Symbol::FromName("_ZN11CHLTVClient26ProcessGMod_ClientToServerEP23CLC_GMod_ClientToServer"),
	};

	const std::vector<Symbol> CHLTVServer_CHLTVServerSym = {
		Symbol::FromName("_ZN11CHLTVServerC2Ev"),
	};

	const std::vector<Symbol> CHLTVServer_DestroyCHLTVServerSym = {
		Symbol::FromName("_ZN11CHLTVServerD2Ev"),
	};

	const std::vector<Symbol> COM_IsValidPathSym = {
		Symbol::FromName("_Z15COM_IsValidPathPKc"),
	};

	const std::vector<Symbol> CHLTVDemoRecorder_StartRecordingSym = {
		Symbol::FromName("_ZN17CHLTVDemoRecorder14StartRecordingEPKcb"),
	};

	const std::vector<Symbol> CHLTVDemoRecorder_StopRecordingSym = {
		Symbol::FromName("_ZN17CHLTVDemoRecorder13StopRecordingEv"),
	};

	const std::vector<Symbol> CHLTVClient_ExecuteStringCommandSym = {
		Symbol::FromName("_ZN11CHLTVClient20ExecuteStringCommandEPKc"),
	};

	const std::vector<Symbol> CHLTVClient_DeconstructorSym = {
		Symbol::FromName("_ZN11CHLTVClientD1Ev"),
	};

	const std::vector<Symbol> UsermessagesSym = {
		Symbol::FromName("usermessages"),
	};

	//---------------------------------------------------------------------------------
	// Purpose: threadpoolfix Symbols
	//---------------------------------------------------------------------------------
	const std::vector<Symbol> CThreadPool_ExecuteToPrioritySym = {
		Symbol::FromName("_ZN11CThreadPool17ExecuteToPriorityE13JobPriority_tPFbP4CJobE"),
	};

	//---------------------------------------------------------------------------------
	// Purpose: precachefix Symbols
	//---------------------------------------------------------------------------------
	const std::vector<Symbol> CVEngineServer_PrecacheModelSym = {
		Symbol::FromName("_ZN14CVEngineServer13PrecacheModelEPKcb"),
		Symbol::FromSignature("\x55\x48\x89\xE5\x41\x54\x44\x0F\xB6\xE2\x53\x48\x89\xF3\x44\x89\xE6\x48\x89\xDF"), // 55 48 89 E5 41 54 44 0F B6 E2 53 48 89 F3 44 89 E6 48 89
	};

	const std::vector<Symbol> CVEngineServer_PrecacheGenericSym = {
		Symbol::FromName("_ZN14CVEngineServer15PrecacheGenericEPKcb"),
		Symbol::FromSignature("\x55\x48\x89\xE5\x53\x48\x89\xF3\x48\x83\xEC\x08\x0F\xB6\xF2\x48\x89\xDF"), // 55 48 89 E5 53 48 89 F3 48 83 EC 08 0F B6 F2 48 89 DF
	};

	const std::vector<Symbol> SV_FindOrAddModelSym = {
		Symbol::FromName("_Z17SV_FindOrAddModelPKcb"),
		Symbol::FromSignature("\x55\x40\x80\xFE\x01\x48\x89\xFE\x48\x89\xE5\x48\x8B\x3D****\x19\xD2\x5D\x83\xE2\xFE\x31\xC9\x83\xC2\x03"), // 55 40 80 FE 01 48 89 FE 48 89 E5 48 8B 3D ?? ?? ?? ?? 19 D2 5D 83 E2 FE 31 C9 83 C2 03
	};

	const std::vector<Symbol> SV_FindOrAddGenericSym = {
		Symbol::FromName("_Z19SV_FindOrAddGenericPKcb"),
		Symbol::FromSignature("\x55\x40\x80\xFE\x01\x48\x89\xFE\x48\x89\xE5\x48\x8B\x3D\xC6***\x19\xD2"), // 55 40 80 FE 01 48 89 FE 48 89 E5 48 8B 3D C6 ?? ?? ?? 19 D2
	};

	//---------------------------------------------------------------------------------
	// Purpose: stringtable Symbols
	//---------------------------------------------------------------------------------
	const std::vector<Symbol> CNetworkStringTable_DeleteAllStringsSym = {
		Symbol::FromName("_ZN19CNetworkStringTable16DeleteAllStringsEv"),
		Symbol::FromSignature("\x55\x48\x89\xE5\x53\x48\x89\xFB\x48\x83\xEC\x08\x48\x8B\x7F\x50"), // 55 48 89 E5 53 48 89 FB 48 83 EC 08 48 8B 7F 50
	};

	const std::vector<Symbol> CNetworkStringTable_DeconstructorSym = {
		Symbol::FromName("_ZN19CNetworkStringTableD0Ev"),
		Symbol::FromSignature("\x55\x48\x89\xE5\x53\x48\x89\xFB\x48\x83\xEC\x08\xE8\x8F\xFF\xFF\xFF\x48\x83\xC4\x08\x48\x89\xDF\x5B\x5D\xE9\x71\x96\xEE\xFF"), // 55 48 89 E5 53 48 89 FB 48 83 EC 08 E8 8F FF FF FF 48 83 C4 08 48 89 DF 5B 5D E9 71 96 EE FF
	};

	//---------------------------------------------------------------------------------
	// Purpose: surffix Symbols
	//---------------------------------------------------------------------------------
	const std::vector<Symbol> CGameMovement_TryPlayerMoveSym = {
		Symbol::FromName("_ZN13CGameMovement13TryPlayerMoveEP6VectorP10CGameTrace"),
	};

	const std::vector<Symbol> CGameMovement_ClipVelocitySym = {
		Symbol::FromName("_ZN13CGameMovement12ClipVelocityER6VectorS1_S1_f"),
	};

	const std::vector<Symbol> CBaseEntity_GetGroundEntitySym = {
		Symbol::FromName("_ZN11CBaseEntity15GetGroundEntityEv"),
	};

	const std::vector<Symbol> CTraceFilterSimple_ShouldHitEntitySym = {
		Symbol::FromName("_ZN18CTraceFilterSimple15ShouldHitEntityEP13IHandleEntityi"),
	};

	const std::vector<Symbol> MoveHelperServerSym = {
		Symbol::FromName("_Z16MoveHelperServerv"),
	};

	const std::vector<Symbol> g_pEntityListSym = {
		Symbol::FromName("g_pEntityList"),
	};

	//---------------------------------------------------------------------------------
	// Purpose: pvs Symbols
	//---------------------------------------------------------------------------------
	const std::vector<Symbol> CGMOD_Player_SetupVisibilitySym = {
		Symbol::FromName("_ZN12CGMOD_Player15SetupVisibilityEP11CBaseEntityPhi"),
	};

	const std::vector<Symbol> CServerGameEnts_CheckTransmitSym = {
		Symbol::FromName("_ZN15CServerGameEnts13CheckTransmitEP18CCheckTransmitInfoPKti"),
	};

	//---------------------------------------------------------------------------------
	// Purpose: filesystem Symbols
	//---------------------------------------------------------------------------------
	const std::vector<Symbol> CBaseFileSystem_FindFileInSearchPathSym = {
		Symbol::FromName("_ZN15CBaseFileSystem20FindFileInSearchPathER13CFileOpenInfo"),
		Symbol::FromSignature("\x55\x48\x89\xE5\x41\x57\x49\x89\xFF\x41\x56\x41\x55\x41\x54\x53\x48\x89\xF3"), // 55 48 89 E5 41 57 49 89 FF 41 56 41 55 41 54 53 48 89 F3
	};

	const std::vector<Symbol> CBaseFileSystem_IsDirectorySym = {
		Symbol::FromName("_ZN15CBaseFileSystem11IsDirectoryEPKcS1_"),
		Symbol::FromSignature("\x55\x48\x89\xE5\x41\x57\x41\x56\x41\x55\x4C\x8D\xAD\x38***\x41\x54"), // 55 48 89 E5 41 57 41 56 41 55 4C 8D AD 38 ?? ?? ?? 41 54
	};

	const std::vector<Symbol> CBaseFileSystem_FindSearchPathByStoreIdSym = {
		Symbol::FromName("_ZN15CBaseFileSystem23FindSearchPathByStoreIdEi"),
	};

	const std::vector<Symbol> CBaseFileSystem_FastFileTimeSym = {
		Symbol::FromName("_ZN15CBaseFileSystem12FastFileTimeEPKNS_11CSearchPathEPKc"),
	};

	const std::vector<Symbol> CBaseFileSystem_FixUpPathSym = {
		Symbol::FromName("_ZN15CBaseFileSystem9FixUpPathEPKcPci"),
	};

	std::vector<Symbol> CBaseFileSystem_OpenForReadSym = {
		Symbol::FromName("_ZN15CBaseFileSystem11OpenForReadEPKcS1_jS1_PPc"),
		Symbol::FromSignature("\x55\x48\x89\xE5\x41\x57\x49\x89\xD7\x41\x56\x4D\x89\xC6\x41\x55\x4D\x89\xCD\x41\x54"), // 55 48 89 E5 41 57 49 89 D7 41 56 4D 89 C6 41 55 4D 89 CD 41 54
	};

	const std::vector<Symbol> CBaseFileSystem_GetFileTimeSym = {
		Symbol::FromName("_ZN15CBaseFileSystem11GetFileTimeEPKcS1_"),
		Symbol::FromSignature("\x55\x48\x89\xE5\x41\x57\x41\x56\x49\x89\xFE\x41\x55\x41\x54\x53\x48\x89\xD3"), // 55 48 89 E5 41 57 41 56 49 89 FE 41 55 41 54 53 48 89 D3
	};

	const std::vector<Symbol> CBaseFileSystem_AddSearchPathSym = {
		Symbol::FromName("_ZN15CBaseFileSystem13AddSearchPathEPKcS1_j"),
	};

	const std::vector<Symbol> CBaseFileSystem_AddVPKFileSym = {
		Symbol::FromName("_ZN15CBaseFileSystem10AddVPKFileEPKcS1_j"),
	};

	const std::vector<Symbol> CBaseFileSystem_RemoveAllMapSearchPathsSym = {
		Symbol::FromName("_ZN15CBaseFileSystem23RemoveAllMapSearchPathsEv"),
	};

	const std::vector<Symbol> CBaseFileSystem_CloseSym = {
		Symbol::FromName("_ZN15CBaseFileSystem5CloseEPv"),
	};

	const std::vector<Symbol> CBaseFileSystem_CSearchPath_GetDebugStringSym = {
		Symbol::FromName("_ZNK15CBaseFileSystem11CSearchPath14GetDebugStringEv"),
	};


	//---------------------------------------------------------------------------------
	// Purpose: concommand Symbols
	//---------------------------------------------------------------------------------
	const std::vector<Symbol> ConCommand_IsBlockedSym = {
		Symbol::FromName("_Z20ConCommand_IsBlockedPKc"),
		Symbol::FromSignature("\x48\x8B\x05****\x55\x48\x89\xE5\x41\x54\x49\x89\xFC\x53\x48\x8B*\x80\x78\x70\x00"), // 48 8B 05 ?? ?? ?? ?? 55 48 89 E5 41 54 49 89 FC 53 48 8B ?? 80 78 70 00
	};

	//---------------------------------------------------------------------------------
	// Purpose: vprof Symbols
	//---------------------------------------------------------------------------------
	const std::vector<Symbol> CLuaGamemode_CallFinishSym = {
		Symbol::FromName("_ZN12CLuaGamemode10CallFinishEi"),
		Symbol::FromSignature("\x55\x48\x89\xE5\x41\x56\x41\x55\x41\x54\x41\x89\xF4\x53\x48\x8B\x1D****\x8B\x93\x0C\x10\x00\x00"), // 55 48 89 E5 41 56 41 55 41 54 41 89 F4 53 48 8B 1D ?? ?? ?? ?? 8B 93 0C 10 00 00
	};

	const std::vector<Symbol> CLuaGamemode_CallWithArgsSym = {
		Symbol::FromName("_ZN12CLuaGamemode12CallWithArgsEi"),
		Symbol::FromSignature("\x55\x48\x89\xE5\x41\x57\x49\x89\xF7\x41\x56\x41\x55\x49\x89\xFD"), // 55 48 89 E5 41 57 49 89 F7 41 56 41 55 49 89 FD
	};

	const std::vector<Symbol> CLuaGamemode_CallSym = {
		Symbol::FromName("_ZN12CLuaGamemode4CallEi"),
		Symbol::FromSignature("\x55\x48\x89\xE5\x41\x57\x41\x56\x41\x89\xF6\x41\x55\x41\x54\x49\x89\xFC\x53\x48\x83\xEC\x08"), // 55 48 89 E5 41 57 41 56 41 89 F6 41 55 41 54 49 89 FC 53 48 83 EC 08
	};

	const std::vector<Symbol> CVProfile_OutputReportSym = {
		Symbol::FromName("_ZN9CVProfile12OutputReportEiPKci"),
		Symbol::FromName("_ZN9CVProfile12OutputReportEiPKci"),
	};

	const std::vector<Symbol> CScriptedEntity_StartFunction1Sym = { // const char* version - GetSoundInterests
		Symbol::FromName("_ZN15CScriptedEntity13StartFunctionEPKc"),
		Symbol::FromSignature("\x55\x48\x89\xE5\x41\x56\x49\x89\xF6\x41\x55\x41\x54\x49\x89\xFC\x53\xE8****\x84\xC0"), // 55 48 89 E5 41 56 49 89 F6 41 55 41 54 49 89 FC 53 E8 ?? ?? ?? ?? 84 C0
	};

	const std::vector<Symbol> CScriptedEntity_StartFunction2Sym = { // int version - SENT:AcceptInput
		Symbol::FromName("_ZN15CScriptedEntity13StartFunctionEi"), 
		Symbol::FromSignature("\x55\x48\x89\xE5\x41\x56\x41\x89\xF6\x41\x55\x41\x54\x49\x89\xFC\x53\xE8****"), // 55 48 89 E5 41 56 41 89 F6 41 55 41 54 49 89 FC 53 E8 ?? ?? ?? ??
	};

	const std::vector<Symbol> CScriptedEntity_CallSym = { // SENT:AcceptInput
		Symbol::FromName("_ZN15CScriptedEntity4CallEii"),
		Symbol::FromSignature("\x48\x8B*****\x55\x83\xC6\x01\x85\xD2\x48\x89\xE5"), // 48 8B ?? ?? ?? ?? ?? 55 83 C6 01 85 D2 48 89 E5
	};

	const std::vector<Symbol> CScriptedEntity_CallFunction1Sym = { // const char* version - SetupDataTables
		Symbol::FromName("_ZN15CScriptedEntity12CallFunctionEPKc"),
		Symbol::FromSignature("\x55\x48\x89\xE5\x41\x55\x41\x54\x53\x48\x89\xFB\x48\x83\xEC\x38\x0F\xB6\x47\x08\x84\xC0"), // 55 48 89 E5 41 55 41 54 53 48 89 FB 48 83 EC 38 0F B6 47 08 84 C0
	};

	const std::vector<Symbol> CScriptedEntity_CallFunction2Sym = { // int version. - Found no good identifyer to find it. Guessed it.
		Symbol::FromName("_ZN15CScriptedEntity12CallFunctionEi"),
		Symbol::FromSignature("\x55\x48\x89\xE5\x41\x54\x53\x48\x89\xFB\x48\x83\xEC\x10\x80\x7F\x08\x00"), //  55 48 89 E5 41 54 53 48 89 FB 48 83 EC 10 80 7F 08 00
	};

	//---------------------------------------------------------------------------------
	// Purpose: networking Symbols
	//---------------------------------------------------------------------------------
	const std::vector<Symbol> AllocChangeFrameListSym = {
		Symbol::FromName("_Z20AllocChangeFrameListii"),
		Symbol::FromSignature("\x55\x48\x89\xE5\x41\x55\x41\x54\x41\x89\xFC\xBF\x28"), // 55 48 89 E5 41 55 41 54 41 89 FC BF 28
	};

	//---------------------------------------------------------------------------------
	// Purpose: steamworks Symbols
	//---------------------------------------------------------------------------------
	const std::vector<Symbol> CSteam3Server_OnLoggedOffSym = {
		Symbol::FromName("_ZN13CSteam3Server11OnLoggedOffEP26SteamServersDisconnected_t"),
		Symbol::FromSignature("\x83\xBF\x30****\x0F\x84\x7C***\x55\x48\x89\xE5"), // 83 BF 30 ?? ?? ?? ?? 0F 84 7C ?? ?? ?? 55 48 89 E5
	};

	const std::vector<Symbol> CSteam3Server_OnLogonSuccessSym = {
		Symbol::FromName("_ZN13CSteam3Server14OnLogonSuccessEP23SteamServersConnected_t"),
		Symbol::FromSignature("\x55\x48\x89\xE5\x53\x48\x89\xFB\x48\x83\xEC\x28\x64\x48\x8B"), // 55 48 89 E5 53 48 89 FB 48 83 EC 28 64 48 8B
	};

	const std::vector<Symbol> CSteam3Server_ShutdownSym = {
		Symbol::FromName("_ZN13CSteam3Server8ShutdownEv"),
		Symbol::FromSignature("\x55\x48\x89\xE5\x53\x48\x89\xFB\x48\x83\xEC\x08\x48\x83\x7F**\x74\x5A"), // 55 48 89 E5 53 48 89 FB 48 83 EC 08 48 83 7F ?? ?? 74 5A
	};

	const std::vector<Symbol> CSteam3Server_ActivateSym = {
		Symbol::FromName("_ZN13CSteam3Server8ActivateENS_11EServerTypeE"),
		Symbol::FromSignature("\x55\x48\x89\xE5\x41\x57\x41\x56\x41\x55\x41\x54\x41\x89\xF4\x53\x48\x89\xFB\x48\x81\xEC") // 55 48 89 E5 41 57 41 56 41 55 41 54 41 89 F4 53 48 89 FB 48 81 EC
	};

	const std::vector<Symbol> Steam3ServerSym = {
		Symbol::FromName("_Z12Steam3Serverv"),
		Symbol::FromSignature("\x55\x48\x8D\x05\x18\x8B\x4C\x00\x48\x89\xE5\x5D\xC3"), // 55 48 8D 05 18 8B 4C 00 48 89 E5 5D C3
	};

	const std::vector<Symbol> SV_InitGameServerSteamSym = {
		Symbol::FromName("_Z22SV_InitGameServerSteamv"),
		Symbol::FromSignature("\x55\x48\x89\xE5\x53\x48\x83\xEC\x08\x48\x8B\x1D********\x00\x00\x01"), // 55 48 89 E5 53 48 83 EC 08 48 8B 1D ?? ?? ?? ?? ?? ?? ?? ?? 00 00 01
	};

	//---------------------------------------------------------------------------------
	// Purpose: pas Symbols
	//---------------------------------------------------------------------------------
	const std::vector<Symbol> g_BSPDataSym = {
		Symbol::FromName("g_BSPData"),
		Symbol::FromSignature("\x48\x8B\x1D\x2A\x2A\x2A\x2A\x8B\x83\x78"), // 48 8B 1D 2A 2A 2A 2A 8B 83 78 | 2A -> ??
	};
}