#include "pch.h"
#include "service.h"

win32cpp::console_service_controller* win32cpp::console_service_controller::m_instance = nullptr;

win32cpp::console_service_controller::console_service_controller()
{
}

win32cpp::console_service_controller* win32cpp::console_service_controller::instance()
{
    if (!m_instance)
    {
        m_instance = new console_service_controller();
    }
    return m_instance;
}

BOOL win32cpp::console_service_controller::CtrlHandler(DWORD controlType)
{
    switch (controlType)
    {
    case CTRL_C_EVENT:
        instance()->stop();
        return TRUE;
    }
    return FALSE;
}

void win32cpp::console_service_controller::add(std::shared_ptr<service_base> p)
{
    m.push_back(p);
}

void win32cpp::console_service_controller::run(unsigned int argc, wchar_t* argv[])
{
    SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);
    m_serviceStopEvent = unique_handle{ CreateEvent(nullptr, TRUE, FALSE, nullptr) };
    CHECK_BOOL(bool(m_serviceStopEvent));

    for (auto p : m)
    {
        p->onInitialize(argc, argv);
    }

    WaitForSingleObject(m_serviceStopEvent.get(), INFINITE);
}

void win32cpp::console_service_controller::stop()
{
    for (auto p : m)
    {
        p->onStop();
    }

    CHECK_BOOL(SetEvent(m_serviceStopEvent.get()));
}

void win32cpp::service_registration::startDispatcher()
{
    // Build a SERVICE_TABLE_ENTRY array and call StartServiceCtrlDispatcher to tell SCM to call our ServiceMain
    // function (controler::service_main)
    std::vector<SERVICE_TABLE_ENTRY> service_table;
    for (size_t i = 0; i < m_serviceNames.size(); ++i)
    {
        service_table.push_back(
            SERVICE_TABLE_ENTRY{ const_cast<LPWSTR>(m_serviceNames[i].c_str()), m_serviceMains[i] });
    }
    service_table.push_back(SERVICE_TABLE_ENTRY{ nullptr, nullptr });
    CHECK_BOOL(StartServiceCtrlDispatcherW(&service_table[0]));
}
