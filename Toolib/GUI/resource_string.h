#ifndef RESOURCE_STRING_H
#define RESOURCE_STRING_H

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

#endif // RESOURCE_STRING_H
