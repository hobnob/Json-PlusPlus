#include <string>
#include <iostream>

#include "src/Json/Decoder.h"
using namespace std;
int main(int argc, char* argv[]) {
    
    std::string jsonString = "{\"key1\":[21.0925, 1,\"randomString\",{\"randomObj\":[1]}],\"key2\":{},\"key3\":[null, true, false, -2.1E-10]}";
    Json::IValue* jsonObj;
    
    jsonObj = Json::Decoder::decode(jsonString);
    cout << jsonObj->toString();
    return 0;
}