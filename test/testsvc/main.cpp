#include "pch.h"

using namespace std;
using namespace win32cpp;

class testsvc : public service_base
{
public:
	std::wstring name() const override
	{
		return L"TestSvc";
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
	std::wstring name() const override
	{
		return L"TestSvc2";
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
	try
	{
		if (!wcscmp(argv[1], L"--console"))
		{
			wcout << L"Running service from console. Press Ctrl+C to stop." << endl;

			auto controller = console_service_controller::instance();
			controller->add(make_shared<testsvc>());
			controller->add(make_shared<testsvc2>());
			controller->run();
		}
		else if (!wcscmp(argv[1], L"--install"))
		{
			auto moduleFilename = getModuleFilename();

			auto serviceInstaller = service_installer<testsvc>{};
			serviceInstaller.install(moduleFilename);

			auto serviceInstaller2 = service_installer<testsvc2>{};
			serviceInstaller2.install(moduleFilename);
		}
		else if (!wcscmp(argv[1], L"--uninstall"))
		{
			auto serviceInstaller = service_installer<testsvc>{};
			serviceInstaller.uninstall();

			auto serviceInstaller2 = service_installer<testsvc2>{};
			serviceInstaller2.uninstall();
		}
		else
		{
			auto serviceRegistration = service_registration{};
			serviceRegistration.add(service_controller<testsvc>{});
			serviceRegistration.add(service_controller<testsvc2>{});

			serviceRegistration.startDispatcher();
		}
	}
	catch (const check_failed& e)
	{
		wcout << L"ERROR (" << e.file << ";" << e.line << "): " << getErrorMessage(e.error) << endl;
	}
}
