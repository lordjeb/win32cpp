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

void win32cpp::console_service_controller::add(std::unique_ptr<service_base> p)
{
    m_services.emplace_back(std::move(p));
}

void win32cpp::console_service_controller::run(unsigned int argc, wchar_t* argv[])
{
    SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);

    std::for_each(begin(m_services), end(m_services),
                  [argc, argv](const std::unique_ptr<service_base>& service) { service->onInitialize(argc, argv); });

    std::unique_lock<std::mutex> lock(m_servicesStoppedMutex);
    m_servicesStopped.wait(lock);
}

void win32cpp::console_service_controller::stop()
{
    // Stop the services in the reverse order in which they were started
    std::for_each(rbegin(m_services), rend(m_services),
                  [](const std::unique_ptr<service_base>& service) { service->onStop(); });

    // Signal thread running console_service_controller::run that all services have been stopped
    std::unique_lock<std::mutex> lock(m_servicesStoppedMutex);
    m_servicesStopped.notify_all();
}

void win32cpp::service_registration::startDispatcher()
{
    // Build a SERVICE_TABLE_ENTRY array and call StartServiceCtrlDispatcher to tell SCM to call our ServiceMain
    // function (controler::service_main)
    std::vector<SERVICE_TABLE_ENTRY> service_table;
    for (size_t i = 0; i < m_serviceNames.size(); ++i)
    {
        service_table.emplace_back(
            SERVICE_TABLE_ENTRY{ const_cast<LPWSTR>(m_serviceNames[i].c_str()), m_serviceMains[i] });
    }
    service_table.emplace_back(SERVICE_TABLE_ENTRY{ nullptr, nullptr });
    CHECK_BOOL(StartServiceCtrlDispatcherW(&service_table[0]));
}
