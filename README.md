# JSON PlusPlus

Json PlusPlus is JSON encoder/decoder for C++. ENcoding an object is as simple as including the library and calling the static method decode() on the JSON decoder, like so:

````c++
#include <string>
#include "src/Json/Decoder.h"
using namespace std;
int main(int argc, char* argv[]) {
    std::string jsonString = "{\"key1\":[21.0925, 1,\"randomString\"]}";
    Json::IValue* jsonObj;
    
    jsonObj = Json::Decoder::decode(jsonString);
}
````

To encode an object to JSON, simply build the object, and call the toString() method. Like so:

````c++
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include "src/Json/Decoder.h"
using namespace std;
int main(int argc, char* argv[]) {

    using namespace Json::Value;
    Object* jsonObj   = new Object();
    Array*  jsonArr   = new Array();
    Numeric* jsonNum  = new Numeric();
    Numeric* jsonNum2 = new Numeric();
    String* jsonStr   = new String();

    using namespace std;
    map<string, Json::IValue*> mapper = map<string, Json::IValue*>();
    mapper["key1"] = jsonArr;

    jsonObj->set(mapper);
    vector<Json::IValue*> arr = vector<Json::IValue*>();
    arr.push_back(jsonNum);
    arr.push_back(jsonNum2);
    arr.push_back(jsonStr);
    
    jsonArr->set(arr);
    jsonNum->set(21.0925);
    jsonNum2->set(1);
    jsonStr->set("randomString");

    cout << jsonObj->toString();
}
````

The following objects are available under the Json::Value namespace:

* Object
* Array
* Numeric
* String
* Boolean
* Null

