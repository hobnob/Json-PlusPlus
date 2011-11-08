/* 
 * File:   Null.cpp
 * Author: David North
 */

#include "Null.h"

namespace Json
{
    namespace Value
    {
        std::string Null::toString()
        {
            return "null";
        }
    }
}
