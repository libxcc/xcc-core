#include <source/machine/XMachineCpuPrivate.h>
#include <sys/sysctl.h>


/*
machdep.cpu.max_basic: 22
machdep.cpu.max_ext: 2147483656
machdep.cpu.vendor: GenuineIntel
machdep.cpu.brand_string: Intel(R) Core(TM) i9-10900K CPU @ 3.70GHz
machdep.cpu.family: 6
machdep.cpu.model: 78
machdep.cpu.extmodel: 4
machdep.cpu.extfamily: 0
machdep.cpu.stepping: 3
machdep.cpu.feature_bits: 18445110247204584447
machdep.cpu.leaf7_feature_bits: 10233771 8
machdep.cpu.leaf7_feature_bits_edx: 3154117632
machdep.cpu.extfeature_bits: 1241984796928
machdep.cpu.signature: 263907
machdep.cpu.brand: 0
machdep.cpu.features: FPU VME DE PSE TSC MSR PAE MCE CX8 APIC SEP MTRR PGE MCA CMOV PAT PSE36 CLFSH MMX FXSR SSE SSE2 SS HTT SSE3 PCLMULQDQ MON SSSE3 FMA CX16 SSE4.1 SSE4.2 x2APIC MOVBE POPCNT AES VMM PCID XSAVE OSXSAVE TSCTMR AVX1.0 RDRAND F16C
machdep.cpu.leaf7_features: RDWRFSGS TSC_THREAD_OFFSET BMI1 AVX2 SMEP BMI2 ERMS INVPCID FPU_CSDS RDSEED ADX SMAP CLFSOPT PKU MDCLEAR IBRS STIBP L1DF ACAPMSR SSBD
machdep.cpu.extfeatures: SYSCALL XD 1GBPAGE EM64T LAHF LZCNT PREFETCHW RDTSCP TSCI
machdep.cpu.logical_per_package: 16
machdep.cpu.cores_per_package: 16
machdep.cpu.microcode_version: 200
machdep.cpu.processor_flag: 0
machdep.cpu.mwait.linesize_min: 64
machdep.cpu.mwait.linesize_max: 4096
machdep.cpu.mwait.extensions: 3
machdep.cpu.mwait.sub_Cstates: 16
machdep.cpu.thermal.sensor: 0
machdep.cpu.thermal.dynamic_acceleration: 0
machdep.cpu.thermal.invariant_APIC_timer: 1
machdep.cpu.thermal.thresholds: 0
machdep.cpu.thermal.ACNT_MCNT: 0
machdep.cpu.thermal.core_power_limits: 0
machdep.cpu.thermal.fine_grain_clock_mod: 0
machdep.cpu.thermal.package_thermal_intr: 0
machdep.cpu.thermal.hardware_feedback: 0
machdep.cpu.thermal.energy_policy: 0
machdep.cpu.xsave.extended_state: 519 832 2696 0
machdep.cpu.xsave.extended_state1: 11 832 0 0
machdep.cpu.arch_perf.version: 1
machdep.cpu.arch_perf.number: 4
machdep.cpu.arch_perf.width: 48
machdep.cpu.arch_perf.events_number: 7
machdep.cpu.arch_perf.events: 255
machdep.cpu.arch_perf.fixed_number: 0
machdep.cpu.arch_perf.fixed_width: 0
machdep.cpu.cache.linesize: 64
machdep.cpu.cache.L2_associativity: 4
machdep.cpu.cache.size: 256
machdep.cpu.tlb.inst.large: 8
machdep.cpu.tlb.data.small: 64
machdep.cpu.tlb.data.small_level1: 64
machdep.cpu.address_bits.physical: 43
machdep.cpu.address_bits.virtual: 48
machdep.cpu.core_count: 16
machdep.cpu.thread_count: 16
machdep.cpu.tsc_ccc.numerator: 0
machdep.cpu.tsc_ccc.denominator: 0
*/


// [get] 拆分值
static std::pair<XString, XString> XMachineCpuPrivatePair(const XString& _Text) noexcept
{
	auto		vMiddle = _Text.find(u8": ", xcc::CaseInsensitive);
	if(XString::npos == vMiddle)
	{
		return {};
	}
	return {_Text.left(vMiddle).simplified(), _Text.mid(vMiddle + 2).simplified()};
}

// [get] sysctl_by_name 转换为整形
static x_int64_t XMachineCpuPrivateSysctlToString(const XString& _Key) noexcept
{
	int64_t		vValue = 0;
	size_t		vSize = sizeof(int64_t);
	auto		vSyncB = sysctlbyname(_Key.data(), &vValue, &vSize, nullptr, 0);
	if (0 == vSyncB)
	{
		return vValue;
	}
	else
	{
		return 0;
	}
}

// [set] 初始化对象
bool XMachineCpuPrivate::init(XMachineCpuPrivate* _Private) noexcept
{
	XCC_UNUSED(_Private);

	size_t		vTextSize = XCC_SIZE_MB - 1;
	auto		vTextAddress = new(std::nothrow) char[XCC_SIZE_MB];
	x_posix_memset(vTextAddress, 0, XCC_SIZE_MB);
	auto		vSyncB = sysctlbyname("machdep.cpu", vTextAddress, &vTextSize, nullptr, 0);
	if(0 != vSyncB)
	{
		return false;
	}
	auto		vCpuInfo = XString(vTextAddress);
	auto		vCpuArray = vCpuInfo.split("\n");
	delete[] vTextAddress;
	for(const auto & vIterator : vCpuArray)
	{
		if(nullptr == vIterator)
		{
			break;
		}
		auto		vPair = XMachineCpuPrivatePair(vIterator.data());
		if(vPair.first == "machdep.cpu.features")			{	_Private->features = vPair.second.toUpper();				}
		else if(vPair.first == "machdep.cpu.brand_string")		{	_Private->model_name = vPair.second;					}
		else if(vPair.first == "machdep.cpu.core_count")		{	_Private->core_count = vPair.second.toULLong();				}
		else if(vPair.first == "machdep.cpu.thread_count")		{	_Private->thread_count = vPair.second.toULLong();			}
		else if(vPair.first == "machdep.cpu.address_bits.physical")	{	_Private->address_bits_physical = vPair.second.toULLong();		}
		else if(vPair.first == "machdep.cpu.address_bits.virtual")	{	_Private->address_bits_virtual = vPair.second.toULLong();		}
		else if(vPair.first == "machdep.cpu.vendor")			{	_Private->vendor_id = vPair.second;					}
		else if(vPair.first == "machdep.cpu.cache.linesize")		{	_Private->cache_alignment = vPair.second.toULLong();			}
		else if(vPair.first == "machdep.cpu.cache.size")		{	_Private->cache_size_bit = vPair.second.toULLong() * XCC_SIZE_KB;	}
	}
	_Private->frequency = (x_uint64_t)XMachineCpuPrivateSysctlToString("hw.cpufrequency");
	return true;
}
