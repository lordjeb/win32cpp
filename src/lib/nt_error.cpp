#include "nt_error.h"

const char* win32cpp::ntstatus_check_failed::errorCodeMessage() const
{
    return u8"NTSTATUS: ";
}
