#include <xcc-core/filesystem/XFolder.h>
#include <xcc-core/filesystem/XFileInfo.h>
#include <source/filesystem/XFolderPrivate.h>


// constructor
XFolder::XFolder() noexcept
{
	d_ptr = new(std::nothrow) XFolderPrivate();
}

// constructor
XFolder::XFolder(XFolderPrivate* _Private) noexcept
{
	d_ptr = _Private;
}

// constructor
XFolder::XFolder(const XString& _Path) noexcept
{
	d_ptr = new(std::nothrow) XFolderPrivate();

	d_ptr->path = XFileInfo::pathToCommon(_Path);
}

// constructor
XFolder::XFolder(const XFolder& _Right) noexcept
{
	d_ptr = new(std::nothrow) XFolderPrivate();

	*(this->d_ptr) = *(_Right.d_ptr);
}

// constructor
XFolder::XFolder(XFolder&& _Right) noexcept
{
	d_ptr = new(std::nothrow) XFolderPrivate();

	*(this->d_ptr) = *(_Right.d_ptr);
}

// destructor
XFolder::~XFolder() noexcept
{
	delete d_ptr;
}



// operator override =
XFolder& XFolder::operator=(const XFolder& _Right) noexcept
{
	if(this != &_Right)
	{
		*(this->d_ptr) = *(_Right.d_ptr);
	}
	return *this;
}

// operator override =
XFolder& XFolder::operator=(XFolder&& _Right) noexcept
{
	if(this != &_Right)
	{
		*(this->d_ptr) = *(_Right.d_ptr);
	}
	return *this;
}
