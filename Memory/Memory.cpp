// Memory.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <memory>

using namespace std;

struct Hen
{
	unsigned id;
	float eggs;

	Hen(unsigned id_, float eggs_): id {id_}, eggs{eggs_}
	{

	}

	~Hen()
	{
		TRACE(L"Chicken soup!\n");
	}
};

auto GetHen()-> unique_ptr<Hen>//Hen*
{
	return make_unique<Hen>(2, 3.9f);
}

auto UpdateHen(unique_ptr<Hen> hen)->unique_ptr<Hen>
{
	hen->eggs += 1.8f;
	return hen;
}

auto hen()->void
{
	//auto sp = unique_ptr<int>{ new int{123 } };
	//auto sp = make_unique<int>(123);

	auto hen = make_unique<Hen>(1, 5.6f);
	if (hen) TRACE(L"'hen' owns  the Hen !\n");


	auto hen2 = move(hen);

	if (hen) TRACE(L"'hen' owns  the Hen !\n");
	if (hen2) TRACE(L"'hen2' owns  the Hen !\n");


	Hen copy = *hen2;

	Hen & ref = *hen2;

	Hen* ptr = hen2.get();

	ptr = hen2.release();

	hen.reset(ptr);

	hen = UpdateHen(move(hen));

	TRACE(L"eggs %f\n", hen->eggs);

	auto getHen = GetHen();

	TRACE(L"eggs %f\n", getHen->eggs);


}

struct work_deleter
{
	typedef PTP_WORK pointer;
	auto operator()(pointer value)const throw()->void
	{
		CloseThreadpoolWork(value);
	}
};

auto uniquePointerdeleter() -> void
{
	typedef unique_ptr<PTP_WORK, work_deleter> work;

	auto w = work{ CreateThreadpoolWork([](PTP_CALLBACK_INSTANCE, void* , PTP_WORK)
						{
							TRACE(L"Created a thread pool object\n");
						},
							nullptr,nullptr) };
	
		
	

	if (w)
	{
		SubmitThreadpoolWork(w.get());
		WaitForThreadpoolWorkCallbacks(w.get(), false);
		//CloseThreadpoolWork(w);

	}
}

auto sharedPointerVoid() -> void
{
	auto sp = shared_ptr<int>{};

	ASSERT(!sp);
	ASSERT(sp.use_count()== 0);
	ASSERT(!sp.unique());

	sp = make_shared<int>(1234);

	ASSERT(sp);
	ASSERT(sp.use_count() == 1	);
	ASSERT(sp.unique());

	auto sp2 = sp;
	
	ASSERT(sp.use_count() ==2);
	ASSERT(!sp.unique());
	ASSERT(sp2.use_count() == 2);
	ASSERT(!sp2.unique());

	int copy = *sp;
	int &ref = *sp;
	int * ptr = sp.get();

	ASSERT(sp.get() == sp2.get());
	ASSERT(sp == sp2);


}

auto weakPointerDemo() -> void
{
	auto sp = make_shared<int>(12345);

	auto wp = weak_ptr<int>{};

	wp = sp;

	ASSERT(!wp.expired());
	ASSERT(wp.use_count() == 1);

	if (auto locked = wp.lock())
	{
		TRACE(L"locked! %d\n", *locked);
	}

	sp = nullptr;
	
	ASSERT(wp.expired());
	ASSERT(wp.use_count() == 0);

	if (auto locked = wp.lock())
	{
		TRACE(L"locked! %d\n", *locked);
	}
	else
	{
		wp.reset();
	}
	
}
auto main() ->int
{

	hen();

	//typedef unique_ptr<PTP_WORK> work;

	uniquePointerdeleter();

	sharedPointerVoid();
	weakPointerDemo();
}

