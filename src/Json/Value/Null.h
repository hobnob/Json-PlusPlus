/* 
 * File:   Null.h
 * Author: David North
 */

#ifndef NULL_H
#define	NULL_H

#include "../IValue.h"

namespace Json
{
    namespace Value
    {
        class Null : public Json::IValue
        {
            public:
                std::string toString();
        };
    }
}

#endif	/* NULL_H */
