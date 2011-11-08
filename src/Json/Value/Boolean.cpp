/* 
 * File:   Boolean.cpp
 * Author: David North
 */

#include "Boolean.h"

namespace Json
{
    namespace Value
    {
        void Boolean::set(bool val)
        {
            this->_val = val;
        }

        bool Boolean::getBoolean()
        {
            return this->_val;
        }

        std::string Boolean::toString()
        {
            if (this->_val)
            {
                return "true";
            }
            else
            {
                return "false";
            }
        }
    }
}
