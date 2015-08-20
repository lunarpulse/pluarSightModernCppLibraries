// Debugging.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <crtdbg.h>
//#include <stdio.h>
#include <Windows.h>

#define ASSERT _ASSERTE

#ifdef _DEBUG
#define VERIFY ASSERT
#else
#define VERIFY(espression) (expression)
#endif // _DEBUG

static_assert(sizeof(float) == 4, "cannot seralise floats!");
struct LastException
{
	DWORD result;
	LastException() : result{ GetLastError() } {}
};

struct ManualResetEvent
{
	HANDLE m_handle;

	ManualResetEvent()
	{
		m_handle = CreateEvent(nullptr, true, false, nullptr);

		if (!m_handle)
		{
			throw LastException();
		}
	}
	~ManualResetEvent()
	{
		VERIFY(CloseHandle(m_handle));
	}
};

auto main() ->	int
{
	//ASSERT(4 == 5);
	//printf("success!\n");
	auto e = ManualResetEvent{};

	auto e2 = e;

}

