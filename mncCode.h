//
// Created by heyi1sam on 3/28/16.
//

#ifndef MACC_N_CHEESE_MNCCODE_H
#define MACC_N_CHEESE_MNCCODE_H
#include <string>

using namespace std;

enum shoutType {floatType, intType, stringType};

struct ShoutExpr {
    shoutType theType;
    int intVal;
    string stringVal;
    float floatVal;
};

class CodeGen{
public:
    void Shout(ShoutExpr shoutStuff);

    void ProcessLit();

private:

};

#endif //MACC_N_CHEESE_MNCCODE_H
