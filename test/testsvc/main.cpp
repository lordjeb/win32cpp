#include "..\..\targetver.h"
//#include <debug.h>
//#include <error.h>
//#include <module_info.h>
//#include <string_extensions.h>
#include <iostream>
#include <service.h>

using namespace std;
using namespace win32cpp;

class testsvc : public service_base
{
public:
	testsvc() : service_base(L"TestSvc")
	{
	}

	void onContinue() override
	{
		TRACE(L"TestSvc::onContinue\n");
	}

	void onInitialize() override
	{
		TRACE(L"TestSvc::onInitialize\n");
	}

	void onPause() override
	{
		TRACE(L"TestSvc::onPause\n");
	}

	void onStop() override
	{
		TRACE(L"TestSvc::onStop\n");
	}
};

class testsvc2 : public service_base
{
public:
	testsvc2() : service_base(L"TestSvc2")
	{
	}

	void onContinue() override
	{
		TRACE(L"TestSvc2::onContinue\n");
	}

	void onInitialize() override
	{
		TRACE(L"TestSvc2::onInitialize\n");
	}

	void onPause() override
	{
		TRACE(L"TestSvc2::onPause\n");
	}

	void onStop() override
	{
		TRACE(L"TestSvc2::onStop\n");
	}
};

void wmain(int argc, const wchar_t* argv[])
{
	if (wcscmp(argv[1], L"--console"))
	{
		wcout << L"Not implemented." << endl;
	}
	else if (wcscmp(argv[1], L"--install"))
	{
		wcout << L"Not implemented." << endl;
	}
	else if (wcscmp(argv[1], L"--uninstall"))
	{
		wcout << L"Not implemented." << endl;
	}
	else
	{
		service_registration serviceRegistration;
		serviceRegistration.add(service_controller<testsvc>{});
		serviceRegistration.add(service_controller<testsvc2>{});

		serviceRegistration.startDispatcher();
	}
}
