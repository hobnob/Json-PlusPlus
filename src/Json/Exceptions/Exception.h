/* 
 * File:   Exception.h
 * Author: David North
 */

#ifndef JSON_EXCEPTIONS_EXCEPTION_H
#define	JSON_EXCEPTIONS_EXCEPTION_H

#include <stdexcept>

namespace Json
{
    namespace Exceptions
    {
        class Exception : public std::runtime_error {
            public:
                Exception(std::string const& s);
        };

    }
}

#endif	/* JSON_EXCEPTIONS_EXCEPTION_H */

