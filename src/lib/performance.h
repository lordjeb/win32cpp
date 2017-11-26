#pragma once
#include <functional>


namespace win32cpp
{

	//	Measures the time in milliseconds that a function (or lambda) takes to complete.
	//
	//	double tt = measureElapsedTime( [&](){
	//		//	Do something that takes time...
	//	} );
	//
	auto measureElapsedTime( std::function<void()> fn ) -> double;
}
