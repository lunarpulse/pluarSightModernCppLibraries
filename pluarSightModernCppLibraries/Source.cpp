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

	c = vector<int>{ begin(c) +1, end(c)-1 };

	_ASSERT(!c.empty());
	_ASSERT(c.size() == 3);

	c = vector <int>{ 1,2,3,4,5 };
	_ASSERT(!c.empty());
	_ASSERT(c.size() == 5);

	c = vector<int>{ begin(c) + 1, end(c) - 1 };

	_ASSERT(c.size() == 3);

	_ASSERT(c[0] == 1);
	_ASSERT(c[1] == 2);
	_ASSERT(c[2] == 3);

	c = vector<int>(10, 1234);

	_ASSERT(c.size() == 10);

	for (auto e : c)
	{
		_ASSERT(e == 1234);
	}

	c = vector<int>{ 1,2,3,4,5 };
	for (auto e : c)
	{
		//TRACE()
	}


}
