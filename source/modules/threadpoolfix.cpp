#include "module.h"
#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/InterfacePointers.hpp>
#include "detours.h"
#include "util.h"
#include "lua.h"
#define DEDICATED
#include "vstdlib/jobthread.h"

class CThreadPoolFixModule : public IModule
{
public:
	virtual void Init(CreateInterfaceFn* appfn, CreateInterfaceFn* gamefn) OVERRIDE;
	virtual void InitDetour(bool bPreServer) OVERRIDE;
	virtual const char* Name() { return "threadpoolfix"; };
	virtual int Compatibility() { return LINUX32 | LINUX64; }; 
	// NOTE: This is only an issue specific to Linux 32x DS so this doesn't need to support anything else.
	// NOTE2: 64x has a different issue: https://github.com/Facepunch/garrysmod-issues/issues/5310
};

static CThreadPoolFixModule g_pThreadPoolFixModule;
IModule* pThreadPoolFixModule = &g_pThreadPoolFixModule;

#if ARCHITECTURE_IS_X86
static Detouring::Hook detour_CThreadPool_ExecuteToPriority;
static int hook_CThreadPool_ExecuteToPriority(IThreadPool* pool, void* idx, void* idx2)
{
	if (pool->GetJobCount() <= 0)
		return 0;

	return detour_CThreadPool_ExecuteToPriority.GetTrampoline<Symbols::CThreadPool_ExecuteToPriority>()(pool, idx, idx2);
}
#endif

void CThreadPoolFixModule::Init(CreateInterfaceFn* appfn, CreateInterfaceFn* gamefn)
{
#if ARCHITECTURE_IS_X86_64
	if (g_pThreadPool->NumThreads() > 0)
	{
		DevMsg("holylib: Threadpool is already running? Skipping our fix.\n"); // Seems to currently work again in gmod but I'll just leave it in
		return;
	}

	ThreadPoolStartParams_t startParams;
	startParams.bEnableOnLinuxDedicatedServer = true;
	g_pThreadPool->Start(startParams);
#endif
}

void CThreadPoolFixModule::InitDetour(bool bPreServer)
{
	if (!bPreServer)
		return;

#if ARCHITECTURE_IS_X86
	SourceSDK::ModuleLoader libvstdlib_loader("libvstdlib_srv");
	Detour::Create(
		&detour_CThreadPool_ExecuteToPriority, "CThreadPool::ExecuteToPriority",
		libvstdlib_loader.GetModule(), Symbols::CThreadPool_ExecuteToPrioritySym,
		(void*)hook_CThreadPool_ExecuteToPriority, m_pID
	);
#endif
}