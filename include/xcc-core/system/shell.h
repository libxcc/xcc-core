#ifndef			_XCC_CORE_SYSTEM_SHELL_H_
#define			_XCC_CORE_SYSTEM_SHELL_H_

#include <xcc-core/header.h>
#include <xcc-core/container/string.h>


// Shell
namespace xcc::shell
{
	// Get terminal result
	_XCOREAPI_ XString __xcall__ terminal_command(const XString& _Command) noexcept;
}

#endif
