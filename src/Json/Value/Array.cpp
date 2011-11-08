/* 
 * File:   Array.cpp
 * Author: David North
 */

#include "Array.h"

namespace Json
{
    namespace Value
    {
        void Array::set(std::vector<Json::IValue*> val)
        {
            this->_val = val;
        };

        std::string Array::toString()
        {
            std::string ret = "[";

            for(int i = 0; i < this->_val.size(); i++)
            {
                if (i != 0)
                {
                    ret += ",";
                }

                ret += this->_val.at(i)->toString();
            }

            ret += "]";
            return ret;
        }
    }
}
