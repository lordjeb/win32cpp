#include <strstream>
#include <service.h>
#include <string_extensions.h>
#include "module_info.h"

using namespace std;
using namespace win32cpp;

class testsvc : public service_base
{
public:
    virtual DWORD controlsAccepted() const override
    {
        return SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;
    }

    virtual std::wstring name() const override
    {
        return L"TestSvc";
    }

    virtual std::wstring displayName() const override
    {
        return L"";
    }

    virtual void onContinue() override
    {
        TRACE(L"TestSvc::onContinue\n");
    }

    virtual void onInitialize(unsigned int argc, wchar_t* argv[]) override
    {
        UNREFERENCED_PARAMETER(argc);
        UNREFERENCED_PARAMETER(argv);
        TRACE(L"TestSvc::onInitialize\n");
    }

    virtual void onPause() override
    {
        TRACE(L"TestSvc::onPause\n");
    }

    virtual void onStop() override
    {
        TRACE(L"TestSvc::onStop\n");
    }

    virtual DWORD serviceType() const override
    {
        return SERVICE_WIN32_SHARE_PROCESS;
    }
};

class testsvc2 : public service_base
{
public:
    virtual DWORD controlsAccepted() const override
    {
        return SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;
    }

    virtual std::wstring name() const override
    {
        return L"TestSvc2";
    }

    virtual std::wstring displayName() const override
    {
        return L"";
    }

    virtual void onContinue() override
    {
        TRACE(L"TestSvc2::onContinue\n");
    }

    virtual void onInitialize(unsigned int argc, wchar_t* argv[]) override
    {
        UNREFERENCED_PARAMETER(argc);
        UNREFERENCED_PARAMETER(argv);
        TRACE(L"TestSvc2::onInitialize\n");
    }

    virtual void onPause() override
    {
        TRACE(L"TestSvc2::onPause\n");
    }

    virtual void onStop() override
    {
        TRACE(L"TestSvc2::onStop\n");
    }

    virtual DWORD serviceType() const override
    {
        return SERVICE_WIN32_SHARE_PROCESS;
    }
};

int wmain(int argc, wchar_t* argv[])
{
    bool console_mode = true;

    TRACE(L"+ testsvc::wmain\n");

    try
    {
        if (argc > 1)
        {
            if (!wcscmp(argv[1], L"--console"))
            {
                wcout << L"Running service from console. Press Ctrl+C to stop." << endl;

                // Note: Singleton is not ideal here, but SetConsoleCtrlHandler doesn't really provide a way to have
                // context, so we will live with this design
                auto controller = console_service_controller::instance();
                controller->add(make_unique<testsvc>());
                controller->add(make_unique<testsvc2>());
                controller->run(argc, argv);
            }
            else if (!wcscmp(argv[1], L"--install"))
            {
                auto moduleFilename = getModuleFilename();

                auto serviceInstaller = service_installer<testsvc>{};
                serviceInstaller.install(moduleFilename);

                auto serviceInstaller2 = service_installer<testsvc2>{};
                serviceInstaller2.install(moduleFilename);

                wcout << L"Installed." << endl;
            }
            else if (!wcscmp(argv[1], L"--uninstall"))
            {
                auto serviceInstaller = service_installer<testsvc>{};
                serviceInstaller.uninstall();

                auto serviceInstaller2 = service_installer<testsvc2>{};
                serviceInstaller2.uninstall();

                wcout << L"Uninstalled." << endl;
            }
        }
        else
        {
            console_mode = false;

            auto serviceRegistration = service_registration{};
            serviceRegistration.add(service_controller<testsvc>{});
            serviceRegistration.add(service_controller<testsvc2>{});

            serviceRegistration.startDispatcher();
        }

        return 0;
    }
    catch (const exception& e)
    {
        auto what = str2wstr(e.what());

        TRACE(L"%S\n", what.c_str());

        if (console_mode)
        {
            wcout << what << endl;
        }
    }
    catch (...)
    {
        auto what = L"Unhandled and unknown exception";

        TRACE(L"%S\n", what);

        if (console_mode)
        {
            wcout << what << endl;
        }
    }

    TRACE(L"- testsvc::wmain\n");

    return 0;
}
