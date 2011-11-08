/* 
 * File:   Numeric.h
 * Author: David North
 */

#ifndef JSON_VALUE_NUMERIC_H
#define	JSON_VALUE_NUMERIC_H

#include <stdio.h>

#include "../IValue.h"

namespace Json
{
    namespace Value
    {
        class Numeric : public Json::IValue
        {
            private:
                float _val;

            public:
                void set(float val);
                std::string toString();
        };
    }
}

#endif	/* JSON_VALUE_NUMERIC_H */
