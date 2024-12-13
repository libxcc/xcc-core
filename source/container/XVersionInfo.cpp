#include <xcc-core/container/XVersionInfo.h>



// XCC - 版本信息
struct PKVersionInfo
{
	// 主版本号
	x_uint32_t 					major = 0;

	// 次版本号
	x_uint32_t					minor = 0;

	// 补丁号
	x_uint32_t					patch = 0;

	// 构建号
	x_uint32_t					build = 0;
};



// [opt] 从整数格式化
static bool XCC_Version_Format_From_Num(PKVersionInfo* _Private, x_uint32_t _VerNum, XVersionInfo::VersionFormat _VF) noexcept
{
	XCC_UNUSED(_VF);

	auto		d_ptr = _Private;

	d_ptr->major = (_VerNum & 0xFF000000) >> 24;
	d_ptr->minor = (_VerNum & 0x00FF0000) >> 16;
	d_ptr->patch = (_VerNum & 0x0000FF00) >> 8;
	d_ptr->build = (_VerNum & 0x000000FF) >> 0;

	return true;
}

// [opt] 从整数格式化
static x_uint32_t XCC_Version_Format_To_Num(PKVersionInfo* _Private, XVersionInfo::VersionFormat _VF) noexcept
{
	XCC_UNUSED(_VF);

	auto		d_ptr = _Private;

	x_uint32_t	vVerNum = 0;
	vVerNum += (d_ptr->major) << 24;
	vVerNum += (d_ptr->minor) << 16;
	vVerNum += (d_ptr->patch) << 8;
	vVerNum += (d_ptr->build) << 0;

	return vVerNum;
}

// [opt] 从字符串格式化
static bool XCC_Version_Format_From_Str(PKVersionInfo* _Private, const XString& _VerStr, XVersionInfo::VersionFormat _VF) noexcept
{
	XCC_UNUSED(_VF);

	auto		d_ptr = _Private;

	x_uint32_t	vMajor = 0;
	x_uint32_t	vMinor = 0;
	x_uint32_t	vPatch = 0;
	x_uint32_t	vBuild = 0;

	auto		vFormat = 0;
	auto		vCount = (int)_VerStr.count('.');
	switch(vCount)
	{
		case 1:
			vFormat = x_posix_sscanf(_VerStr.data(), "%u.%u", &vMajor, &vMinor);
			break;
		case 2:
			vFormat = x_posix_sscanf(_VerStr.data(), "%u.%u.%u", &vMajor, &vMinor, &vPatch);
			break;
		case 3:
			vFormat = x_posix_sscanf(_VerStr.data(), "%u.%u.%u.%u", &vMajor, &vMinor, &vPatch, &vBuild);
			break;
		default:
			break;
	}

	if(0 != vFormat && vFormat == vCount + 1)
	{
		d_ptr->major = vMajor;
		d_ptr->minor = vMinor;
		d_ptr->patch = vPatch;
		d_ptr->build = vBuild;
		return true;
	}

	return false;
}

// [opt] 从字符串格式化
static XString XCC_Version_Format_To_Str(PKVersionInfo* _Private, XVersionInfo::VersionFormat _VF) noexcept
{
	auto		d_ptr = _Private;

	if(XVersionInfo::VersionFormatV3 == _VF)
	{
		auto		vVerStr = XString::format("%u.%u.%u", d_ptr->major, d_ptr->minor, d_ptr->patch);
		return vVerStr;
	}
	else
	{
		auto		vVerStr = XString::format("%u.%u.%u.%u", d_ptr->major, d_ptr->minor, d_ptr->patch, d_ptr->build);
		return vVerStr;
	}
}



// constructor
XVersionInfo::XVersionInfo() noexcept
{
	d_ptr = new(std::nothrow) PKVersionInfo();
}

// constructor
XVersionInfo::XVersionInfo(x_uint32_t _VerNum) noexcept
{
	d_ptr = new(std::nothrow) PKVersionInfo();
	XCC_Version_Format_From_Num(d_ptr, _VerNum, VersionFormatAuto);
}

// constructor
XVersionInfo::XVersionInfo(const char* _VerStr) noexcept
{
	d_ptr = new(std::nothrow) PKVersionInfo();
	XCC_Version_Format_From_Str(d_ptr, _VerStr, VersionFormatAuto);
}

// constructor
XVersionInfo::XVersionInfo(const XString& _VerStr) noexcept
{
	d_ptr = new(std::nothrow) PKVersionInfo();
	XCC_Version_Format_From_Str(d_ptr, _VerStr, VersionFormatAuto);
}

// constructor
XVersionInfo::XVersionInfo(const XVersionInfo& _Right) noexcept
{
	d_ptr = new(std::nothrow) PKVersionInfo();
	*d_ptr = *_Right.d_ptr;
}

// destructor
XVersionInfo::~XVersionInfo() noexcept
{
	delete d_ptr;
}



// operator override =
XVersionInfo& XVersionInfo::operator = (x_uint32_t _VerNum) noexcept
{
	XCC_Version_Format_From_Num(d_ptr, _VerNum, VersionFormatAuto);
	return *this;
}

// operator override =
XVersionInfo& XVersionInfo::operator = (const char* _VerStr) noexcept
{
	XCC_Version_Format_From_Str(d_ptr, _VerStr, VersionFormatAuto);
	return *this;
}

// operator override =
XVersionInfo& XVersionInfo::operator = (const XString& _VerStr) noexcept
{
	XCC_Version_Format_From_Str(d_ptr, _VerStr, VersionFormatAuto);
	return *this;
}

// operator override =
XVersionInfo& XVersionInfo::operator = (const XVersionInfo& _Right) noexcept
{
	if(this != &_Right)
	{
		*d_ptr = *_Right.d_ptr;
	}
	return *this;
}



// operator override ==
bool XVersionInfo::operator == (x_uint32_t _VerNum) const noexcept
{
	return *this == XVersionInfo(_VerNum);
}

// operator override ==
bool XVersionInfo::operator == (const char* _VerStr) const noexcept
{
	return *this == XVersionInfo(_VerStr);
}

// operator override ==
bool XVersionInfo::operator == (const XString& _VerStr) const noexcept
{
	return *this == XVersionInfo(_VerStr);
}

// operator override ==
bool XVersionInfo::operator == (const XVersionInfo& _Right) const noexcept
{
	return 0 == this->compare(_Right);
}

// operator override !=
bool XVersionInfo::operator != (x_uint32_t _VerNum) const noexcept
{
	return *this != XVersionInfo(_VerNum);
}

// operator override !=
bool XVersionInfo::operator != (const char* _VerStr) const noexcept
{
	return *this != XVersionInfo(_VerStr);
}

// operator override !=
bool XVersionInfo::operator != (const XString& _VerStr) const noexcept
{
	return *this != XVersionInfo(_VerStr);
}

// operator override !=
bool XVersionInfo::operator != (const XVersionInfo& _Right) const noexcept
{
	return 0 != this->compare(_Right);
}

// operator override <
bool XVersionInfo::operator < (x_uint32_t _VerNum) const noexcept
{
	return *this < XVersionInfo(_VerNum);
}

// operator override <
bool XVersionInfo::operator < (const char* _VerStr) const noexcept
{
	return *this < XVersionInfo(_VerStr);
}

// operator override <
bool XVersionInfo::operator < (const XString& _VerStr) const noexcept
{
	return *this < XVersionInfo(_VerStr);
}

// operator override <
bool XVersionInfo::operator < (const XVersionInfo& _Right) const noexcept
{
	return this->compare(_Right) < 0;
}

// operator override >
bool XVersionInfo::operator > (x_uint32_t _VerNum) const noexcept
{
	return *this > XVersionInfo(_VerNum);
}

// operator override >
bool XVersionInfo::operator > (const char* _VerStr) const noexcept
{
	return *this > XVersionInfo(_VerStr);
}

// operator override >
bool XVersionInfo::operator > (const XString& _VerStr) const noexcept
{
	return *this > XVersionInfo(_VerStr);
}

// operator override >
bool XVersionInfo::operator > (const XVersionInfo& _Right) const noexcept
{
	return this->compare(_Right) > 0;
}

// operator override <=
bool XVersionInfo::operator <= (x_uint32_t _VerNum) const noexcept
{
	return *this <= XVersionInfo(_VerNum);
}

// operator override <=
bool XVersionInfo::operator <= (const char* _VerStr) const noexcept
{
	return *this <= XVersionInfo(_VerStr);
}

// operator override <=
bool XVersionInfo::operator <= (const XString& _VerStr) const noexcept
{
	return *this <= XVersionInfo(_VerStr);
}

// operator override <=
bool XVersionInfo::operator <= (const XVersionInfo& _Right) const noexcept
{
	return this->compare(_Right) <= 0;
}

// operator override >=
bool XVersionInfo::operator >= (x_uint32_t _VerNum) const noexcept
{
	return *this >= XVersionInfo(_VerNum);
}

// operator override >=
bool XVersionInfo::operator >= (const char* _VerStr) const noexcept
{
	return *this >= XVersionInfo(_VerStr);
}

// operator override >=
bool XVersionInfo::operator >= (const XString& _VerStr) const noexcept
{
	return *this >= XVersionInfo(_VerStr);
}

// operator override >=
bool XVersionInfo::operator >= (const XVersionInfo& _Right) const noexcept
{
	return this->compare(_Right) >= 0;
}



// [opt] 是否为空
bool XVersionInfo::empty() const noexcept
{
	return (0 == d_ptr->major) && (0 == d_ptr->minor) && (0 == d_ptr->patch) && (0 == d_ptr->build);
}

// [opt] 是否有值
bool XVersionInfo::exist() const noexcept
{
	return (0 != d_ptr->major) || (0 != d_ptr->minor) || (0 != d_ptr->patch) || (0 != d_ptr->build);
}



// [fmt] 从整数格式化
XVersionInfo XVersionInfo::fromNumber(x_uint32_t _VerNum, VersionFormat _VF) noexcept
{
	auto		vObject = XVersionInfo();
	XCC_Version_Format_From_Num(vObject.d_ptr, _VerNum, _VF);
	return vObject;
}

// [fmt] 从字符串格式化
XVersionInfo XVersionInfo::fromString(const XString& _VerStr, VersionFormat _VF) noexcept
{
	auto		vObject = XVersionInfo();
	XCC_Version_Format_From_Str(vObject.d_ptr, _VerStr, _VF);
	return vObject;
}



// [cnv] 转换为整数
x_uint32_t XVersionInfo::toNumber() const noexcept
{
	return toNumber(VersionFormatAuto);
}

// [cnv] 转换为字符串
XString XVersionInfo::toString() const noexcept
{
	return toString(VersionFormatAuto);
}

// [cnv] 转换为整数
x_uint32_t XVersionInfo::toNumber(VersionFormat _VF) const noexcept
{
	auto		vVerNum = XCC_Version_Format_To_Num(d_ptr, _VF);
	return vVerNum;
}

// [cnv] 转换为字符串
XString XVersionInfo::toString(VersionFormat _VF) const noexcept
{
	auto		vVerStr = XCC_Version_Format_To_Str(d_ptr, _VF);
	return vVerStr;
}



// [cmp] 检查是否与指定数据相同
int XVersionInfo::compare(x_uint32_t _VerNum) const noexcept
{
	return XVersionInfo::compare(*this, _VerNum);
}

// [cmp] 检查是否与指定数据相同
int XVersionInfo::compare(const char* _VerStr) const noexcept
{
	return XVersionInfo::compare(*this, _VerStr);
}

// [cmp] 检查是否与指定数据相同
int XVersionInfo::compare(const XString& _VerStr) const noexcept
{
	return XVersionInfo::compare(*this, _VerStr);
}

// [cmp] 检查是否与指定数据相同
int XVersionInfo::compare(const XVersionInfo& _VerInfo) const noexcept
{
	return XVersionInfo::compare(*this, _VerInfo);
}

// [cmp] 检查是否与指定数据相同
int XVersionInfo::compare(const XVersionInfo& _VerInfoL, const XVersionInfo& _VerInfoR) noexcept
{
	auto		vNumberL = _VerInfoL.toNumber();
	auto		vNumberR = _VerInfoR.toNumber();

	if(vNumberL > vNumberR)
	{
		return 1;
	}
	else if(vNumberL < vNumberR)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}



// [set] 主版本号
void XVersionInfo::major(x_uint32_t _Major) noexcept
{
	d_ptr->major = _Major;
}

// [get] 主版本号
x_uint32_t XVersionInfo::major() const noexcept
{
	return d_ptr->major;
}

// [set] 次版本号
void XVersionInfo::minor(x_uint32_t _Minor) noexcept
{
	d_ptr->minor = _Minor;
}

// [get] 次版本号
x_uint32_t XVersionInfo::minor() const noexcept
{
	return d_ptr->minor;
}

// [set] 补丁号
void XVersionInfo::patch(x_uint32_t _Patch) noexcept
{
	d_ptr->patch = _Patch;
}

// [get] 补丁号
x_uint32_t XVersionInfo::patch() const noexcept
{
	return d_ptr->patch;
}

// [set] 构建号
void XVersionInfo::build(x_uint32_t _Build) noexcept
{
	d_ptr->build = _Build;
}

// [get] 构建号
x_uint32_t XVersionInfo::build() const noexcept
{
	return d_ptr->build;
}



// Syntax sugar: operator ==
_XCOREAPI_ bool __xcall__ operator == (x_uint32_t _VerNum, const XVersionInfo& _Object) noexcept
{
	return _Object == _VerNum;
}

// Syntax sugar: operator ==
_XCOREAPI_ bool __xcall__ operator == (const char* _VerStr, const XVersionInfo& _Object) noexcept
{
	return _Object == _VerStr;
}

// Syntax sugar: operator ==
_XCOREAPI_ bool __xcall__ operator == (const XString& _VerStr, const XVersionInfo& _Object) noexcept
{
	return _Object == _VerStr;
}

// Syntax sugar: operator !=
_XCOREAPI_ bool __xcall__ operator != (x_uint32_t _VerNum, const XVersionInfo& _Object) noexcept
{
	return _Object != _VerNum;
}

// Syntax sugar: operator !=
_XCOREAPI_ bool __xcall__ operator != (const char* _VerStr, const XVersionInfo& _Object) noexcept
{
	return _Object != _VerStr;
}

// Syntax sugar: operator !=
_XCOREAPI_ bool __xcall__ operator != (const XString& _VerStr, const XVersionInfo& _Object) noexcept
{
	return _Object != _VerStr;
}

// Syntax sugar: operator <
_XCOREAPI_ bool __xcall__ operator < (x_uint32_t _VerNum, const XVersionInfo& _Object) noexcept
{
	return _Object >= _VerNum;
}

// Syntax sugar: operator <
_XCOREAPI_ bool __xcall__ operator < (const char* _VerStr, const XVersionInfo& _Object) noexcept
{
	return _Object >= _VerStr;
}

// Syntax sugar: operator <
_XCOREAPI_ bool __xcall__ operator < (const XString& _VerStr, const XVersionInfo& _Object) noexcept
{
	return _Object >= _VerStr;
}

// Syntax sugar: operator >
_XCOREAPI_ bool __xcall__ operator > (x_uint32_t _VerNum, const XVersionInfo& _Object) noexcept
{
	return _Object <= _VerNum;
}

// Syntax sugar: operator >
_XCOREAPI_ bool __xcall__ operator > (const char* _VerStr, const XVersionInfo& _Object) noexcept
{
	return _Object <= _VerStr;
}

// Syntax sugar: operator >
_XCOREAPI_ bool __xcall__ operator > (const XString& _VerStr, const XVersionInfo& _Object) noexcept
{
	return _Object <= _VerStr;
}

// Syntax sugar: operator <=
_XCOREAPI_ bool __xcall__ operator <= (x_uint32_t _VerNum, const XVersionInfo& _Object) noexcept
{
	return _Object > _VerNum;
}

// Syntax sugar: operator <=
_XCOREAPI_ bool __xcall__ operator <= (const char* _VerStr, const XVersionInfo& _Object) noexcept
{
	return _Object > _VerStr;
}

// Syntax sugar: operator <=
_XCOREAPI_ bool __xcall__ operator <= (const XString& _VerStr, const XVersionInfo& _Object) noexcept
{
	return _Object > _VerStr;
}

// Syntax sugar: operator >=
_XCOREAPI_ bool __xcall__ operator >= (x_uint32_t _VerNum, const XVersionInfo& _Object) noexcept
{
	return _Object < _VerNum;
}

// Syntax sugar: operator >=
_XCOREAPI_ bool __xcall__ operator >= (const char* _VerStr, const XVersionInfo& _Object) noexcept
{
	return _Object < _VerStr;
}

// Syntax sugar: operator >=
_XCOREAPI_ bool __xcall__ operator >= (const XString& _VerStr, const XVersionInfo& _Object) noexcept
{
	return _Object < _VerStr;
}
