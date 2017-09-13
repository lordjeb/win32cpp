#include "stdafx.h"
#include "performance.hpp"
#include <functional>

using namespace std;

auto vxshared::measureElapsedTime( function<void()> fn ) -> double
{
	LARGE_INTEGER countsPerS = { 0 };
	LARGE_INTEGER beginElapsedCounts = { 0 };
	LARGE_INTEGER endElapsedCounts = { 0 };

	VERIFY( QueryPerformanceFrequency( &countsPerS ) );
	VERIFY( QueryPerformanceCounter( &beginElapsedCounts ) );

	//	Call the fn we are supposed to measure
	fn();

	VERIFY( QueryPerformanceCounter( &endElapsedCounts ) );
	return ( double( endElapsedCounts.QuadPart - beginElapsedCounts.QuadPart ) * 1.0 * 1000 / double( countsPerS.QuadPart ) );
}
