#include "..\..\Includes\customCRT.h"

#pragma function(memset)
void * __stdcall memset(void * dest, char c, unsigned int len)
{
    unsigned int i;
    unsigned int fill;
    unsigned int chunks = len / sizeof(fill);
    char * char_dest = (char *)dest;
    unsigned int * uint_dest = (unsigned int *)dest;

    //
    //  Note we go from the back to the front.  This is to 
    //  prevent newer compilers from noticing what we're doing
    //  and trying to invoke the built-in memset instead of us.
    //

    fill = (c<<24) + (c<<16) + (c<<8) + c;

    for (i = len; i > chunks * sizeof(fill); i--) {
        char_dest[i - 1] = c;
    }

    for (i = chunks; i > 0; i--) {
        uint_dest[i - 1] = fill;
    }

    return dest;
}

#pragma function(memcpy)
void * __stdcall memcpy(void * dest, void * src, unsigned int len)
{
    unsigned int i;
    char * char_src = (char *)src;
    char * char_dest = (char *)dest;
    for (i = 0; i < len; i++) {
        char_dest[i] = char_src[i];
    }
    return dest;
}
