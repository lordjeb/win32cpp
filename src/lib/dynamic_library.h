#pragma once
#include "debug.h"
#include "error.h"
#include "handle.h"
#include <string>
#include <Windows.h>

namespace win32cpp
{
    // TODO: How do we get the calling convention in here?

    template <typename TReturn, typename... Args>
    class DynamicFunction
    {
    private:
        typedef TReturn(WINAPI* FuncPtr)(Args...);

    public:
        explicit DynamicFunction(const std::string& library_name, const std::string& function_name)
            : m_library_name{ library_name }, m_function_name{ function_name }
        {
        }

        virtual TReturn operator()(const Args&... args)
        {
            Load();
            return m_func(args...);
        }

    private:
        FuncPtr m_func{ nullptr };
        std::string m_library_name;
        std::string m_function_name;
        unique_library_handle m_library;

        void Load()
        {
            m_library.reset(LoadLibraryA(m_library_name.c_str()));
            if (!m_library)
            {
                throw check_failed(GetLastError(), __FILEW__, __LINE__);
            }

            m_func = (FuncPtr)GetProcAddress(m_library.get(), m_function_name.c_str());
            if (!m_func)
            {
                throw check_failed(GetLastError(), __FILEW__, __LINE__);
            }
        }
    };
}
