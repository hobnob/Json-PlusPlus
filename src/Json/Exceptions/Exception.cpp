/* 
 * File:   Exception.cpp
 * Author: David North
 */

#include "Exception.h"

namespace Json
{
    namespace Exceptions
    {
        Exception::Exception(std::string const& s) : std::runtime_error(s){}
    }
}
