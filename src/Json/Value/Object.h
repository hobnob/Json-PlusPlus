/* 
 * File:   Object.h
 * Author: David North
 */

#ifndef JSON_VALUE_OBJECT_H
#define	JSON_VALUE_OBJECT_H

#include <map>

#include "../IValue.h"

namespace Json
{
    namespace Value
    {
        class Object : public Json::IValue
        {
            private:
                std::map<std::string, Json::IValue*> _val;

            public:
                void set( std::map<std::string, Json::IValue*> val);
                std::string toString();
        };
    }
}

#endif	/* JSON_VALUE_OBJECT_H */
