/* 
 * File:   String.h
 * Author: David North
 */

#ifndef JSON_VALUE_STRING_H
#define	JSON_VALUE_STRING_H

#include "../IValue.h"

namespace Json
{
    namespace Value
    {
        class String : public Json::IValue
        {
            private:
                std::string _val;

            public:
                void set(std::string val);

                std::string getString();
                std::string toString();
        };
    }
}

#endif	/* JSON_VALUE_STRING_H */
