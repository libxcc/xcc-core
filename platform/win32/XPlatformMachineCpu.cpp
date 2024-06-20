#include <source/machine/XMachineCpuPrivate.h>
#include <intrin.h>


/*
Windows Registry Editor Version 5.00

[HKEY_LOCAL_MACHINE\HARDWARE\DESCRIPTION\System\CentralProcessor\0]
"Component Information"=hex:00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00
"Identifier"="Intel64 Family 6 Model 165 Stepping 5"
"Configuration Data"=hex(9):ff,ff,ff,ff,ff,ff,ff,ff,00,00,00,00,00,00,00,00
"ProcessorNameString"="Intel(R) Core(TM) i9-10900K CPU @ 3.70GHz"
"VendorIdentifier"="GenuineIntel"
"FeatureSet"=dword:3d1b3fff
"~MHz"=dword:00000e70
"Update Revision"=hex:00,00,00,00,c8,00,00,00
"Update Status"=dword:00000002
"Previous Update Revision"=hex:00,00,00,00,c8,00,00,00
"Platform Specific Field 1"=dword:00000002
*/

// https://learn.microsoft.com/en-za/previous-versions/visualstudio/visual-studio-2008/hskdteyh(v=vs.90)


// windows: CpuCountSetBits
static DWORD CpuCountSetBits(ULONG_PTR bitMask)
{
	DWORD		LSHIFT = (sizeof(ULONG_PTR) * 8) - 1;
	DWORD		vBitSetCount = 0;
	ULONG_PTR	vBitTest = ((ULONG_PTR)1) << LSHIFT;
	DWORD		vIndex = 0;

	for (vIndex = 0; vIndex <= LSHIFT; ++vIndex)
	{
		vBitSetCount += ((bitMask & vBitTest) ? 1 : 0);
		vBitTest /= 2;
	}
	return vBitSetCount;
}

// windows: GetLogicalProcessorInformation
static int GetLogicalProcessorInformation(SYSTEM_LOGICAL_PROCESSOR_INFORMATION** _CpuInfo, size_t* _Count)
{
	SYSTEM_LOGICAL_PROCESSOR_INFORMATION*	vProcessInfo = nullptr;
	DWORD					vReturnedLength = 0;

	GetLogicalProcessorInformation(nullptr, &vReturnedLength);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
	{
		return 0;
	}
	vProcessInfo = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION*) x_posix_malloc(vReturnedLength);
	if(vProcessInfo == nullptr)
	{
		return 0;
	}
	if(GetLogicalProcessorInformation(vProcessInfo, &vReturnedLength))
	{
		*_CpuInfo = vProcessInfo;
		*_Count = vReturnedLength / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
		return 0;
	}
	x_posix_free(vProcessInfo);
	return -1;
}

// windows: 从注册表获取CPU数据
static bool XMachineCpuPrivateByRegistry(XMachineCpuPrivate* _Private) noexcept
{
	// 从注册表获取信息
	HKEY		vKey = nullptr;
	auto		vSyncR = RegOpenKeyExA(HKEY_LOCAL_MACHINE, R"(HARDWARE\DESCRIPTION\System\CentralProcessor\0)", 0, KEY_READ, &vKey);
	if (ERROR_SUCCESS == vSyncR)
	{
		DWORD 		vTempSize = XCC_SIZE_KB - 1;
		char		vTempText[XCC_SIZE_KB] = {0};
		DWORD		vTempValue = 0;

		// 名称
		vSyncR = RegQueryValueExA(vKey, "ProcessorNameString", nullptr, nullptr, (LPBYTE)vTempText, &vTempSize);
		if (ERROR_SUCCESS == vSyncR)
		{
			vTempText[vTempSize] = 0;
			_Private->model_name = vTempText;
		}

		// 频率
		vTempSize = sizeof(DWORD);
		vSyncR = RegQueryValueExA(vKey, "~MHz", nullptr, nullptr, (LPBYTE)&vTempValue, &vTempSize);
		if (ERROR_SUCCESS == vSyncR)
		{
			_Private->frequency = vTempValue * 1000000;
		}

		// 指令集
		vTempSize = sizeof(DWORD);
		vSyncR = RegQueryValueExA(vKey, "FeatureSet", nullptr, nullptr, (LPBYTE)&vTempValue, &vTempSize);
		if (ERROR_SUCCESS == vSyncR)
		{
			_Private->features = XString::number(vTempValue);
		}

		// 供应商
		vTempSize = XCC_SIZE_KB - 1;
		vSyncR = RegQueryValueExA(vKey, "VendorIdentifier", nullptr, nullptr, (LPBYTE)vTempText, &vTempSize);
		if (ERROR_SUCCESS == vSyncR)
		{
			vTempText[vTempSize] = 0;
			_Private->vendor_id = vTempText;
		}
		RegCloseKey(vKey);
	}
	else
	{
		return false;
	}
	return true;
}

// windows: 从COM获取CPU数据
static bool XMachineCpuPrivateById(XMachineCpuPrivate* _Private) noexcept
{
	if(nullptr == _Private)
	{
		return false;
	}

	int		vCpuData[4] = {-1};

	__cpuid(vCpuData, 0x80000006);
	auto		vCacheLine = (vCpuData[2] >> 0) & 0x000000FF;
	auto		vCacheSize = (vCpuData[2] >> 16) & 0x0000FFFF;
	_Private->cache_alignment = vCacheLine;
	_Private->cache_size_bit = vCacheSize * XCC_SIZE_KB;

	__cpuid(vCpuData, 0x80000008);
	auto		vPhysicalAddress = (vCpuData[0] >> 0) & 0x000000FF;
	auto		vVirtualAddress = (vCpuData[0] >> 8) & 0x000000FF;

	_Private->address_bits_physical = vPhysicalAddress;
	_Private->address_bits_virtual = vVirtualAddress;

	return vPhysicalAddress && vVirtualAddress;
}

// [set] 初始化对象
bool XMachineCpuPrivate::init(XMachineCpuPrivate* _Private) noexcept
{
	if(nullptr == _Private)
	{
		return false;
	}

	// 获取CPU核心数与线程数
	size_t 		vCpuCoreCount = 0;
	size_t 		vCpuThreadCount = 0;
	size_t 		vProcessIndex = 0;
	size_t 		vProcessCount = 0;
	size_t 		vProcessCacheLineSize = 0;
	auto		vProcessInfo = static_cast<SYSTEM_LOGICAL_PROCESSOR_INFORMATION*>(nullptr);
	GetLogicalProcessorInformation(&vProcessInfo, &vProcessCount);

	if(vProcessInfo && vProcessCount > 0)
	{
		for(vProcessIndex = 0; vProcessIndex < vProcessCount; ++vProcessIndex)
		{
			if (vProcessInfo[vProcessIndex].Relationship == RelationProcessorCore)
			{
				vCpuCoreCount++;
				vCpuThreadCount += CpuCountSetBits(vProcessInfo[vProcessIndex].ProcessorMask);
			}
			if (0 == vProcessCacheLineSize && vProcessInfo[vProcessIndex].Relationship == RelationCache && vProcessInfo[vProcessIndex].Cache.Level == 1)
			{
				vProcessCacheLineSize = vProcessInfo[vProcessIndex].Cache.LineSize;
			}
		}
		x_posix_free(vProcessInfo);
	}
	_Private->core_count = vCpuCoreCount;
	_Private->thread_count = vCpuThreadCount;

	// 从注册表获取信息
	XMachineCpuPrivateByRegistry(_Private);

	// 从CPUID获取数据
	XMachineCpuPrivateById(_Private);

	return true;
}
