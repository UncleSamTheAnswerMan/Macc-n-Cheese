//
// Created by heyi1sam on 3/28/16.
//

#include "symbolTableEntries.h"
#include "mncCode.h"
#include "mncScan.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>


extern Scanner scan;
extern ifstream sourceFile;
extern ofstream outFile, listFile;


// *******************
// **  Constructor  **
// *******************

CodeGen::CodeGen()
{
    maxTemp = 0;
}



void CodeGen::Shout(Expr& shoutStuff) {
    switch (shoutStuff.theType){
        case (intType):
            Generate("WRI       ", "#" + to_string(shoutStuff.intVal), "");
            break;
        case (floatType):

            Generate("WRF       ", "#" + to_string(shoutStuff.floatVal), "");
            break;
        case (boolType):
            //Generate(("WRI      ", ));
            break;
        case (cheeseType):

            break;
        case (IDType):

            break;
        default:
            break;

    }
}
///sets up the ID in there correct directory
void CodeGen::DefineVar(const ExprType type, bool HipOrNah) {
    symbolTableEntries temp = symbolTableEntries();
    temp.setName(scan.tokenBuffer.data());
    switch (type) {
        case (intType):
            if (HipOrNah == true) {
                temp.setDataType(integer);
                temp.setRelAddress(calcNewRelativeAddress());
                temp.setHipHip(true);
                temp.setNumComponents(//need temp  int var);
                break;
            }
            temp.setDataType(integer);
            temp.setRelAddress(calcNewRelativeAddress());
            break;
        case (floatType):
            temp.setDataType(floating);
            temp.setRelAddress(calcNewRelativeAddress());
            break;
        case (boolType):
            temp.setDataType(boolean);
            temp.setRelAddress(calcNewRelativeAddress());
            break;
        case (cheeseType):
            temp.setDataType(cheese);
            temp.setRelAddress(calcNewRelativeAddress());
            break;
    }


//        case (cheeseType):
//            CheeseSymbolTable.push_back(scan.tokenBuffer.data());
//            Cheeselength [scan.tokenBuffer.data()] = null;
//            break;
//        case (boolType):
//            boolSymbolTable.push_back(scan.tokenBuffer.data());
//            break;
//        case (Hip):
//    }
}

string CodeGen::getCurrentTempName() {
    string value;
    string temp = "TEMP";
    IntToAlpha(maxTemp, value);
    temp += value;
    maxTemp++;
    return temp;

}

void CodeGen::CheckId(const string &s) {
    if (!LookUp(s))
        Enter(s);
}

void CodeGen::Enter(const string &s) {
    symbolTable.push_back(s);
}

void CodeGen::ExtractExpr(const Expr &e, string &s) {
    string t;
    int k, n;

    switch(e.theType)
    {
        case IDType:
        case
    }
}
void CodeGen::Listen(const Expr &InExpr) {
    string s;

    ExtractExpr(InExpr, s);
    Generate("RDI       ", s, "");
}

void CodeGen::ProcessVar(Expr &e) {
    string varName = scan.tokenBuffer;
    int index = getSymbolTableIndex(varName);
    if (index >= 0){
        e.ID = varName;
        e.tableEntryIndex = index;
    } else {

    }


}

void CodeGen::ProcessLit(Expr& expr) {
    symbolTableEntries tempEntry;
    switch (expr.theType){
        case (floatType):
            expr.floatVal = (float) atof(scan.tokenBuffer.data());
            break;
        case (intType):
            expr.intVal = atoi(scan.tokenBuffer.data());
            break;
        case (cheeseType):
            expr.cheeseVal = scan.stringBuffer;
            break;
    }
}

bool CodeGen::LookUp(const string &s) {
    for (unsigned i = 0; i < symbolTable.size(); i++)
    {
        if (symbolTable[i].getName() == s){
            return true;
        }
    }
    return false;
}

int CodeGen::getSymbolTableIndex(const string s) {
    for (unsigned i = 0; i < symbolTable.size(); i++)
    {
        if (symbolTable[i].getName() == s){
            return i;
        }
    }
    return -1;
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

void CodeGen::Generate(const string & s1, const string & s2, const string & s3)
{
    listFile.width(20);
    listFile << ' ' << s1;
    outFile << s1;
    if (s2.length() > 0)
    {
        listFile << s2;
        outFile << s2;
        if (s3.length() > 0)
        {
            listFile << ',' << s3;
            outFile << ',' << s3;
        }
    }
    listFile << endl;
    outFile << endl;
}

int CodeGen::calcNewRelativeAddress() {
    int relAddress = 0;
    vector<symbolTableEntries>::iterator iter;
    for (iter = symbolTable.begin(); iter < symbolTable.end(); ++iter){
        switch(iter->getDataType()){
            case (boolean):
            case (integer):
                if (iter->getHipHip()){
                    relAddress += (iter->getNumComponents() *2);
                } else {
                    relAddress+=2;
                }
                break;
            case (floating):
                if (iter->getHipHip()){
                    relAddress += (iter->getNumComponents() *4);
                } else {
                    relAddress+=4;
                }
                break;
            case (cheese):
                if (iter->getHipHip()) {
                    int size = iter->getStrSize();
                    if (size % 2 == 1) {
                        relAddress += (iter->getNumComponents() * (size + 1));
                    } else {
                        relAddress += (iter->getNumComponents() * (size + 2));
                    }
                } else {
                    int size = iter->getStrSize();
                    if (size % 2 == 1) {
                        relAddress += size + 1;
                    } else {
                        relAddress += size + 2;
                    }
                }
                break;
        }
    }

    return relAddress;
}


// ******************************
// ** Public Member Functions  **
// ******************************

void CodeGen::Finish()
{
    string s;

    listFile.width(6);
    listFile << ++scan.lineNumber << "  " << scan.lineBuffer << endl;
    Generate("HALT      ", "", "");
    Generate("LABEL     ", "VARS", "");
    IntToAlpha(int(2*(symbolTable.size()+1)), s);
    /*Generate("SKIP      ", s, "");
    if (!stringTable.empty()) {//go through all the strings
        Generate("LABEL     ", "STRINGS", "");//need a label
        for (int i = 0; i < stringTable.size(); i++) {
            Generate("STRING    ", "\"" + stringTable[i] + "\"", "");
        }
    }*/
    outFile.close();
    listFile << endl << endl;
    listFile << " _____________________________________________\n";
    listFile << " <><><><>   S Y M B O L   T A B L E   <><><><>\n"
    << endl;
    listFile << " Relative" << endl;
    listFile << " Address      Identifier" << endl;
    listFile << " --------     --------------------------------"
    << endl;
    for (unsigned i = 0; i < symbolTable.size(); i++)
    {
        listFile.width(7);
        listFile << 2*i << "       " << symbolTable[i] << endl;
    }
    listFile << " _____________________________________________"
    << endl;
    listFile << endl;
    /*listFile << "<><><><><> ALL THE STRINGS <><><><><>" << endl;
    if (!stringTable.empty()) {//printing stringTable to listFile with relevant info
        int runSum = 0;
        for (int i = 0; i < stringTable.size(); i++) {
            listFile << stringTable[i] << endl;
            listFile << "Length (including null bytes): " << strSize(stringTable[i]) << endl;
            listFile << "Offset from R14: " << runSum;
            runSum += strSize(stringTable[i]);
        }
    }*/
    listFile << "______________________________________________";
    listFile << " Normal successful compilation." << endl;
    listFile.close();
}

void CodeGen::NewLine()
{
    Generate("WRNL      ", "", "");
}

void CodeGen::Start()
{
    Generate("LDA       ", "R15", "VARS");
    //Generate("LDA       ", "R14", "STRINGS");//load address of strings
}



