/* 
 * File:   Value.h
 * Author: David North
 */

#ifndef JSON_VALUE_H
#define	JSON_VALUE_H

#include <string>

namespace Json
{
    class IValue
    {
        public:
            IValue(){};
            virtual ~IValue() {}
            virtual void set(std::string val){};

            virtual std::string getString(){return"";};
            virtual bool getBoolean(){return false;};

            virtual std::string toString(){};
    };
}

#endif	/* JSON_VALUE_H */
