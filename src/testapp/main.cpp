#include <debug.hpp>

void _tmain()
{
    ASSERT(true);
    ASSERT(false);
    
    TRACE(_T("This line will output to the debugger only in debug builds\n"));

    RELTRACE(_T("This line will output to the debugger in all builds\n"));
    RELTRACE(_T("%s\n"), _T("Tracing macros can be used with format specifier strings"));
}
