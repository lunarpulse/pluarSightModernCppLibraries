// Containers.cpp : Defines the entry point for the console application.
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

		unique_handle(unique_handle const &) = delete;
		auto operator=(unique_handle const &)->unique_handle & = delete;

	public:
		explicit unique_handle(pointer value = Traits::invalid()) throw() :
			m_value{ value }
		{

		}

		unique_handle(unique_handle && other) throw() :
			m_value{ other.release() }
		{
			TRACE(L"move contor\n");
		}

		auto operator= (unique_handle && other) throw() -> unique_handle &
		{
			if (this != &other)
			{
				reset(other.release());
			}
			TRACE(L"move assignment\n");
			return *this;
		}
		~unique_handle() throw()
		{
			TRACE(L"decontor\n");
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

		auto swap(unique_handle<Traits> & other) throw()-> void
		{
		std::swap(m_value, other.m_value);
		}
	};

	template <typename Traits>
	auto swap(unique_handle<Traits> & left,
	unique_handle<Traits> & right) throw()-> void
	{
		TRACE(L"non member swap\n");
	left.swap(right);
	}
	
	template <typename Traits>
	auto operator==(unique_handle<Traits> const& left,
		unique_handle<Traits> const& right) throw()-> bool
	{
		TRACE(L"non member equality\n");
		return left.get() == right.get();
	}

	template <typename Traits>
	auto operator!=(unique_handle<Traits> const& left,
		unique_handle<Traits> const& right) throw()-> bool
	{
		return left.get() != right.get();
	}

	template <typename Traits>
	auto operator>(unique_handle<Traits> const& left,
		unique_handle<Traits> const& right) throw()-> bool
	{
		return left.get() >right.get();
	}

	template <typename Traits>
	auto operator>=(unique_handle<Traits> const& left,
		unique_handle<Traits> const& right) throw()-> bool
	{
		return left.get() >= right.get();
	}

	template <typename Traits>
	auto operator<=(unique_handle<Traits> const& left,
		unique_handle<Traits> const& right) throw()-> bool
	{
		return left.get() <= right.get();
	}

	template <typename Traits>
	auto operator<(unique_handle<Traits> const& left,
		unique_handle<Traits> const& right) throw()-> bool
	{
		return left.get() < right.get();
	}



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



#include <vector>
#include <algorithm>
#include <memory>

using namespace std;
using namespace SmartClass;

auto vectorMain() -> void
{
	auto c = vector<int>{};

	ASSERT(c.empty());
	ASSERT(c.size() == 0);

	c = vector<int>{ 1,2,3,4,5 };

	ASSERT(!c.empty());
	ASSERT(c.size() == 5);

	c = vector<int>{ begin(c), end(c) };

	ASSERT(!c.empty());
	ASSERT(c.size() == 5);

	c = vector<int>{ begin(c) + 1, end(c) - 1 };

	ASSERT(!c.empty());
	ASSERT(c.size() == 3);

	c = vector <int>{ 1,2,3,4,5 };
	ASSERT(!c.empty());
	ASSERT(c.size() == 5);

	c = vector<int>{ begin(c) + 1, end(c) - 1 };

	ASSERT(c.size() == 3);

	ASSERT(c[0] == 2);
	ASSERT(c[1] == 3);
	ASSERT(c[2] == 4);

	c = vector<int>(10, 1234);

	ASSERT(c.size() == 10);

	for (auto e : c)
	{
		ASSERT(e == 1234);
	}

	c = vector<int>{ 1,2,3,4,5 };
	for (auto e : c)
	{
		TRACE(L"%d\n", e);
	}

	for (auto i = begin(c); i < end(c); i++)
	{
		TRACE(L"%d\n", *i);
	}

	for (auto i = rbegin(c); i < rend(c); i++)
	{
		TRACE(L"%d\n", *i);
	}
	for (auto i = 0u; i < c.size(); i++)
	{
		TRACE(L"%d\n", c[i]);
	}

	c.emplace_back(6);
	c.emplace(begin(c), 0);

	for (auto e : c)
	{
		TRACE(L"%d\n", e);
	}

}

auto swapExample() -> void
{
	auto first = null_handle
	{
		CreateEvent(nullptr,true,false,nullptr)
	};

	auto second = null_handle{};

	swap(first, second);

}

auto adlLookup() -> void
{
	auto first = make_unique<int>(5);
	auto second = make_unique<int>(3);

	swap(first, second);
}

auto swapSimple() -> void
{
	null_handle handles[10000];

	reverse(begin(handles), end(handles));


}
auto main() -> int
{
	
	//adlLookup();
	//swapExample();
	swapSimple();
}
