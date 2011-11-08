/* 
 * File:   Decoder.h
 * Author: David North
 *
 */

#ifndef JSON_DECODER_H
#define	JSON_DECODER_H

#include <string>
#include <map>
#include <vector>
#include <regex.h>

#include "IValue.h"
#include "Value/Object.h"
#include "Value/Array.h"
#include "Value/String.h"
#include "Value/Numeric.h"
#include "Value/Boolean.h"
#include "Value/Null.h"

#include "Exceptions/Exception.h"

namespace Json
{
    class Decoder
    {
        public:
            static IValue* decode(std::string value);

        private:
            static IValue*              _decode(std::string value, int &pos);
            static std::string         _decodeString(std::string value);
            static std::vector<IValue*> _decodeArray(std::string value, int &pos);
            static float               _decodeNumeric(std::string value, int &pos);
            static bool                _decodeBoolean(std::string value);
            static void                _decodeNull(std::string value);
            static std::map<std::string, IValue*> _decodeObject(std::string value, int &pos);
    };
}
#endif	/* JSON_DECODER_H */
