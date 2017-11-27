#pragma once
#include <vector>
#include <Windows.h>
#include "debug.h"
#include "error.h"
#include "handle.h"


namespace win32cpp
{
	//	Defines the interface that a service should implement
	struct service_base
	{
		virtual DWORD controlsAccepted() const = 0;
		virtual std::wstring name() const = 0;
		virtual std::wstring displayName() const = 0;
		virtual void onContinue() = 0;
		virtual void onInitialize() = 0;
		virtual void onPause() = 0;
		virtual void onStop() = 0;
		virtual DWORD serviceType() const = 0;
	};

	template <typename T>
	class service_controller
	{
	public:
		static_assert(std::is_base_of<service_base, T>::value, "T must be a derived class of service_base");

		explicit service_controller()
			: m_ssh{ nullptr }
		{
			memset(&m_ss, 0, sizeof(m_ss));
			m_ss.dwServiceType = m_service_base.serviceType();
			m_ss.dwControlsAccepted = m_service_base.controlsAccepted();
		}

		service_controller(const service_controller& src) = delete;
		service_controller& operator=(const service_controller& src) = delete;

		void main(__in unsigned int argc, __in wchar_t* argv[])
		{
			try
			{
				m_ssh = RegisterServiceCtrlHandlerExW(name().c_str(), serviceControlHandler, this);
				CHECK_BOOL(0 != m_ssh);

				m_ss.dwCurrentState = SERVICE_START_PENDING;
				m_ss.dwCheckPoint = 1;
				m_ss.dwWaitHint = 500; // TODO: Get these from the T
				CHECK_BOOL(SetServiceStatus(m_ssh, &m_ss));

				m_serviceStopEvent = unique_handle{ CreateEvent(nullptr, TRUE, FALSE, nullptr) };
				CHECK_BOOL(static_cast<bool>(m_serviceStopEvent));

				m_service_base.onInitialize();

				m_ss.dwCurrentState = SERVICE_RUNNING;
				m_ss.dwCheckPoint = 0;
				m_ss.dwWaitHint = 0;
				CHECK_BOOL(SetServiceStatus(m_ssh, &m_ss));

				WaitForSingleObject(m_serviceStopEvent.get(), INFINITE);
			}
			catch (...)
			{
				m_ss.dwCurrentState = SERVICE_STOPPED;
				m_ss.dwCheckPoint = 0;
				m_ss.dwWaitHint = 0;
				m_ss.dwWin32ExitCode = ERROR_EXCEPTION_IN_SERVICE;
				SetServiceStatus(m_ssh, &m_ss);
			}
		}

		std::wstring name() const
		{
			return m_service_base.name();
		}

		static DWORD WINAPI serviceControlHandler(__in DWORD dwControl, __in DWORD dwEventType, __in LPVOID lpEventData, __in LPVOID lpContext)
		{
			service_controller<T>* pInstance = reinterpret_cast<service_controller<T>*>(lpContext);
			DWORD dr = NO_ERROR;

			switch (dwControl)
			{
			case SERVICE_CONTROL_STOP:
				pInstance->_stop();
				break;
			case SERVICE_CONTROL_PAUSE:
				pInstance->_pause();
				break;
			case SERVICE_CONTROL_CONTINUE:
				pInstance->_continue();
				break;
			case SERVICE_CONTROL_INTERROGATE:
				//	Docs say we should return NO_ERROR even if we don't handle this
				break;
			default:
				dr = ERROR_CALL_NOT_IMPLEMENTED;
				break;
			}

			return dr;
		}

		static void WINAPI serviceMain(__in DWORD dwArgc, __in LPWSTR* lpszArgv)
		{
			//	This services ServiceMain function, called by SCM to start the service. This function is just a stub
			//	that can be called as a non-class function by SCM. It will create an instance of the appropriate service
			//	and call its main() function, which will be be the "real" service main.
			service_controller<T> instance;
			instance.main(dwArgc, lpszArgv);
		}

	private:
		T m_service_base;
		SERVICE_STATUS_HANDLE m_ssh;
		SERVICE_STATUS m_ss;
		unique_handle m_serviceStopEvent;

		void _continue()
		{
			m_service_base.onContinue();

			m_ss.dwCurrentState = SERVICE_RUNNING;
			m_ss.dwCheckPoint = 0;
			m_ss.dwWaitHint = 0;
			CHECK_BOOL(SetServiceStatus(m_ssh, &m_ss));
		}

		void _pause()
		{
			m_service_base.onPause();

			m_ss.dwCurrentState = SERVICE_PAUSED;
			m_ss.dwCheckPoint = 0;
			m_ss.dwWaitHint = 0;
			CHECK_BOOL(SetServiceStatus(m_ssh, &m_ss));
		}

		void _stop()
		{
			m_ss.dwCurrentState = SERVICE_STOP_PENDING;
			m_ss.dwCheckPoint = 0;
			m_ss.dwWaitHint = 0;
			CHECK_BOOL(SetServiceStatus(m_ssh, &m_ss));

			m_service_base.onStop();

			CHECK_BOOL(SetEvent(m_serviceStopEvent.get()));

			m_ss.dwCurrentState = SERVICE_STOPPED;
			m_ss.dwCheckPoint = 0;
			m_ss.dwWaitHint = 0;
			CHECK_BOOL(SetServiceStatus(m_ssh, &m_ss));
		}
	};
	
	class console_service_controller
	{
	public:
		static console_service_controller* instance();
		static BOOL CtrlHandler(DWORD controlType);
		void add(std::shared_ptr<service_base> p);
		void run();
		void stop();

	private:
		static console_service_controller* m_instance;
		std::vector<std::shared_ptr<service_base>> m;
		unique_handle m_serviceStopEvent;

		console_service_controller();
	};

	template <typename T>
	class service_installer
	{
	public:
		static_assert(std::is_base_of<service_base, T>::value, "T must be a derived class of service_base");

		auto install(const std::wstring& filename, const std::wstring& username = L"", const std::wstring& password = L"",
			DWORD startType = SERVICE_DEMAND_START, DWORD errorControl = SERVICE_ERROR_NORMAL) -> void
		{
			auto scm = unique_service_handle{ OpenSCManagerW(nullptr, SERVICES_ACTIVE_DATABASEW, SC_MANAGER_CREATE_SERVICE) };
			CHECK_BOOL(static_cast<bool>(scm));

			auto service = unique_service_handle{ CreateServiceW(scm.get(), m_service_base.name().c_str(),
				m_service_base.displayName().c_str(), SERVICE_ALL_ACCESS, m_service_base.serviceType(), startType,
				errorControl, filename.c_str(), nullptr, nullptr, nullptr, username.empty() ? nullptr : username.c_str(),
				password.c_str()) };
			CHECK_BOOL(static_cast<bool>(scm));
		}

		auto uninstall() -> void
		{
			auto scm = unique_service_handle{ OpenSCManagerW(nullptr, SERVICES_ACTIVE_DATABASEW, SC_MANAGER_CREATE_SERVICE) };
			CHECK_BOOL(static_cast<bool>(scm));

			auto service = unique_service_handle{ OpenServiceW(scm.get(), m_service_base.name().c_str(), DELETE) };
			CHECK_BOOL(static_cast<bool>(service));
			CHECK_BOOL(DeleteService(service.get()));
		}

	private:
		T m_service_base;
	};

	class service_registration
	{
	public:
		template <typename T> void add(__in const service_controller<T>& controller)
		{
			//	Save the service name and pointer to the service main for the controller type
			m_serviceNames.push_back(controller.name());
			m_serviceMains.push_back(controller.serviceMain);
		}

		void startDispatcher();

	private:
		std::vector<std::wstring> m_serviceNames;
		std::vector<LPSERVICE_MAIN_FUNCTION> m_serviceMains;
	};
}
