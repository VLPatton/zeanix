#ifndef _ASSERT_H
#define _ASSERT_H 1

#include <stdlib.h>

#define assert(check) do { if (!(check)) abort(); } while(0)
 
#endif
