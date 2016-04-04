//
// Created by heyi1sam on 3/28/16.
//

#include "mncCode.h"
#include "mncScan.h"
#include <iostream>
#include <fstream>
#include <iomanip>

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

/// initializes maps for IntIDS, FloatIDs, and CHesseIDS to their value
static map<string,int> IntTable;
static map<string,float> FloatTable;
static map<string,string> CheeseTable;

void CodeGen::Shout(Expr& shoutStuff) {
    switch (shoutStuff.theType){
        case (intType):
            Generate("WRI       ", "#" + to_string(shoutStuff.intVal), "");
            break;

    }
}
///sets up the ID in there correct directory
void CodeGen::DefineVar(const ExprType type){
    switch (type){
        case (intType):
            IntTable [scan.tokenBuffer.data()] = null;
            break;
        case (floatType):
            FloatTable [scan.tokenBuffer.data()] = null;
            break;
        case (cheeseType):
            CheeseTable [scan.tokenBuffer.data()] = null;
            break;
    }


}

void CodeGen::ProcessLit(Expr& expr) {
    switch (expr.theType){
        case (floatType):
            expr.floatVal = atof(scan.tokenBuffer.data());
            break;
        case (intType):
            expr.intVal = atoi(scan.tokenBuffer.data());
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



