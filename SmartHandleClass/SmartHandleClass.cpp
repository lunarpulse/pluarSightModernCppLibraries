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

	typedef unique_handle<null_handle_traits> null_handle;
}

#include <utility>

using namespace std;
using namespace SmartClass;


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
}

