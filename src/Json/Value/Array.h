/* 
 * File:   Array.h
 * Author: David North
 */

#ifndef JSON_VALUE_ARRAY_H
#define	JSON_VALUE_ARRAY_H

#include <vector>

#include "../IValue.h"

namespace Json
{
    namespace Value
    {
        class Array : public Json::IValue
        {
            private:
               std::vector<Json::IValue*> _val;

            public:
                void set(std::vector<Json::IValue*> val);
                std::string toString();
        };
    }
}
#endif	/* JSON_VALUE_ARRAY_H */
