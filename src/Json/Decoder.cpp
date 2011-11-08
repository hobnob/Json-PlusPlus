
#include "Decoder.h"

namespace Json
{
    IValue* Decoder::decode(std::string value)
    {
        int pos = 0;
        IValue* ret = Decoder::_decode(value, pos);
        return ret;
    }

    IValue* Decoder::_decode(std::string value, int &pos)
    {
        IValue* json;
        std::string str = value.substr(pos);
        if (str.length() > 0)
        {
            switch(str[0])
            {
                case '{':
                {
                    int startPos = 0;
                    json = new Value::Object();
                    (dynamic_cast<Value::Object*>(json))->set(Decoder::_decodeObject(str, startPos));

                    pos += startPos;
                }
                    break;
                case '"':
                    json = new Value::String();
                    json->set(Decoder::_decodeString(str));
                    pos  += (json->getString().length() + 2);
                    break;
                case '[':
                {
                    int startPos = 0;
                    json = new Value::Array();
                    (dynamic_cast<Value::Array*>(json))->set(Decoder::_decodeArray(str, startPos));

                    pos += startPos;
                }
                    break;
                case 't':
                case 'f':
                    json = new Value::Boolean();
                    (dynamic_cast<Value::Boolean*>(json))->set(Decoder::_decodeBoolean(str));

                    if (value[pos] == 't')
                    {
                        pos += 4;
                    }
                    else
                    {
                        pos += 5;
                    }
                    break;
                case 'n':
                    json = new Value::Null();
                    Decoder::_decodeNull(str);
                    pos += 4;
                    break;

                default:
                {
                    int startPos = 0;
                    json = new Value::Numeric();
                    (dynamic_cast<Value::Numeric*>(json))->set(Decoder::_decodeNumeric(str, startPos));

                    pos += startPos;
                }

            }
        }
        return json;
    }

    std::string Decoder::_decodeString(std::string value)
    {
        std::string decoded = "";
        bool escaping = false;

        for(int i = 1; i < value.length(); i++)
        {
            if (escaping)
            {
                switch(value[i])
                {
                    case '"':
                    case '\\':
                    case '/':
                        decoded += value[i];
                        escaping = false;
                        break;
                    case 'b':
                        decoded += '\b';
                        escaping = false;
                        break;
                    case 'f':
                        decoded += '\f';
                        escaping = false;
                        break;
                    case 'n':
                        decoded += '\n';
                        escaping = false;
                        break;
                    case 'r':
                        decoded += '\r';
                        escaping = false;
                        break;
                    case 't':
                        decoded += '\t';
                        escaping = false;
                        break;
                    case 'u':
                    {
                        regex_t preg;
                        size_t nmatch = 1;
                        regmatch_t pmatch[1];

                        regcomp(&preg, "^[0-9]{4}$", 0);

                        const char* compStr = value.substr(i + 1, 4).c_str();
                        if (regexec(&preg, compStr, nmatch, pmatch, 0))
                        {
                            int unicode;
                            sscanf(compStr, "%d", &unicode);
                            decoded +=  static_cast<wchar_t>(unicode);
                            i += 4;
                            escaping = false;
                        }
                        else
                        {
                            throw  Exceptions::Exception("Invalid unicode expression");
                        }
                    }
                        break;
                    default:
                        throw  Exceptions::Exception("Invalid escape option selected");
                }
            }
            else
            {
                switch(value[i])
                {
                    case '"':
                        return decoded;
                        break;
                    case '\\':
                        escaping = true;
                        break;
                    default:
                        decoded += value[i];
                        break;
                }
            }
        }

        throw  Exceptions::Exception("No end of string found");
    }

    std::vector<IValue*> Decoder::_decodeArray(std::string value, int &pos)
    {
        bool endFound = false;
        std::vector<IValue*> ret = std::vector<IValue*>();

        while(pos < value.length())
        {
            if (pos == 0 || value[pos] == ',')
            {
                pos ++;

                //Don't decode an empty array
                if (value[pos] != ']')
                {
                    while(value[pos] == ' ' && pos < value.length())
                    {
                        pos ++;
                    }
                    ret.push_back(Decoder::_decode(value, pos));
                }
                else
                {
                    endFound = true;
                }
            }
            else if (value[pos] == ']')
            {
                endFound = true;
                pos ++;
                break;
            }
            else if (value[pos] != ' ')
            {
                throw  Exceptions::Exception("Invalid character found when decoding array");
            }
            else
            {
                pos ++;
            }
        }

        if (!endFound)
        {
            throw  Exceptions::Exception("No end found for array");
        }

        return ret;
    }

    std::map<std::string, IValue*> Decoder::_decodeObject(std::string value, int &pos)
    {
        bool endFound = false;
        std::map<std::string, IValue*> ret = std::map<std::string, IValue*>();

        while(pos < value.length())
        {
            if ((pos == 0 || value[pos] == ',') && value[pos] != '}')
            {
                pos ++;
                //Don't try to get values of an empty object
                if (value[pos] != '}')
                {
                    while(value[pos] == ' ' && pos < value.length())
                    {
                        pos ++;
                    }

                    IValue* key = Decoder::_decode(value, pos);
                    Value::String* strKey = dynamic_cast<Value::String*>(key);

                    if(strKey != 0)
                    {
                        while((value[pos] == ' ' || value[pos] == ':')
                            && pos < value.length())
                        {
                            pos++;
                        }

                        IValue* val = Decoder::_decode(value, pos);
                        ret[strKey->getString()] = val;
                    }
                    else
                    {
                        throw  Exceptions::Exception("Object key type must be a string");
                    }
                }
                else
                {
                    endFound = true;
                }
            }
            else if (value[pos] == '}')
            {
                pos ++;
                endFound = true;
                break;
            }
            else if (value[pos] != ' ')
            {
                throw  Exceptions::Exception("Invalid character found when decoding array");
            }
            else
            {
                pos ++;
            }
        }

        if (!endFound)
        {
            throw  Exceptions::Exception("No end found for object");
        }

        return ret;
    }

    float Decoder::_decodeNumeric(std::string value, int &pos)
    {
        float ret = 0;
        std::string strNum = "";
        bool hasDecimal = false;

        if (value[pos] == '-')
        {
            strNum = "-";
            pos ++;
        }

        while(pos < value.length())
        {
            if (value[pos] == 'e' || value[pos] == 'E')
            {
                strNum += "e";
                if (value[pos + 1] == '+')
                {
                    pos ++;
                }
                else if (value[pos + 1] == '-')
                {
                    strNum += "-";
                    pos ++;
                }

                hasDecimal = true;
            }
            else if (!hasDecimal && value[pos] == '.')
            {
                strNum    += ".";
                hasDecimal = true;
            }
            else if (value[pos] >= 48 && value[pos] <= 57)
            {
                strNum  += value[pos];
            }
            else
            {
                break;
            }

            pos++;
        }

        if(EOF == sscanf(strNum.c_str(), "%f", &ret))
        {
            throw  Exceptions::Exception("An error occurred while decoding number");
        }

        return ret;
    }

    bool Decoder::_decodeBoolean(std::string value)
    {
        if (value.substr(0, 4) == "true")
        {
            return true;
        }
        else if (value.substr(0, 5) == "false")
        {
            return false;
        }

        throw  Exceptions::Exception("Was expecting true or false but got \""+value.substr(0, 5)+"\"");
    }

    void Decoder::_decodeNull(std::string value)
    {
        if (value.substr(0, 4) != "null")
        {
            throw Exceptions::Exception("Was expecting null but got \""+value.substr(0, 4)+"\"");
        }
    }
}
