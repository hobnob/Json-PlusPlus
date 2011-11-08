/* 
 * File:   String.cpp
 * Author: David North
 */

#include "String.h"

namespace Json
{
    namespace Value
    {
        void String::set(std::string val)
        {
            this->_val = val;
        }

        std::string String::getString()
        {
            return this->_val;
        }

        std::string String::toString()
        {
            return "\""+this->_val+"\"";
        }
    }
}
