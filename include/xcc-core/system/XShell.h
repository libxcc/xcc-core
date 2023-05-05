﻿#ifndef			_XCC_CORE_SHELL_H_
#define			_XCC_CORE_SHELL_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>

// Xanadu Class Shell
class _XCOREAPI_ XShell
{
public:
	XShell() noexcept;

	virtual ~XShell() noexcept;

public:
	// Async run shell
	static int run(const XString& _Shell) noexcept;

	// Sync run shell
	static int run(const XString& _Shell, const std::function<bool(const XString& _Output)>& _Lambda) noexcept;

	// Native system
	static int system(const XString& _Command) noexcept;
};

#endif
