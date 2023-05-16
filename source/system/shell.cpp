#include <xcc-core/system/shell.h>
#include <xcc-core/system/XShell.h>


// Get terminal result
_XCOREAPI_ XString __xcall__ xcc::shell::terminal_command(const XString& _Command) noexcept
{
	XString		vTempResult;
	XShell::run(_Command, [&](const XString& _Output)->bool
	{
		vTempResult += _Output;
		return true;
	});
	return vTempResult;
}
