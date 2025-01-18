#include <xcc-core/container/XAllocator.h>
#include <xcc-core/XException.h>



// Core分配器: 申请内存
static XAllocator::elem_type* __xcall__ x_core_allocator_malloc(XAllocator::size_type _Size) noexcept
{
	auto		vMemory = new(std::nothrow) XAllocator::elem_type [_Size];
	return vMemory;
}

// Core分配器: 释放内存
static void __xcall__ x_core_allocator_free(const XAllocator::elem_type* _Address) noexcept
{
	delete[] _Address;
}



// 分配器的静态值
const XAllocator::size_type		XAllocator::redundancy = 2;
const XAllocator::size_type		XAllocator::minimum_capacity = 16;



// constructor
XAllocator::XAllocator() noexcept
{
	this->_memory_address = nullptr;
	this->_memory_length = 0;
	this->_memory_capacity = 0;

	this->malloc(0);
}

// constructor
XAllocator::XAllocator(size_type _Length, int _Char) noexcept
{
	this->_memory_address = nullptr;
	this->_memory_length = 0;
	this->_memory_capacity = 0;

	this->malloc(_Length);
	x_posix_memset(this->_memory_address, _Char, this->_memory_length);
}

// constructor
XAllocator::XAllocator(const void* _Memory, size_type _Length) noexcept
{
	this->_memory_address = nullptr;
	this->_memory_length = 0;
	this->_memory_capacity = 0;

	this->malloc(_Length);
	x_posix_memcpy(this->_memory_address, _Memory, this->_memory_length);
}

// constructor
XAllocator::XAllocator(const XAllocator& _Allocator) noexcept
{
	this->_memory_address = nullptr;
	this->_memory_length = 0;
	this->_memory_capacity = 0;

	this->malloc(_Allocator._memory_length);
	x_posix_memcpy(this->_memory_address, _Allocator._memory_address, this->_memory_length);
}

// constructor
XAllocator::XAllocator(XAllocator&& _Allocator) noexcept
{
	this->_memory_address = _Allocator._memory_address;
	this->_memory_length = _Allocator._memory_length;
	this->_memory_capacity = _Allocator._memory_capacity;

	_Allocator._memory_address = nullptr;
	_Allocator._memory_length = 0;
	_Allocator._memory_capacity = 0;
}

// destructor
XAllocator::~XAllocator() noexcept
{
	x_core_allocator_free(this->_memory_address);
}





// operator override =
XAllocator& XAllocator::operator = (const XAllocator& _Allocator) noexcept
{
	if(this != &_Allocator)
	{
		this->free();
		if(this->malloc(_Allocator.size()))
		{
			x_posix_memcpy(this->_memory_address, _Allocator._memory_address, this->_memory_length);
		}
	}
	return *this;
}

// operator override =
XAllocator& XAllocator::operator = (XAllocator&& _Allocator) noexcept
{
	if(this != &_Allocator)
	{
		this->free();

		this->_memory_address = _Allocator._memory_address;
		this->_memory_length = _Allocator._memory_length;
		this->_memory_capacity = _Allocator._memory_capacity;

		_Allocator._memory_address = nullptr;
		_Allocator._memory_length = 0;
		_Allocator._memory_capacity = 0;
	}
	return *this;
}





// operator override +
XAllocator XAllocator::operator + (const XAllocator& _Allocator) noexcept
{
	auto		vTemp = *this;
	vTemp += _Allocator;
	return vTemp;
}





// operator override +=
XAllocator& XAllocator::operator += (const XAllocator& _Allocator) noexcept
{
	auto		vLengthT = this->_memory_length;
	auto		vLengthO = _Allocator._memory_length;

	if(this->resize(vLengthT + vLengthO))
	{
		x_posix_memcpy(this->_memory_address + vLengthT, _Allocator._memory_address, vLengthO);
	}
	return *this;
}





// operator override []
XAllocator::elem_type& XAllocator::operator [] (pos_type _Index)
{
	if(this->_memory_length <= _Index)
	{
		throw XException::range_error();
	}
	return this->_memory_address[_Index];
}

// operator override []
XAllocator::elem_type XAllocator::operator [] (pos_type _Index) const
{
	if(this->_memory_length <= _Index)
	{
		throw XException::range_error();
	}
	return this->_memory_address[_Index];
}





// [opt] capacity from length
XAllocator::size_type XAllocator::capacity_from_length(size_type _Length) noexcept
{
	auto		vCapacity = XAllocator::minimum_capacity;
	while((_Length + XAllocator::redundancy) >= vCapacity)
	{
		vCapacity <<= 1;
	};
	return vCapacity;
}





// [opt] malloc
bool XAllocator::malloc(size_type _Length) noexcept
{
	auto		vCapacity = XAllocator::capacity_from_length(_Length);
	auto		vMemory = x_core_allocator_malloc(vCapacity);

	if(nullptr == vMemory)
	{
		return false;
	}
	x_posix_memset(vMemory, 0, vCapacity);

	x_core_allocator_free(this->_memory_address);
	this->_memory_address = vMemory;
	this->_memory_length = _Length;
	this->_memory_capacity = vCapacity - XAllocator::redundancy;

	return true;
}

// [opt] clear
void XAllocator::clear() noexcept
{
	this->malloc(0);
}

// [opt] resize
bool XAllocator::resize(size_type _Length) noexcept
{
	auto		vCapacity = XAllocator::capacity_from_length(_Length);
	auto		vMemory = static_cast<XAllocator::elem_type*>(nullptr);

	if(_Length > this->_memory_length)
	{
		if(_Length > this->_memory_capacity)
		{
			vMemory = x_core_allocator_malloc(vCapacity);
			if(nullptr == vMemory)
			{
				return false;
			}
			x_posix_memcpy(vMemory, this->_memory_address, this->_memory_length);
			x_posix_memset(vMemory + this->_memory_length, 0, vCapacity - this->_memory_length);

			x_core_allocator_free(this->_memory_address);
			this->_memory_address = vMemory;
			this->_memory_length = _Length;
			this->_memory_capacity = vCapacity - XAllocator::redundancy;
		}
		else
		{
			this->_memory_length = _Length;
		}
	}
	else if(_Length < this->_memory_length)
	{
		x_posix_memset(this->_memory_address + _Length, 0, this->_memory_length - _Length);
		this->_memory_length = _Length;
	}
	return true;
}

// [opt] reverse
bool XAllocator::reverse(size_type _Length) noexcept
{
	auto		vCapacity = XAllocator::capacity_from_length(_Length);
	auto		vMemory = static_cast<XAllocator::elem_type*>(nullptr);

	if(_Length > this->_memory_capacity)
	{
		vMemory = x_core_allocator_malloc(vCapacity);
		if(nullptr == vMemory)
		{
			return false;
		}
		x_posix_memcpy(vMemory, this->_memory_address, this->_memory_length);
		x_posix_memset(vMemory + this->_memory_length, 0, vCapacity - this->_memory_length);

		x_core_allocator_free(this->_memory_address);
		this->_memory_address = vMemory;
		this->_memory_capacity = vCapacity - XAllocator::redundancy;
	}
	return true;
}

// [opt] free
void XAllocator::free() noexcept
{
	x_core_allocator_free(this->_memory_address);
	this->_memory_address = nullptr;
	this->_memory_length = 0;
	this->_memory_capacity = 0;
}





// [get] exist
bool XAllocator::exist() const noexcept
{
	return this->_memory_length != 0;
}

// [get] empty
bool XAllocator::empty() const noexcept
{
	return this->_memory_length == 0;
}

// [get] data
void* XAllocator::data() noexcept
{
	return this->_memory_address;
}

// [get] data
const void* XAllocator::data() const noexcept
{
	return this->_memory_address;
}

// [get] size
XAllocator::size_type XAllocator::size() const noexcept
{
	return this->_memory_length;
}

// [get] capacity
XAllocator::size_type XAllocator::capacity() const noexcept
{
	return this->_memory_capacity;
}

// [get] surplus
XAllocator::size_type XAllocator::surplus() const noexcept
{
	return this->_memory_capacity - this->_memory_length;
}

// [get] max_size
XAllocator::size_type XAllocator::max_size() const noexcept
{
	return static_cast<size_type>(-1);
}





// [opt] prepend
bool XAllocator::prepend(const void* _Memory, size_type _Length) noexcept
{
	return this->insert(0, _Memory, _Length);
}

// [opt] prepend
bool XAllocator::prepend(const XAllocator& _Allocator) noexcept
{
	return this->insert(0, _Allocator);
}





// [opt] append
bool XAllocator::append(const void* _Memory, size_type _Length) noexcept
{
	return this->insert(this->size(), _Memory, _Length);
}

// [opt] append
bool XAllocator::append(const XAllocator& _Allocator) noexcept
{
	return this->insert(this->size(), _Allocator);
}





// [opt] insert
bool XAllocator::insert(pos_type _Pos, const void* _Memory, size_type _Length) noexcept
{
	if(nullptr == _Memory || _Length == 0)
	{
		return false;
	}

	if(this->surplus() < _Length && !this->reverse(this->_memory_length + _Length))
	{
		return false;
	}

	if(_Pos > this->_memory_length)
	{
		_Pos = this->_memory_length;
	}

	x_posix_memmove(this->_memory_address + _Pos + _Length, this->_memory_address + _Pos, this->_memory_length - _Pos);
	x_posix_memcpy(this->_memory_address + _Pos, _Memory, _Length);
	this->_memory_length += _Length;
	return true;
}

// [opt] append
bool XAllocator::insert(pos_type _Pos, const XAllocator& _Allocator) noexcept
{
	return this->insert(_Pos, _Allocator.data(), _Allocator.size());
}





// [opt] remove
bool XAllocator::remove(pos_type _Pos, size_type _Length) noexcept
{
	if(_Pos >= this->_memory_length)
	{
		return false;
	}

	if(_Length > (this->_memory_length - _Pos))
	{
		_Length = this->_memory_length - _Pos;
	}

	x_posix_memmove(this->_memory_address + _Pos, this->_memory_address + _Pos + _Length, this->_memory_length - _Pos - _Length);
	x_posix_memset(this->_memory_address + (this->_memory_length - _Length), 0, _Length);
	this->_memory_length -= _Length;
	return true;
}





// [opt] replace
bool XAllocator::replace(size_type _Pos, size_type _Length, const void* _After, size_type _LengthA) noexcept
{
	size_type 	vDifference = 0;

	if(_Pos + _Length > this->_memory_length)
	{
		return false;
	}

	if(_Length == _LengthA)
	{
		x_posix_memcpy(this->_memory_address + _Pos, _After, _LengthA);
	}
	else if(_Length > _LengthA)
	{
		vDifference = _Length - _LengthA;
		x_posix_memmove(this->_memory_address + _Pos + _LengthA, this->_memory_address + _Pos + _Length, this->_memory_length - _Pos - _Length);
		x_posix_memset(this->_memory_address + this->_memory_length - vDifference, 0, vDifference);
		x_posix_memcpy(this->_memory_address + _Pos, _After, _LengthA);
		this->_memory_length += _LengthA;
		this->_memory_length -= _Length;
	}
	else
	{
		vDifference = _LengthA - _Length;
		if(this->surplus() < vDifference && !this->reverse(this->_memory_length + vDifference))
		{
			return false;
		}
		x_posix_memmove(this->_memory_address + _Pos + _LengthA, this->_memory_address + _Pos + _Length, this->_memory_length - _Pos - _Length);
		x_posix_memcpy(this->_memory_address + _Pos, _After, _LengthA);
		this->_memory_length += _LengthA;
		this->_memory_length -= _Length;
	}
	return true;
}

// [opt] replace
bool XAllocator::replace(size_type _Pos, size_type _Length, const XAllocator& _After) noexcept
{
	return this->replace(_Pos, _Length, _After._memory_address, _After._memory_length);
}





// [opt] compare
int XAllocator::compare(const void* _Memory, size_type _Length) const noexcept
{
	int		vCompare = 0;

	if (this->_memory_address == _Memory)
	{
		return 0;
	}
	if (this->_memory_address && _Memory)
	{
		if(this->_memory_length == _Length)
		{
			vCompare = x_posix_memcmp(this->_memory_address, _Memory, _Length);
		}
		else if(this->_memory_length > _Length)
		{
			vCompare = x_posix_memcmp(this->_memory_address, _Memory, _Length);
			if(vCompare == 0)
			{
				vCompare = -1;
			}
		}
		else
		{
			vCompare = x_posix_memcmp(this->_memory_address, _Memory, this->_memory_length);
			if(vCompare == 0)
			{
				vCompare = 1;
			}
		}
		return vCompare;
	}
	else if(this->empty() && nullptr == _Memory)
	{
		return 0;
	}
	else if(this->_memory_address)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

// [opt] compare
int XAllocator::compare(const XAllocator& _Allocator) const noexcept
{
	return this->compare(_Allocator._memory_address, _Allocator._memory_length);
}





// [opt] find
XAllocator::pos_type XAllocator::find(const void* _Memory, size_type _Length, pos_type _Pos) const noexcept
{
	if(this->_memory_address == _Memory)
	{
		return 0;
	}
	if(this->_memory_length == 0 || nullptr == _Memory || _Length == 0 || _Pos > this->_memory_length  || (this->_memory_length - _Pos) < _Length)
	{
		return XAllocator::npos;
	}

	auto		vCount = this->_memory_length - _Length;
	for(XAllocator::pos_type vPos = _Pos; vPos <= vCount; ++vPos)
	{
		if(0 == x_posix_memcmp(this->_memory_address + vPos, _Memory, _Length))
		{
			return vPos;
		}
	}

	return XAllocator::npos;
}

// [opt] find
XAllocator::pos_type XAllocator::find(const XAllocator& _Allocator, pos_type _Pos) const noexcept
{
	return this->find(_Allocator._memory_address, _Allocator._memory_length, _Pos);
}





// [opt] rfind
XAllocator::pos_type XAllocator::rfind(const void* _Memory, size_type _Length, pos_type _Pos) const noexcept
{
	if(this->_memory_address == _Memory)
	{
		return 0;
	}
	if(this->_memory_length == 0 || nullptr == _Memory || _Length == 0 || _Pos > this->_memory_length  || (this->_memory_length - _Pos) < _Length)
	{
		return XAllocator::npos;
	}

	for(; _Pos != XAllocator::npos; --_Pos)
	{
		if(0 == x_posix_memcmp(this->_memory_address + _Pos, _Memory, _Length))
		{
			return _Pos;
		}
		if(_Pos == 0)
		{
			break;
		}
	}

	return XAllocator::npos;
}

// [opt] rfind
XAllocator::pos_type XAllocator::rfind(const XAllocator& _Allocator, pos_type _Pos) const noexcept
{
	return this->rfind(_Allocator._memory_address, _Allocator._memory_length, _Pos);
}





// [opt] 子项是否全部一样
bool XAllocator::isSameElements() const noexcept
{
	return this->isSameElements(1);
}

// [opt] 子项是否全部一样
bool XAllocator::isSameElements(size_type _ElementSize) const noexcept
{
	if(this->_memory_length == 0 || _ElementSize == 0 || this->_memory_length % _ElementSize)
	{
		return false;
	}
	auto		vMatchStatus = false;
	auto		vMatchData = new(std::nothrow) XAllocator::elem_type [_ElementSize];
	if(nullptr == vMatchData)
	{
		return false;
	}
	x_posix_memcpy(vMatchData, this->_memory_address, _ElementSize);
	vMatchStatus = this->isSameElements(_ElementSize, vMatchData);
	delete[] vMatchData;

	return vMatchStatus;
}

// [opt] 子项是否全部一样
bool XAllocator::isSameElements(size_type _ElementSize, const void* _MatchData) const noexcept
{
	if(this->_memory_length == 0 || _ElementSize == 0 || nullptr == _MatchData || this->_memory_length % _ElementSize != 0)
	{
		return false;
	}

	for(size_type vIndex = 0; vIndex < this->_memory_length; vIndex += _ElementSize)
	{
		if(x_posix_memcmp(this->_memory_address + vIndex, _MatchData, _ElementSize) != 0)
		{
			return false;
		}
	}
	return true;
}
