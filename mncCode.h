//
// Created by heyi1sam on 3/28/16.
//

#ifndef MACC_N_CHEESE_MNCCODE_H
#define MACC_N_CHEESE_MNCCODE_H
#include <string>

using namespace std;

enum ExprType {floatType, intType, cheeseType, boolType, IDType};

struct Expr {
    ExprType theType;
    int intVal;
    float floatVal;
    bool boolVal;
    string cheeseVal;
    string ID
};

class CodeGen{
public:
    void Shout(Expr& shoutStuff);

    void ProcessLit(Expr& expr);

private:
    void IntToAlpha(int val, string& str);
    // Makes a string representation for a positive integer val.
};

#endif //MACC_N_CHEESE_MNCCODE_H
