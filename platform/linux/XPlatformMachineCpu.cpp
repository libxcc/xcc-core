#include <source/machine/XMachineCpuPrivate.h>
#include <xcc-core/filesystem/XFileSystem.h>


/*
processor       : 0
vendor_id       : GenuineIntel
cpu family      : 6
model           : 165
model name      : Intel(R) Core(TM) i9-10900K CPU @ 3.70GHz
stepping        : 5
microcode       : 0xffffffff
cpu MHz         : 3696.000
cache size      : 256 KB
physical id     : 0
siblings        : 20
core id         : 0
cpu cores       : 10
apicid          : 0
initial apicid  : 0
fpu             : yes
fpu_exception   : yes
cpuid level     : 6
wp              : yes
flags           : fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm pni pclmulqdq dtes64 monitor ds_cpl vmx smx est tm2 ssse3 fma cx16 xtpr pdcm pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave osxsave avx f16c rdrand lahf_lm abm 3dnowprefetch fsgsbase tsc_adjust bmi1 avx2 smep bmi2 erms invpcid mpx rdseed adx smap clflushopt intel_pt pku ibrs ibpb stibp ssbd
bogomips        : 7392.00
clflush size    : 64
cache_alignment : 64
address sizes   : 36 bits physical, 48 bits virtual
power management:
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

// [set] 初始化对象
bool XMachineCpuPrivate::init(XMachineCpuPrivate* _Private) noexcept
{
	auto		vCpuInfo = XFileSystem::file::toBytes("/proc/cpuinfo");
	if(nullptr == vCpuInfo || nullptr == _Private)
	{
		return false;
	}
	auto		vCpuArray = vCpuInfo.split("\n");
	for(const auto & vIterator : vCpuArray)
	{
		if(nullptr == vIterator)
		{
			break;
		}
		auto		vPair = XMachineCpuPrivatePair(vIterator.data());
		if(vPair.first == "flags")			{	_Private->features = vPair.second.toUpper();			}
		else if(vPair.first == "model name")		{	_Private->model_name = vPair.second;				}
		else if(vPair.first == "cpu cores")		{	_Private->core_count = vPair.second.toULLong();			}
		else if(vPair.first == "siblings")		{	_Private->thread_count = vPair.second.toULLong();		}
		else if(vPair.first == "address sizes")
		{
			_Private->address_bits_physical = vPair.second.left(3).toULLong();
			_Private->address_bits_virtual = vPair.second.mid(", ", " bits virtual").toULLong();
		}
		else if(vPair.first == "vendor_id")		{	_Private->vendor_id = vPair.second;				}
		else if(vPair.first == "cache_alignment")	{	_Private->cache_alignment = vPair.second.toULLong();		}
		else if(vPair.first == "cpu MHz")
		{
			auto		vFrequency = vPair.second.toDouble();
			vFrequency *= 1000;
			vFrequency *= 1000;
			_Private->frequency = (x_uint64_t)vFrequency;
		}
		else if(vPair.first == "cache size")
		{
			_Private->cache_size_bit = vPair.second.toULLong() * XCC_SIZE_KB;
		}
	}
	return true;
}
