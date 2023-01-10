#include <xcc-core/xcc.h>



// Dynamic library process attach
void xcc_core_dynamic_library_process_attach()
{
}

// Dynamic library process detach
void xcc_core_dynamic_library_process_detach()
{
}

// Dynamic library thread attach
void xcc_core_dynamic_library_thread_attach()
{
}

// Dynamic library thread detach
void xcc_core_dynamic_library_thread_detach()
{
}


// Dynamic library entry
#if defined(XCC_SYSTEM_WINDOWS)
#if defined(__cplusplus)
extern "C"
#endif
BOOL WINAPI DllMain(HANDLE _DllHandle, DWORD _Reason, LPVOID _Reserved)
{
	UNREFERENCED_PARAMETER(_DllHandle);
	UNREFERENCED_PARAMETER(_Reserved);

	switch(_Reason)
	{
		case DLL_PROCESS_ATTACH:
			xcc_core_dynamic_library_process_attach();
			break;
		case DLL_THREAD_ATTACH:
			xcc_core_dynamic_library_thread_attach();
			break;
		case DLL_THREAD_DETACH:
			xcc_core_dynamic_library_thread_detach();
			break;
		case DLL_PROCESS_DETACH:
			xcc_core_dynamic_library_process_detach();
			break;
		default:
			break;
	}
	return TRUE;
}
#else
__attribute((constructor)) void xcc_core_dynamic_library_init(void)
{
	xcc_core_dynamic_library_process_attach();
}

__attribute((destructor)) void xcc_core_dynamic_library_fini(void)
{
	xcc_core_dynamic_library_process_detach();
}
#endif
