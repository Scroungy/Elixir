#include "string.h"

namespace std
{
    bool is_equal(const char *first, const char *second)
    {
        while (*first)
        {
            if (*first != *second)
            {
                return false;
            }
            first++;
            second++;
        }
        return true;
    }

    bool is_equal(const wchar_t *first, const wchar_t *second)
    {
        while (*first)
        {
            if (*first != *second)
            {
                return false;
            }
            first++;
            second++;
        }
        return true;
    }
}