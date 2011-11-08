/* 
 * File:   Numeric.cpp
 * Author: David North
 */

#include "Numeric.h"

namespace Json
{
    namespace Value
    {
        void Numeric::set(float val)
        {
            this->_val = val;
        }

        std::string Numeric::toString()
        {
            char ret[100];
            if ((int) this->_val == this->_val)
            {
                sprintf(ret, "%d", (int) this->_val);
            }
            else
            {
                sprintf(ret, "%g", this->_val);
            }

            return ret;
        }
    }
}
