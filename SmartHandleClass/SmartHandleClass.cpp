// SmartHandleClass.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

namespace SmartClass
{
	template <typename Traits>
	class unique_handle
	{
		typedef typename Traits::pointer pointer;
		pointer m_value;
		auto close() throw() -> void
		{
			if (*this)
			{
				Traits::close(m_value);
			}
		}

		unique_handle(unique_handle const &)= delete;
		auto operator=(unique_handle const &)->unique_handle & = delete;

	public:
		explicit unique_handle(pointer value = Traits::invalid()) throw() :
			m_value{ value }
		{

		}

		unique_handle(unique_handle && other) throw() :
			m_value{ other.release() }
		{

		}
		
		auto operator= (unique_handle && other) throw() -> unique_handle &
		{
			if (this != &other)
			{
				reset(other.release());
			}

			return *this;
		}
		~unique_handle() throw()
		{
			close();
		}

		explicit operator bool() const throw()
		{
			return m_value != Traits::invalid();
		}

		auto get() const throw() -> pointer
		{
			return m_value;
		}

		auto release() throw() -> pointer
		{
			auto value = m_value;
			m_value = Traits::invalid();
			return value;
		}

		auto reset(pointer value = Traits::invalid()) throw() -> bool
		{
			if (m_value != value)
			{
				close();
				m_value = value;
			}
			return static_cast<bool>(*this);
		}
	};

	struct null_handle_traits
	{
		typedef HANDLE pointer;

		static auto invalid() throw() -> pointer // change it to the noexcept semantic
		{
			return nullptr;
		}
		static auto close(pointer value) throw() -> void
		{
			VERIFY(CloseHandle(value));
		}
	};

	struct invalid_handle_traits
	{
		typedef HANDLE pointer;

		static auto invalid() throw() -> pointer
		{
			return INVALID_HANDLE_VALUE;
		}

		static auto close(pointer value) throw() -> void
		{
			VERIFY(CloseHandle(value));
		}
	};

	typedef unique_handle<null_handle_traits> null_handle;
	typedef unique_handle<invalid_handle_traits> invalid_handle;
}


#include <utility>
#include <memory>

using namespace std;
using namespace SmartClass;

struct map_view_deleter
{
	typedef char const* pointer;
	auto operator()(pointer value) const throw() -> void
	{
		VERIFY(UnmapViewOfFile(value));
	}
};

int main()
{
	/*auto raw = HANDLE{ nullptr };
	auto h = null_handle{raw};
	
	if (h)
	{

	}

	bool b = static_cast<bool>(h);
	*/

	auto event = null_handle
	{
		CreateEvent(nullptr,
					true,
					false,
					nullptr)
	};


	if (event)
	{
		VERIFY(SetEvent(event.get()));
	}

	HANDLE danger = event.release();

	VERIFY(CloseHandle(danger));

	if (event.reset(CreateEvent(nullptr, false, false, nullptr)))
	{
		TRACE(L"Reset!\n");
	}

	ASSERT(event);

	auto other = null_handle{ move(event) };

	ASSERT(!event);
	ASSERT(other);

	event = move(other);
	ASSERT(!other);
	ASSERT(event);

	/*auto copy = null_handle{ event };
	copy = event;*/

	auto filename = LR"(C:\Users\uqmjung\Documents\Visual Studio 2015\Projects\pluarSightModernCppLibraries\SmartHandleClass\SmartHandleClass.cpp)";

	auto file = invalid_handle
	{
		CreateFile(filename,
			GENERIC_READ,
			FILE_SHARE_READ,
			nullptr,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			nullptr)
	};

	if (!file)
	{
		printf("CreateFile failed %d\n", GetLastError());
		
	}
	auto size = LARGE_INTEGER{};

	if (!GetFileSizeEx(file.get(), &size))
	{
		printf("GetFileSizeEx failed %d\n", GetLastError());
		return 0;
	}
	
	if (size.QuadPart == 0)
	{
		printf("File is empty\n");
		return 0;
	}

	auto map = null_handle
	{
		CreateFileMapping(file.get(),
		nullptr,
			PAGE_READONLY,
			0,0,
			nullptr)
	};

	if (!map)
	{
		printf("CreateFileMaping failed %d\n", GetLastError());
		return 0;
	}

	file.reset();

	auto view = unique_ptr<char, map_view_deleter>
	{
		static_cast<char*> (MapViewOfFile(map.get(),
											FILE_MAP_READ,
												0,0,
												0))
	};

	if (!view)
	{
		printf("MapViewOfFile failed %d\n", GetLastError());
		return 0;
	}

	printf("%.*s\n", static_cast<unsigned>(size.QuadPart), view.get());
}

