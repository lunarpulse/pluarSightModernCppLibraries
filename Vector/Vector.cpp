// Vector.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>

using namespace std;

auto main() -> int
{
	auto c = vector<int>{};

	_ASSERT(c.empty());
	_ASSERT(c.size() == 0);

	c = vector<int>{ 1,2,3,4,5 };

	_ASSERT(!c.empty());
	_ASSERT(c.size() == 5);

	c = vector<int>{ begin(c), end(c) };

	_ASSERT(!c.empty());
	_ASSERT(c.size() == 5);

	c = vector<int>{ begin(c) + 1, end(c) - 1 };

	_ASSERT(!c.empty());
	_ASSERT(c.size() == 3);

	c = vector <int>{ 1,2,3,4,5 };
	_ASSERT(!c.empty());
	_ASSERT(c.size() == 5);

	c = vector<int>{ begin(c) + 1, end(c) - 1 };

	_ASSERT(c.size() == 3);

	_ASSERT(c[0] == 2);
	_ASSERT(c[1] == 3);
	_ASSERT(c[2] == 4);

	c = vector<int>(10, 1234);

	_ASSERT(c.size() == 10);

	for (auto e : c)
	{
		_ASSERT(e == 1234);
	}

	c = vector<int>{ 1,2,3,4,5 };
	for (auto e : c)
	{
		printf("%d\n", e);
	}

	printf("\n");
	for (auto i = begin(c); i < end(c); i++)
	{
		printf("%d\n", *i);
	}
	printf("\n");
	for (auto i = rbegin(c); i < rend(c); i++)
	{
		printf("%d\n", *i);
	}
	printf("\n");
	for (auto i = 0u; i < c.size(); i++)
	{
		printf("%d\n", c[i]);
	}
	printf("\n");

	c.emplace_back(6);
	c.emplace(begin(c), 0);

	for (auto e : c)
	{
		printf("%d\n", e);
	}

}
