#pragma once
#include <vector>
#include <Windows.h>
#include "debug.hpp"
#include "error.hpp"
#include "handle.hpp"


namespace vxshared
{
	//	Defines the interface that a service should implement
	class service_base
	{
	public:
		explicit service_base(const std::wstring& name) : m_name{ name }
		{
		}

		virtual ~service_base()
		{
		}

		virtual DWORD controlsAccepted()
		{
			return SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;
		}

		std::wstring name() const
		{
			return m_name;
		}

		virtual void onContinue()
		{
		}

		virtual void onInitialize()
		{
		}

		virtual void onPause()
		{
		}

		virtual void onStop()
		{
		}

	private:
		const std::wstring m_name;
	};


	//
	template <typename T>
	class service_controller
	{
	public:
		static_assert(std::is_base_of<service_base, T>::value, "T must be a derived class of service_base");

		explicit service_controller()
			: m_ssh{ nullptr }
		{
			memset(&m_ss, 0, sizeof(m_ss));
			m_ss.dwServiceType = SERVICE_WIN32_SHARE_PROCESS; // TODO: Get this from the T??
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

	//
	class service_registration
	{
	public:
		template <typename T> void add(__in const service_controller<T>& controller)
		{
			//	Save the service name and pointer to the service main for the controller type
			m_serviceNames.push_back(controller.name());
			m_serviceMains.push_back(controller.serviceMain);
		}

		void startDispatcher()
		{
			//	Build a SERVICE_TABLE_ENTRY array and call StartServiceCtrlDispatcher to tell SCM to call our ServiceMain function (controler::service_main)
			std::vector<SERVICE_TABLE_ENTRY> service_table;
			for (size_t i = 0; i < m_serviceNames.size(); ++i)
			{
				service_table.push_back(SERVICE_TABLE_ENTRY{ const_cast<LPWSTR>(m_serviceNames[i].c_str()), m_serviceMains[i] });
			}
			service_table.push_back(SERVICE_TABLE_ENTRY{ nullptr, nullptr });
			CHECK_BOOL(StartServiceCtrlDispatcherW(&service_table[0]));
		}

	private:
		std::vector<std::wstring> m_serviceNames;
		std::vector<LPSERVICE_MAIN_FUNCTION> m_serviceMains;
	};
}
