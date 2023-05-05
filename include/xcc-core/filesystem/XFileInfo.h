#ifndef			_XCC_CORE_FILE_INFO_H_
#define			_XCC_CORE_FILE_INFO_H_

#include <xcc-core/header.h>
#include <xcc-core/container/XString.h>


// file details
struct XFileInfoPrivate;
typedef struct XFileInfoPrivate			XFileInfoPrivate;
class _XCOREAPI_ XFileInfo
{
private:
	// Private structures
	XFileInfoPrivate*			d_ptr;

public:
	// constructor
	XFileInfo() noexcept;

	// constructor
	explicit XFileInfo(XFileInfoPrivate* _Private) noexcept;

	// constructor
	explicit XFileInfo(const XString& _Path) noexcept;

	// constructor
	XFileInfo(const XFileInfo& _Right) noexcept;

	// constructor
	XFileInfo(XFileInfo&& _Right) noexcept;

	// destructor
	virtual ~XFileInfo() noexcept;

public:
	// operator =
	XFileInfo& operator=(const XFileInfo& _Right) noexcept;

	// operator =
	XFileInfo& operator=(XFileInfo&& _Right) noexcept;

public:
	// [fmt] Convert path to native format
	static XString pathToNative(const XString& _Path) noexcept;

	// [fmt] Convert path to common format
	static XString pathToCommon(const XString& _Path) noexcept;

public:
	// [static] Check if the path exists
	static bool exist(const XString& _Path) noexcept;

	// [static] checks whether the given path refers to a directory
	static bool isDir(const XString& _Path) noexcept;

	// [static] checks whether the given path refers to a file
	static bool isFile(const XString& _Path) noexcept;

	// [static] returns the size of a path
	static x_uint64_t size(const XString& _Path) noexcept;

public:
	// clear all status flags
	virtual void clear() const noexcept final;

	// Does the file exist
	virtual bool exist() const noexcept final;

	// checks whether the given path refers to a directory
	virtual bool isDir() const noexcept final;

	// checks whether the given path refers to a file
	virtual bool isFile() const noexcept final;

	// Query file size
	virtual x_uint64_t size() const noexcept final;

public:
	// file parent directory path
	virtual XString dirPath() const noexcept final;

	// file absolute path
	virtual XString filePath() const noexcept final;

	// file name
	virtual XString fileName() const noexcept final;

	// returns the stem path component
	virtual XString stem() const noexcept final;

	// returns the file extension path component
	virtual XString extension() const noexcept final;

	// returns the file extension path component
	virtual XString suffix() const noexcept final;
};

#endif
