#include <xcc-core/sync/XProcessInfo.h>
#include <platform/xpa/XPlatformProcess.h>




// constructor
XProcessInfo::XProcessInfo(XPrivateProcessData* _Private) noexcept
{
	d_ptr = _Private;
}



// constructor
XProcessInfo::XProcessInfo() noexcept
{
	d_ptr = new(std::nothrow) XPrivateProcessData();
}

// constructor
XProcessInfo::XProcessInfo(const XProcessInfo& _Right) noexcept
{
	d_ptr = new(std::nothrow) XPrivateProcessData();
	*d_ptr = *_Right.d_ptr;
}

// destructor
XProcessInfo::~XProcessInfo() noexcept
{
	delete d_ptr;
}



// operator =
XProcessInfo& XProcessInfo::operator = (const XProcessInfo& _Right) noexcept
{
	if(this != &_Right)
	{
		*d_ptr = *_Right.d_ptr;
	}
	return *this;
}



// [get] 进程ID
x_uint64_t XProcessInfo::pid() const noexcept
{
	return d_ptr->id;
}

// [get] 进程名称
XString XProcessInfo::name() const noexcept
{
	return d_ptr->name;
}

// [get] 进程路径
XString XProcessInfo::path() const noexcept
{
	return d_ptr->path;
}

// [get] 进程参数
std::list<XString> XProcessInfo::args() const noexcept
{
	return d_ptr->args;
}
