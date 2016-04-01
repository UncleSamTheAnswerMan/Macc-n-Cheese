//
// Created by heyi1sam on 3/28/16.
//

#include "mncCode.h"
#include "mncScan.h"

extern Scanner scan;


void CodeGen::Shout(Expr& shoutStuff) {
    
}
void CodeGen::DefineVar(const ExprType type){
    string ASMDecString = "";


}

void CodeGen::ProcessLit(Expr& expr) {
    switch (expr.theType){
        case (floatType):
            expr.floatVal = atof(scan.tokenBuffer);
            break;
        case (intType):
            expr.theType = atoi(scan.tokenBuffer);
            break;
        case (cheeseType):
            expr.cheeseVal = scan.stringBuffer;
            break;
    }
}

void CodeGen::IntToAlpha(int val, string& str)
{
    int k;
    char temp;

    str = "";
    if (val == 0) str = "0";
    while (val > 0)
    {
        str.append(1, (char)(val % 10 + (int)'0'));
        val /= 10;
    }
    k = int(str.length());
    for (int i = 0; i < k/2; i++)
    {
        temp = str[i];
        str[i] = str[k-i-1];
        str[k-i-1] = temp;
    }
}