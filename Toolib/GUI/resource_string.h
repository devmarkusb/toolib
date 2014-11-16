#ifndef RESOURCE_STRING_H_sjzbxgf3uin4fg2n97ytgfvn32y
#define RESOURCE_STRING_H_sjzbxgf3uin4fg2n97ytgfvn32y

#include "../types.h"
#include "resources.h"

namespace too
{
    namespace gui
    {
        class IResourceString
        {
        public:
            virtual ~IResourceString() {}

            virtual too::string GetString(TResID id) const = 0;
        };
    }
}

#endif
