#include "public/imodule.h"
#include <vector>

class CModuleManager;
class CModule : public IModuleWrapper
{
public:
	~CModule();
	virtual void SetModule(IModule* module);
	virtual void SetEnabled(bool bEnabled, bool bForced = false);
	virtual void Shutdown();
	virtual bool IsEnabled();

public:
	inline IModule* GetModule() { return m_pModule; };
	inline bool FastIsEnabled() { return m_bEnabled; };
	inline ConVar* GetConVar() { return m_pCVar; };
	inline ConVar* GetDebugConVar() { return m_pDebugCVar; };
	inline bool IsCompatible() { return m_bCompatible; };

protected:
	friend class CModuleManager;
	inline void SetID(unsigned int pID) { m_pModule->m_pID = pID; };

protected:
	IModule* m_pModule = NULL;
	ConVar* m_pCVar = NULL;
	char* m_pCVarName = NULL;
	ConVar* m_pDebugCVar = NULL;
	char* m_pDebugCVarName = NULL;
	bool m_bEnabled = false;
	bool m_bCompatible = false;
	bool m_bStartup = false;
	char* m_strDebugValue; // Workaround for a crash.
};

class CModuleManager : public IModuleManager
{
public:
	CModuleManager();
	virtual void LoadModules();
	virtual IModuleWrapper* RegisterModule(IModule* mdl);
	virtual IModuleWrapper* FindModuleByConVar(ConVar* convar);
	virtual IModuleWrapper* FindModuleByName(const char* name);

	virtual void SetGhostInj() { m_bGhostInj = true; };
	virtual bool IsUsingGhostInj() { return m_bGhostInj; };

	virtual void Setup(CreateInterfaceFn appfn, CreateInterfaceFn gamefn);
	virtual void Init();
	virtual void LuaInit(bool bServerInit);
	virtual void LuaShutdown();
	virtual void InitDetour(bool bPreServer);
	virtual void Think(bool bSimulating);
	virtual void Shutdown();
	virtual void ServerActivate(edict_t* pEdictList, int edictCount, int clientMax);
	virtual void OnEdictAllocated(edict_t* pEdict);
	virtual void OnEdictFreed(const edict_t* pEdict);

	inline int GetStatus() { return m_pStatus; };
	inline CreateInterfaceFn& GetAppFactory() { return m_pAppFactory; };
	inline CreateInterfaceFn& GetGameFactory() { return m_pGameFactory; };
	inline edict_t* GetEdictList() { return m_pEdictList; };
	inline int GetEdictCount() { return m_iEdictCount; };
	inline int GetClientMax() { return m_iClientMax; };
	inline std::vector<CModule*>& GetModules() { return m_pModules; };

private:
	std::vector<CModule*> m_pModules;
	int m_pStatus = 0;
	CreateInterfaceFn m_pAppFactory = NULL;
	CreateInterfaceFn m_pGameFactory = NULL;
	bool m_bGhostInj = false;

private: // ServerActivate stuff
	edict_t* m_pEdictList = NULL;
	int m_iEdictCount = 0;
	int m_iClientMax = 0;
};
extern CModuleManager g_pModuleManager;