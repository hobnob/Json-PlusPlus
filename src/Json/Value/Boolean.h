/* 
 * File:   Boolean.h
 * Author: David North
 */

#ifndef JSON_VALUE_BOOLEAN_H
#define	JSON_VALUE_BOOLEAN_H

#include "../IValue.h"

namespace Json
{
    namespace Value
    {
        class Boolean : public Json::IValue
        {
            private:
                bool _val;

            public:
                void set(bool val);
                std::string toString();

                bool getBoolean();
        };
    }
}

#endif	/* JSON_VALUE_BOOLEAN_H */
