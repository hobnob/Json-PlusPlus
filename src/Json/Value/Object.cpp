/* 
 * File:   Object.cpp
 * Author: David North
 */

#include "Object.h"

namespace Json
{
    namespace Value
    {
        void Object::set( std::map<std::string, Json::IValue*> val)
        {
            this->_val = val;
        }

        std::string Object::toString()
        {
            std::string ret = "{";


            typedef std::map<std::string, Json::IValue*> MapType;
            MapType data = this->_val;
            MapType::const_iterator end = data.end(); 

            for (MapType::const_iterator it = data.begin(); it != end; ++it) {
                if (it != data.begin())
                {
                    ret += ",";
                }

                ret += "\""+it->first+"\":"+it->second->toString();
            }
            ret += "}";

            return ret;
        }
    }
}
