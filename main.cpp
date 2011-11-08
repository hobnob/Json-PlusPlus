#include <string>
#include <regex.h>
#include <stdio.h>
#include <vector>
#include <map>
 #include <stdexcept>

#include <iostream>

namespace Json
{
    class Exception : public std::runtime_error {
        public:
            Exception(std::string const& s) : std::runtime_error(s)
            { }
    };

    class Value
    {
        public:
            Value(){};
            virtual ~Value() {}
            virtual void set(std::string val){};
            
            virtual std::string getString(){return"";};
            virtual bool getBoolean(){return false;};
            
            virtual std::string toString(){};
    };
    
    class ValueString : public Value
    {
        private:
            std::string _val;
        public:
            ValueString() : Value()
            {
                
            }
            
            void set(std::string val)
            {
                this->_val = val;
            }
            
            std::string getString()
            {
                return this->_val;
            }
            
            std::string toString()
            {
                return "\""+this->_val+"\"";
            }
    };
    
    class ValueArray : public Value
    {
        private:
           std::vector<Value*> _val;

        public:
            void set(std::vector<Value*> val)
            {
                this->_val = val;
            };
            
            std::string toString()
            {
                std::string ret = "[";
                
                for(int i = 0; i < this->_val.size(); i++)
                {
                    if (i != 0)
                    {
                        ret += ",";
                    }
                    
                    ret += this->_val.at(i)->toString();
                }
                
                ret += "]";
                return ret;
            }
    };
    
    class ValueObject : public Value
    {
        private:
            std::map<std::string, Value*> _val;
        public:
            
            void set( std::map<std::string, Value*> val)
            {
                this->_val = val;
            }
            
            std::string toString()
            {
                std::string ret = "{";
                
                
                typedef std::map<std::string, Value*> MapType;
                MapType data = this->_val;
                MapType::const_iterator end = data.end(); 
                
                for (MapType::const_iterator it = data.begin(); it != end; ++it) {
                    if (it != data.begin())
                    {
                        ret += ",";
                    }

                    ret += "\""+it->first+"\":"+it->second->toString();
                }
                ret += "}";
                
                return ret;
            }
    };
    
    class ValueNumeric : public Value
    {
        private:
            float _val;

        public:
            void set(float val)
            {
                this->_val = val;
            }

            std::string toString()
            {
                char ret[100];
                if ((int) this->_val == this->_val)
                {
                    sprintf(ret, "%d", (int) this->_val);
                }
                else
                {
                    sprintf(ret, "%g", this->_val);
                }
                
                return ret;
            }
    };
    
    class ValueBoolean : public Value
    {
        private:
            bool _val;
        
        public:
            void set(bool val)
            {
                this->_val = val;
            }

            bool getBoolean()
            {
                return this->_val;
            }

            std::string toString()
            {
                if (this->_val)
                {
                    return "true";
                }
                else
                {
                    return "false";
                }
            }
    };
    
    class ValueNull : public Value
    {
        public:
            std::string toString()
            {
                return "null";
            }
    };
    
    class Decoder
    {
        public:
            static Value* decode(std::string value)
            {
                int pos = 0;
                Value* ret = Decoder::_decode(value, pos);
                return ret;
            }
        private:
            static Value* _decode(std::string value, int &pos)
            {
                Value* json;
                std::string str = value.substr(pos);
                if (str.length() > 0)
                {
                    switch(str[0])
                    {
                        case '{':
                        {
                            int startPos = 0;
                            json = new ValueObject();
                            (dynamic_cast<ValueObject*>(json))->set(Decoder::_decodeObject(str, startPos));
                            
                            pos += startPos;
                        }
                            break;
                        case '"':
                            json = new ValueString();
                            json->set(Decoder::_decodeString(str));
                            pos  += (json->getString().length() + 2);
                            break;
                        case '[':
                        {
                            int startPos = 0;
                            json = new ValueArray();
                            (dynamic_cast<ValueArray*>(json))->set(Decoder::_decodeArray(str, startPos));
                            
                            pos += startPos;
                        }
                            break;
                        case 't':
                        case 'f':
                            json = new ValueBoolean();
                            (dynamic_cast<ValueBoolean*>(json))->set(Decoder::_decodeBoolean(str));
                            
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
                            json = new ValueNull();
                            Decoder::_decodeNull(str);
                            pos += 4;
                            break;
                            
                        default:
                        {
                            int startPos = 0;
                            json = new ValueNumeric();
                            (dynamic_cast<ValueNumeric*>(json))->set(Decoder::_decodeNumeric(str, startPos));
                            
                            pos += startPos;
                        }
                            
                    }
                }
                return json;
            }
            
            static std::string _decodeString(std::string value)
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
                                    throw Exception("Invalid unicode expression");
                                }
                            }
                                break;
                            default:
                                throw Exception("Invalid escape option selected");
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
                
                throw Exception("No end of string found");
            }
            
            static std::vector<Value*> _decodeArray(std::string value, int &pos)
            {
                bool endFound = false;
                std::vector<Value*> ret = std::vector<Value*>();
                
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
                        throw Exception("Invalid character found when decoding array");
                    }
                    else
                    {
                        pos ++;
                    }
                }
                
                if (!endFound)
                {
                    throw Exception("No end found for array");
                }
                
                return ret;
            }
            
            static std::map<std::string, Value*> _decodeObject(std::string value, int &pos)
            {
                bool endFound = false;
                std::map<std::string, Value*> ret = std::map<std::string, Value*>();
                
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

                            Value* key = Decoder::_decode(value, pos);
                            ValueString* strKey = dynamic_cast<ValueString*>(key);

                            if(strKey != 0)
                            {
                                while((value[pos] == ' ' || value[pos] == ':')
                                    && pos < value.length())
                                {
                                    pos++;
                                }

                                Value* val = Decoder::_decode(value, pos);
                                ret[strKey->getString()] = val;
                            }
                            else
                            {
                                throw Exception("Object key type must be a string");
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
                        throw Exception("Invalid character found when decoding array");
                    }
                    else
                    {
                        pos ++;
                    }
                }
                
                if (!endFound)
                {
                    throw Exception("No end found for object");
                }
                
                return ret;
            }
            
            static float _decodeNumeric(std::string value, int &pos)
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
                    throw Exception("An error occurred while decoding number");
                }

                return ret;
            }
            
            static bool _decodeBoolean(std::string value)
            {
                if (value.substr(0, 4) == "true")
                {
                    return true;
                }
                else if (value.substr(0, 5) == "false")
                {
                    return false;
                }
                
                throw Exception("Was expecting true or false but got \""+value.substr(0, 5)+"\"");
            }
            
            static void _decodeNull(std::string value)
            {
                if (value.substr(0, 4) != "null")
                {
                    throw Exception("Was expecting null but got \""+value.substr(0, 4)+"\"");
                }
            }
    };
    
}


using namespace std;
int main(int argc, char* argv[]) {
    //game::Game * game = new game::Game();
 
    //return game->onExecute();
    
    std::string jsonString = "{\"key1\":[21.09246, 1,\"randomString\",{\"randomObj\":[1]}],\"key2\":{},\"key3\":[null, true, false, -2.1E-10]}";
    Json::Value* jsonObj;
    
    jsonObj = Json::Decoder::decode(jsonString);
    cout << jsonObj->toString();
    return 0;
}