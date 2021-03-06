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
    maxFloat = 0;
    maxString = 0;
    maxBoolShout = 0;
    ifElseEndNum = 0;
    doUntilNum = 0;
    whileNum = 0;
}

string CodeGen::getCurrentBoolShoutName() {
    string val;
    string temp = "BS";
    val = to_string(maxBoolShout);
    maxBoolShout++;
    return (temp+val);
}

string CodeGen::getCurrentEndNumber(){
    string val;
    IntToAlpha(ifElseEndNum, val);
    ifElseEndNum++;
    return (val);
}

string CodeGen::getCurrentElseNumber(){
    string val;
    IntToAlpha(ifElseEndNum, val);
    return (val);
}

string CodeGen::getCurrentDoUntilNum(){
    string val;
    IntToAlpha(doUntilNum, val);
    doUntilNum++;
    return (val);
}

string CodeGen::getCurrentWhileNum(){
    string val;
    IntToAlpha(whileNum, val);
    whileNum++;
    return val;
}

void CodeGen::WhileCond(string &whileLbl) {
    whileLbl = getCurrentWhileNum();
    Generate("LABEL    ", "WILE" + whileLbl, "");
}

void CodeGen::WhileBegin(string &whileLbl, OpRec whileOp) {
    string jumpCond = "";
    switch (whileOp.oper){
        case (LESS):
            jumpCond = "JGE    ";
            break;
        case(LESS_EQUAL):
            jumpCond = "JGT    ";
            break;
        case(EQUAL):
            jumpCond = "JNE    ";
            break;
        case (NOT_EQUAL):
            jumpCond = "JEQ    ";
            break;
        case (GREAT):
            jumpCond = "JLE    ";
            break;
        case (GREAT_EQUAL):
            jumpCond = "JLT    ";
            break;
    }
    Generate(jumpCond, "WEND" + whileLbl, "");
}

void CodeGen::WhileEnd(string &whileLbl) {
    Generate("JMP    ", "WILE" + whileLbl, "");
    Generate("LABEL    ", "WEND" + whileLbl, "");
}

void CodeGen::LoopBegin(string &startTheLoop) {
//le stuff
//gonna need a doUntilNum and function that returns as string and increments
// call fn to get number, then generate the label
    startTheLoop = getCurrentDoUntilNum();
    Generate("LABEL    ", "LOOP" + startTheLoop, "");
}

void CodeGen::LoopEnd(string &endTheLoop, OpRec loopOp) {
    string jumpCond = "";
    switch (loopOp.oper){
        case (LESS):
            jumpCond = "JGE    ";
            break;
        case(LESS_EQUAL):
            jumpCond = "JGT    ";
            break;
        case(EQUAL):
            jumpCond = "JNE    ";
            break;
        case (NOT_EQUAL):
            jumpCond = "JEQ    ";
            break;
        case (GREAT):
            jumpCond = "JLE    ";
            break;
        case (GREAT_EQUAL):
            jumpCond = "JLT    ";
            break;
    }
    Generate(jumpCond, "LOOP" + endTheLoop, "");
}

void CodeGen::HipHipIndex(Expr &hiphip, Expr &index) {
    symbolTableEntries temp;
    string val;
    hiphip.hiphipIndex = true;
//    if (index.theType == intType){
//        IntToAlpha(index.intVal, val);
//        Generate("LD    ", "R10", " #" + val);
//    } else {
        temp = symbolTable[index.tableEntryIndex];
        if (temp.getDataType() != integer){
            string errorMessage = "in the HipHip index. Value must be an integer.";
            errorOccurred(errorMessage);
        } else {
            IntToAlpha(temp.getRelAddress(), val);
            Generate("LD    ", "R10", " +" + val + "(R15)");
        }
}

void CodeGen::Shout(Expr& shoutStuff) {
    symbolTableEntries holdThis;
    string val;

    string booShout;
    switch (shoutStuff.theType){
        case (intType):
            holdThis = symbolTable[shoutStuff.tableEntryIndex];
            IntToAlpha(holdThis.getRelAddress(), val);
            Generate("WRI    ", " +" + val + "(R15)" , "");
            //Generate("WRI       ", "#" + to_string(shoutStuff.intVal), "");
            break;
        case (floatType):
            holdThis = symbolTable[shoutStuff.tableEntryIndex];
            IntToAlpha(holdThis.getRelAddress(), val);
            Generate("WRF    ", " +" + val + "(R15)" , "");
            break;
        case (boolType):
            holdThis = symbolTable[shoutStuff.tableEntryIndex];
            booShout = getCurrentBoolShoutName();
            IntToAlpha(holdThis.getRelAddress(), val);
            Generate("LD    ", "R0", " +" + val + "(R15)");
            Generate("LD    ", "R2", "#0");
            Generate("IC    ", "R0", "R2");
            Generate("JNE    ", booShout, "");
            Generate("WRST    ", "FBOOL", "");

            Generate("JMP    ", booShout + "END", "");
            Generate("LABEL    ", booShout, "");
            Generate("WRST    ", "TBOOL", "");
            Generate("LABEL    ", booShout +"END", "");
            break;
        case (cheeseType):
            holdThis = symbolTable[shoutStuff.tableEntryIndex];
            IntToAlpha(holdThis.getRelAddress(), val);
            Generate("WRST    ", " +" + val + "(R15)", "");
            break;
        case (IDType):
            holdThis = symbolTable[shoutStuff.tableEntryIndex];
            if (holdThis.getHipHip()){
                if (shoutStuff.hiphipIndex){

                    switch (holdThis.getDataType()) {// shouting a hip hip with index
                        case (integer):

                            IntToAlpha(holdThis.getRelAddress(), val);
                            Generate("LD    ", "R6", " #" + val);
                            IntToAlpha(2, val);
                            Generate("LD    ", "R8", " #" + val);
                            Generate("IM    ", "R8", "R10");
                            Generate("IA    ", "R6", "R8");
                            Generate("IA    ", "R6", " R15");
                            Generate("WRI ", "+0(R6)", "");
                            break;
                        case (floating):
                            IntToAlpha(holdThis.getRelAddress(), val);
                            Generate("LD    ", "R6", " #" + val);
                            IntToAlpha(4, val);
                            Generate("LD    ", "R8", " #" + val);
                            Generate("IM    ", "R8", "R10");
                            Generate("IA    ", "R6", "R8");
                            Generate("IA    ", "R6", "R15");
                            Generate("WRF ", "+0(R6)", "");
                            break;
                        case (cheese):
                            IntToAlpha(holdThis.getRelAddress(), val);
                            Generate("LD    ", "R6", " #" + val);
                            IntToAlpha(holdThis.getStrSize(), val);
                            Generate("LD    ", "R8", " #" + val);
                            Generate("IM    ", "R8", " R10");
                            Generate("IA    ", "R6", " R8");
                            Generate("IA    ", "R6", " R15");
                            Generate("WRST    ", "+0(R6)", "");
                            break;
                        case (boolean):
                            booShout = getCurrentBoolShoutName();
                            IntToAlpha(holdThis.getRelAddress(), val);
                            Generate("LD    ", "R6", " #" + val);
                            IntToAlpha(2, val);
                            Generate("LD    ", "R8", " #" + val);
                            Generate("IM    ", "R8", "R10");
                            Generate("IA    ", "R6", "R8");
                            Generate("IA    ", "R6", " R15");
                            Generate("LD    ", "R0", " +0(R6)");
                            Generate("LD    ", "R2", "#0");
                            Generate("IC    ", "R0", "R2");
                            Generate("JNE    ", booShout, "");
                            Generate("WRST    ", "FBOOL", "");
                            Generate("JMP    ", booShout + "END", "");
                            Generate("LABEL    ", booShout, "");
                            Generate("WRST    ", "TBOOL", "");
                            Generate("LABEL    ", booShout + "END", "");
                            break;
                    }
                } else { //hiphip with no index
                    switch (holdThis.getDataType()) {
                        case (integer):
                            for (int i = 0; i < holdThis.getNumComponents(); i++) {
                                IntToAlpha(holdThis.getRelAddress() + (2 * i), val);
                                Generate("WRI    ", " +" + val + "(R15)", "");
                                if (i != holdThis.getNumComponents()-1){
                                    Generate("WRNL", "", "");
                                }
                            }
                            break;
                        case (floating):
                            for (int i = 0; i < holdThis.getNumComponents(); i++) {
                                cout << "Write float" << endl;
                                IntToAlpha(holdThis.getRelAddress() + (4 * i), val);
                                Generate("WRF    ", " +" + val + "(R15)", "");
                                if (i != holdThis.getNumComponents()-1){
                                    Generate("WRNL", "", "");
                                }
                            }
                            break;
                        case (cheese):
                            for (int i = 0; i < holdThis.getNumComponents(); i++) {
                                IntToAlpha(holdThis.getRelAddress() + (holdThis.getStrSize() * i), val);
                                Generate("WRST    ", " +" + val + "(R15)", "");
                                if (i != holdThis.getNumComponents()-1){
                                    Generate("WRNL", "", "");
                                }
                            }
                            break;
                        case (boolean):
                            for (int i = 0; i < holdThis.getNumComponents(); i++) {
                                booShout = getCurrentBoolShoutName();
                                IntToAlpha(holdThis.getRelAddress() + (i * 2), val);
                                Generate("LD    ", "R0", " +" + val + "(R15)");
                                Generate("LD    ", "R2", "#0");
                                Generate("IC    ", "R0", "R2");
                                Generate("JNE    ", booShout, "");
                                Generate("WRST    ", "FBOOL", "");
                                Generate("JMP    ", booShout + "END", "");
                                Generate("LABEL    ", booShout, "");
                                Generate("WRST    ", "TBOOL", "");
                                Generate("LABEL    ", booShout + "END", "");
                                if (i != holdThis.getNumComponents()-1){
                                    Generate("WRNL", "", "");
                                }
                            }
                            break;
                    }
                }

            } else {
                IntToAlpha(holdThis.getRelAddress(), val);
                switch (holdThis.getDataType()) {//shouting ID that is not hiphip
                    case (integer):
                        Generate("WRI    ", " +" + val + "(R15)", "");
                        break;
                    case (floating):
                        Generate("WRF    ", " +" + val + "(R15)", "");
                        break;
                    case (cheese):
                        Generate("WRST    ", " +" + val + "(R15)", "");
                        break;
                    case (boolean):
                        booShout = getCurrentBoolShoutName();
                        IntToAlpha(holdThis.getRelAddress(), val);
                        Generate("LD    ", "R0", " +" + val + "(R15)");
                        Generate("LD    ", "R2", "#0");
                        Generate("IC    ", "R0", "R2");
                        Generate("JNE    ", booShout, "");
                        Generate("WRST    ", "FBOOL", "");
                        Generate("JMP    ", booShout + "END", "");
                        Generate("LABEL    ", booShout, "");
                        Generate("WRST    ", "TBOOL", "");
                        Generate("LABEL    ", booShout + "END", "");
                        break;
                }
            }
            break;
        default:
            break;

    }
}
///sets up the ID in there correct directory
void CodeGen::DefineVar(const ExprType type, bool HipOrNah, int HipHip_Size, int Cheese_Size) {
    symbolTableEntries temp = symbolTableEntries();
    temp.setName(scan.tokenBuffer.data());
    int Cheese_Size_Temp = 1024;
    string val;
    switch (type) {
        case (intType):
            ///set up array of ints
            if (HipOrNah) {
                temp.setDataType(integer);
                temp.setRelAddress(calcNewRelativeAddress());
                temp.setHipHip(true);
                temp.setNumComponents(HipHip_Size);
                CheckId(temp);
                for(int i = 0; i < HipHip_Size; i++){
                    Generate("LD    ", "R0", "NINT");
                    IntToAlpha((temp.getRelAddress() + (i * 2)), val);
                    Generate("STO    ", "R0", " +" + val + "(R15)");
                }
                break;
            }
            temp.setDataType(integer);
            temp.setRelAddress(calcNewRelativeAddress());
            CheckId(temp);
            Generate("LD    ", "R0", "NINT");
            IntToAlpha(temp.getRelAddress(), val);
            Generate("STO    ", "R0", " +" + val + "(R15)");
            break;
        case (floatType):
            ///set up array of floats
            if (HipOrNah) {
                temp.setDataType(floating);
                temp.setRelAddress(calcNewRelativeAddress());
                temp.setHipHip(true);
                temp.setNumComponents(HipHip_Size);
                CheckId(temp);
                for(int i = 0; i < HipHip_Size; i++){
                    Generate("LDA    ", "R0", "NREAL");
                    IntToAlpha(4, val);
                    Generate("LD    ", "R1", " #" +val);
                    IntToAlpha((temp.getRelAddress() + (i * 4)), val);
                    Generate("BKT    ", "R0", " +" + val + "(R15)");
                }
                break;
            }
            temp.setDataType(floating);
            temp.setRelAddress(calcNewRelativeAddress());
            CheckId(temp);
            Generate("LD    ", "R0", "NREAL");
            IntToAlpha(4, val);
            Generate("LD    ", "R1", " #" +val);
            IntToAlpha(temp.getRelAddress(), val);
            Generate("STO    ", "R0", " +" + val + "(R15)");
            break;
        case (boolType):
            ///set up array of float
            if (HipOrNah) {
                temp.setDataType(boolean);
                temp.setRelAddress(calcNewRelativeAddress());
                temp.setHipHip(true);
                temp.setNumComponents(HipHip_Size);
                CheckId(temp);
                for(int i = 0; i < HipHip_Size; i++){
                    Generate("LD    ", "R0", "NINT");
                    IntToAlpha((temp.getRelAddress() + (i * 2)), val);
                    Generate("STO    ", "R0", " +" + val + "(R15)");
                }
                break;
            }
            temp.setDataType(boolean);
            temp.setRelAddress(calcNewRelativeAddress());
            CheckId(temp);
            Generate("LD    ", "R0", "NINT");
            IntToAlpha(temp.getRelAddress(), val);
            Generate("STO    ", "R0", " +" + val + "(R15)");
            break;
        case (cheeseType):
            if(Cheese_Size > 0){
                Cheese_Size_Temp = Cheese_Size;
            }
            Cheese_Size_Temp++;
            if (HipOrNah){
                temp.setDataType(cheese);
                temp.setRelAddress(calcNewRelativeAddress());
                temp.setHipHip(true);
                temp.setNumComponents(HipHip_Size);
                temp.setStrSize(Cheese_Size_Temp);
                CheckId(temp);
                for(int i = 0; i < HipHip_Size; i++){
                    Generate("LD    ", "R0", "NSTRING");
                    IntToAlpha(1, val);
                    Generate("LD    ", "R1", " #" +val);
                    IntToAlpha((temp.getRelAddress() + (i * Cheese_Size_Temp)), val);
                    Generate("BKT    ", "R0", " +" + val + "(R15)");
                }
                break;
            }
            temp.setDataType(cheese);
            temp.setRelAddress(calcNewRelativeAddress());
            temp.setStrSize(Cheese_Size_Temp);
            CheckId(temp);
            Generate("LD    ", "R0", "NSTRING");
            IntToAlpha(1, val);
            Generate("LD    ", "R1", " #" +val);
            IntToAlpha(temp.getRelAddress(), val);
            Generate("BKT    ", "R0", " +" + val + "(R15)");
            break;
    }
    LookUp(temp);
}

string CodeGen::getCurrentTempName() {
    string value;
    string temp = "TEMP";
    IntToAlpha(maxTemp, value);
    temp += value;
    maxTemp++;
    return temp;

}

void CodeGen::CheckId(const symbolTableEntries &s) {
    if (!LookUp(s))
        Enter(s);
}

void CodeGen::Enter(const symbolTableEntries &s) {
    symbolTable.push_back(s);
}

void CodeGen::ExtractExpr(const Expr &e, string &s) {
    string t;
    int k, n;


}
void CodeGen::Listen(const Expr &InExpr) {
    symbolTableEntries entry = symbolTable[InExpr.tableEntryIndex];
    string address = to_string(entry.getRelAddress());
    if(entry.getHipHip()){
    //TODO error probably
    } else {
        switch (entry.getDataType()){
            case (floating):
                Generate("RDF    ", " +" + address + "(R15)", "");
                break;
            case (integer):
                Generate("RDI    ", " +" + address + "(R15)", "");
                break;
            case (cheese):
                if (entry.getStrSize() >= 1024){
                    Generate("RDST    ", " +" + address + "(R15)", "");\
            } else {
                    for (int i = 0; i < entry.getStrSize() - 1; i++) {
                        string address = to_string(entry.getRelAddress() + i);
                        Generate("RDCH    ", " +" + address + "(R15)", "");
                    }
                    Generate("RDNL    ", "", "");
                }

                break;
        }
    }
}

void CodeGen::ProcessVar(Expr &e) {
    int index = getSymbolTableIndex(e.ID);
    if (index >= 0){
        e.tableEntryIndex = index;
        e.theType = IDType;
    } else {
        string theError = "An error occurred in ProcessVar.";
        errorOccurred(theError);
    }


}

void CodeGen::ProcessLit(Expr& expr) {
    symbolTableEntries tempEntry;
    string val;

//    switch (expr.theType){
//
//        case (intType):
//            expr.intVal = atoi(scan.tokenBuffer.data());
//            break;
//        case (floatType):
//        case (cheeseType):
//        case (boolType):
            tempEntry.setName(getCurrentTempName());
            tempEntry.setRelAddress(calcNewRelativeAddress());

            if (boolType == expr.theType){
                tempEntry.setDataType(boolean);
                string fromScanner = scan.tokenBuffer;
                makeItLowerCase(fromScanner);
                expr.boolVal = fromScanner.compare("true") == 0;
                IntToAlpha(tempEntry.getRelAddress(), val);
                Generate("LD    ", "R0", (expr.boolVal) ? "#1" : "#0");
                Generate("STO    ", "R0", " +" + val + "(R15)");
            } else if (expr.theType == cheeseType){
                int tStringVal = stringValues.size();
                stringValues.push_back(scan.stringBuffer);
                tempEntry.setDataType(cheese);
                tempEntry.setStrSize(stringValues[stringValues.size() - 1].size() + 1);
                IntToAlpha(tStringVal,val);
                string tString = "STEMP" + val;
                Generate("LDA    ", "R0", tString );
                IntToAlpha(stringValues[stringValues.size() - 1].size(), val);
                Generate("LD    ", "R1", "#" + val);
                IntToAlpha(tempEntry.getRelAddress(), val);
                Generate("BKT    ", "R0", " +" + val + "(R15)");
                Generate("LDA    ", "R0", " BIGEMPT");
                Generate("LD    ", "R1", " #1");
                IntToAlpha(tempEntry.getRelAddress() + stringValues[stringValues.size() - 1].size(), val);
                Generate("BKT    ", "R0", " +" + val + "(R15)");

            } else if (expr.theType == floatType){
                int tFloatVal = floatingValues.size();
                floatingValues.push_back(atof(scan.tokenBuffer.data()));
                tempEntry.setDataType(floating);
                IntToAlpha(tFloatVal, val);
                string fString = "FTEMP" + val;
                Generate("LDA    ", "R0", fString);
                Generate("LD    ", "R1", "#4");
                IntToAlpha(tempEntry.getRelAddress(), val);
                Generate("BKT    ", "R0", " +" + val + "(R15)");
            } else if (expr.theType == intType) {
                int num = atoi(scan.tokenBuffer.data());
                tempEntry.setDataType(integer);
                IntToAlpha(num, val);
                Generate("LD    ", "R0", " #" + val);
                IntToAlpha(tempEntry.getRelAddress(), val);
                Generate("STO    ", "R0", " +" + val + "(R15)");
            }
            expr.tableEntryIndex = symbolTable.size();
            symbolTable.push_back(tempEntry);
//            break;
//        default:
//            break;
//    }
}

int CodeGen::oneOrTwo(int size) {
    if ((size % 2) == 0) {
        return 2;
    }
    return 1;
}

bool CodeGen::LookUp(const symbolTableEntries &s) {
    for (unsigned i = 0; i < symbolTable.size(); i++)
    {
        if (symbolTable[i].getName() == s.getName()){
            return true;
        }
    }
    return false;
}

int CodeGen::getSymbolTableIndex(const string s) {
    for (int i = 0; i < symbolTable.size(); i++)
    {
        if (symbolTable[i].getName().compare(s) == 0){
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

void CodeGen::IfThen(OpRec& opRec, string& numLbl) {
    //If condition isn't met either jump to the end or out of loop
    numLbl = getCurrentEndNumber();
    string jumpToEnd = "IEND" + numLbl;
    string jumpCond = "";
    switch (opRec.oper){
        case (LESS):
            jumpCond = "JGE    ";
            break;
        case(LESS_EQUAL):
            jumpCond = "JGT    ";
            break;
        case(EQUAL):
            jumpCond = "JNE    ";
            break;
        case (NOT_EQUAL):
            jumpCond = "JEQ    ";
            break;
        case (GREAT):
            jumpCond = "JLE    ";
            break;
        case (GREAT_EQUAL):
            jumpCond = "JLT    ";
            break;
    }
    Generate(jumpCond, jumpToEnd, "");
}

void CodeGen::IfElse(string& numLbl) {
    //If there is an else clause
    Generate("JMP    ", "EEND" + numLbl, "");
    Generate("LABEL    ", "ELSE" + numLbl, "");
}

void CodeGen::IfEnd(bool& isElse, string& numLbl) {
    //Codey codedly code code stuff
    Generate("JMP    ", "EEND" + numLbl, "");
    Generate("LABEL    ", "IEND" + numLbl, "");
    if (isElse){
        Generate("JMP    ", "ELSE" + numLbl, "");
    }
    Generate("LABEL    ", "EEND" + numLbl, "");
}

void CodeGen::errorOccurred(string theError) {
    cout << endl << "The compiler encountered an error." << endl <<
    "The error was located here: " << theError << endl;

    exit(1);

}

void CodeGen::SetCondition(OpRec opRec) {
    symbolTableEntries leftSide = symbolTable[opRec.leftSide.tableEntryIndex];
    string val;
    if (opRec.rightSet) {
        symbolTableEntries rightSide = symbolTable[opRec.rightSide.tableEntryIndex];
        if (leftSide.getDataType() == rightSide.getDataType()) {
            IntToAlpha(leftSide.getRelAddress(), val);
            Generate("LD    ", "R11", " +" + val + "(R15)");
            IntToAlpha(rightSide.getRelAddress(), val);
            Generate("LD    ", "R13", " +" + val + "(R15)");
            if (leftSide.getDataType() == integer) {
                Generate("IC    ", "R11", "R13");
            } else if (leftSide.getDataType() == floating) {
                IntToAlpha(leftSide.getRelAddress()+2, val);
                Generate("LD    ", "R12", " +" + val + "(R15)");
                IntToAlpha(rightSide.getRelAddress()+2, val);
                Generate("LD    ", "R14", " +" + val + "(R15)");
                Generate("FC    ", "R11", "R13");
            }
        }
    } else {
        if (leftSide.getDataType() != boolean){
            errorOccurred("Non-Boolean value used as boolean");
        } else {
            IntToAlpha(leftSide.getRelAddress(), val);
            Generate("LD    ", "R11", " +" + val + "(R15)");
            Generate("LD    ", "R13", " #1");
            Generate("IC    ", "R11", "R13");
        }
    }
}

void CodeGen::genFloatStatements() {
    for (int i = 0; i < floatingValues.size(); i++){
        Generate("LABEL    ", getCurrentFloatName(), "");
        Generate("REAL    ", to_string(floatingValues[i]), "");
    }
}

string CodeGen::getCurrentFloatName() {
    string val;
    string temp = "FTEMP";
    IntToAlpha(maxFloat, val);
    maxFloat++;

    return  (temp + val);

}

void CodeGen::genStringStatements() {
    for (int i = 0; i < stringValues.size(); i++){
        Generate("LABEL    ", getCurrentStringName(), "");
        Generate("STRING    ", "\"" + stringValues[i] + "\"", "");
    }

}

string CodeGen::getCurrentStringName() {
    string val;
    string temp = "STEMP";
    IntToAlpha(maxString, val);
    maxString++;

    return (temp + val);
}

void CodeGen::makeItLowerCase(string &doIt) {
    for (int i = 0; i < doIt.size(); i++){
        doIt[i] = (char) tolower(doIt[i]);
    }
}

void CodeGen::genSymbolTableStuff() {
    for (int i = 0; i < symbolTable.size(); i++){
        symbolTableEntries temp = symbolTable[i];
        // Generate("LABEL    ", temp.getName(), "");
        switch(temp.getDataType()){
            case(integer):

                if (temp.getHipHip()){
                    Generate("SKIP    ", to_string(temp.getNumComponents()*2) + "", "");
                } else {
                    Generate("SKIP    ", "2", "");
                }
                break;
            case(floating):
                if (temp.getHipHip()){
                    Generate("SKIP    ", to_string(temp.getNumComponents()*4) + "", "");
                } else {
                    Generate("SKIP    ", "4", "");
                }
                break;
            case(boolean):
                if (temp.getHipHip()){
                    Generate("SKIP    ", to_string(temp.getNumComponents()*4) +"", "");
                } else {
                    Generate("SKIP    ", "2", "");
                }
                break;
            case(cheese):
                if (temp.getHipHip()){
                    Generate("SKIP    ", to_string(temp.getNumComponents()*temp.getStrSize()) + "", "");
                } else {
                    Generate("SKIP    ", to_string(temp.getStrSize()) + "", "");
                }
                break;

        }
    }
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
    genSymbolTableStuff();
    Generate("LABEL    ", "NINT", "");
    Generate("INT    ", "0", "");
    Generate("LABEL    ", "NREAL", "");
    Generate("REAL    ", "0.0", "");
    Generate("LABEL    ", "NSTRING", "");
    Generate("STRING    ", "\"\\000\"", "");
    Generate("LABEL    ", "FBOOL", "");
    Generate("STRING    ", "\"FALSE\"", "");
    Generate("LABEL    ", "TBOOL", "");
    Generate("STRING    ", "\"TRUE\"", "");
    Generate("LABEL    ", "BIGEMPT", "");
    Generate("SKIP    ", "1024", "");
    genFloatStatements();
    genStringStatements();
    outFile.close();
    listFile << endl << endl;
    listFile << " _____________________________________________\n";
    listFile << " <><><><>   S Y M B O L   T A B L E   <><><><>\n"
    << endl;
    listFile << " Relative" << endl;
    listFile << " Address      Identifier" << endl;
    listFile << " --------     --------------------------------"
    << endl;
    //for (unsigned i = 0; i < symbolTable.size(); i++)
    //{
        //listFile.width(7);
        //listFile << 2*i << "       " << symbolTable[i]. << endl;
    //}
    for (int i = 0; i < symbolTable.size(); i++){
        listFile << symbolTable[i].getName() << endl;
        listFile << symbolTable[i].getDataType();
        listFile << symbolTable[i].getRelAddress() << endl;
        listFile << symbolTable[i].getHipHip() << endl;
        listFile << symbolTable[i].getNumComponents() << endl;
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
void CodeGen::Assign(Expr &Assign, Expr &AssignTail){
    symbolTableEntries A = symbolTable[Assign.tableEntryIndex]; //left of the assignment
    symbolTableEntries t = symbolTable[AssignTail.tableEntryIndex];
    string val;
    if (AssignTail.theType == intType){//working with int literal assignment
        if (A.getDataType() == integer) {
            IntToAlpha(t.getRelAddress(), val);
            Generate("LD    ", "R0", " +" + val + "(R15)");
            if (A.getHipHip()) {
                IntToAlpha(A.getRelAddress(), val);
                Generate("LD    ", "R6", " #" + val);
                IntToAlpha(2, val);
                Generate("LD    ", "R8", " #" + val);
                Generate("IM    ", "R8", "R10");
                Generate("IA    ", "R6", "R8");
                Generate("IA    ", "R6", " R15");
                Generate("STO ", "R0", " +(R6)");
            } else {
                IntToAlpha(A.getRelAddress(), val);
                Generate("STO    ", "R0", " +" + val + "(R15)");
            }
        } else {
            string theError = "Error occurred in assignment.";
            errorOccurred(theError);
        }
    } else {//other assignments

        if (t.getDataType() != A.getDataType()) {
            string theError = "Error in assignment. Data types do not match.";
            errorOccurred(theError);
        } else {

            if (A.getHipHip()) {//assign to hiphip locations
                switch (t.getDataType()) {
                    case (integer):
                        IntToAlpha(t.getRelAddress(), val);
                        Generate("LD    ", "R0", " +" + val + "(R15)");
                        IntToAlpha(A.getRelAddress(), val);
                        Generate("LD    ", "R6", " #" + val);
                        IntToAlpha(2, val);
                        Generate("LD    ", "R8", " #" + val);
                        Generate("IM    ", "R8", "R10");
                        Generate("IA    ", "R6", "R8");
                        Generate("IA    ", "R6", " R15");
                        Generate("STO ", "R0", " +(R6)");
                        break;
                    case (floating):
                        IntToAlpha(t.getRelAddress(), val);
                        Generate("LDA    ", "R0", " +" + val + "(R15)");
                        Generate("LD    ", "R1", "#4");
                        IntToAlpha(A.getRelAddress(), val);
                        Generate("LD    ", "R6", " #" + val);
                        IntToAlpha(4, val);
                        Generate("LD    ", "R8", " #" + val);
                        Generate("IM    ", "R8", "R10");
                        Generate("IA    ", "R6", "R8");
                        Generate("IA    ", "R6", " R15");
                        Generate("BKT ", "R0", " +(R6)");
                        break;
                    case (cheese):
                        IntToAlpha((t.getRelAddress()), val);
                        Generate("LDA    ", "R0", " +" + val + "(R15)");
                        if ((A.getStrSize() - 1) < (t.getStrSize() - 1)) {
                            IntToAlpha(A.getStrSize() - 1, val);
                        } else {
                            IntToAlpha(t.getStrSize() - 1, val);//if broken change this and cheese lit in processLit
                        }
                        Generate("LD    ", "R1", "#" + val);
                        IntToAlpha(A.getStrSize(), val);
                        Generate("LD    ", "R8", " #" + val);
                        IntToAlpha(A.getRelAddress(), val);
                        Generate("LD    ", "R6", " #" + val);
                        Generate("IM    ", "R8", "R10");
                        Generate("IA    ", "R6", "R8");
                        Generate("IA    ", "R6", " R15");
                        Generate("BKT    ", "R0", " +0(R6)");
                        Generate("LDA    ", "R0", "BIGEMPT");
                        if ((A.getStrSize()) > (t.getStrSize())) {
                            IntToAlpha(((A.getStrSize()) - (t.getStrSize())), val);
                            Generate("LD    ", "R1", "#" + val);
                            IntToAlpha((t.getStrSize() - 1), val);
                            Generate("BKT    ", "R0", " +" + val + "(R6)");
                        } else {
                            Generate("LD    ", "R1", " #1");
                            IntToAlpha(A.getStrSize() - 1, val);
                            Generate("BKT    ", "R0", " +" + val + "(R6)");
                        }
                        break;
                    case (boolean):
                        IntToAlpha(t.getRelAddress(), val);
                        Generate("LD    ", "R0", " +" + val + "(R15)");
                        IntToAlpha(A.getRelAddress(), val);
                        Generate("LD    ", "R6", " #" + val);
                        IntToAlpha(2, val);
                        Generate("LD    ", "R8", " #" + val);
                        Generate("IM    ", "R8", "R10");
                        Generate("IA    ", "R6", "R8");
                        Generate("IA    ", "R6", " R15");
                        Generate("STO ", "R0", " +(R6)");
                        break;
                }

            }else {//assign to not hiphip locations
                switch (t.getDataType()) {
                    case (integer):
                        IntToAlpha(t.getRelAddress(), val);
                        Generate("LD    ", "R0", " +" + val + "(R15)");
                        IntToAlpha(A.getRelAddress(), val);
                        Generate("STO ", "R0", " +" + val + "(R15)");
                        break;
                    case (floating):
                        IntToAlpha(t.getRelAddress(), val);
                        Generate("LDA    ", "R0", " +" + val + "(R15)");
                        Generate("LD    ", "R1", "#4");
                        IntToAlpha(A.getRelAddress(), val);
                        Generate("BKT    ", "R0", " +" + val + "(R15)");
                        break;
                    case (cheese):
                        IntToAlpha((t.getRelAddress()), val);
                        Generate("LDA    ", "R0", " +" + val + "(R15)");
                        if (A.getStrSize() < t.getStrSize()) {
                            IntToAlpha(A.getStrSize() - 1, val);
                        } else {
                            IntToAlpha(t.getStrSize() - 1, val);
                        }
                        Generate("LD    ", "R1", "#" + val);
                        IntToAlpha(A.getRelAddress(), val);
                        Generate("BKT    ", "R0", " +" + val + "(R15)");
                        Generate("LDA    ", "R0", "BIGEMPT");
                        if (A.getStrSize() > t.getStrSize()) {
                            IntToAlpha((A.getStrSize() - t.getStrSize()), val);
                            Generate("LD    ", "R1", "#" + val);
                            IntToAlpha(A.getRelAddress() + t.getStrSize(), val);
                            Generate("BKT    ", "R0", " +" + val + "(R15)");
                        }
                        break;
                    case (boolean):
                        IntToAlpha(t.getRelAddress(), val);
                        Generate("LD    ", "R0", " +" + val + "(R15)");
                        IntToAlpha(A.getRelAddress(), val);
                        Generate("STO ", "R0", " +" + val + "(R15)");
                        break;
                }
            }
        }
    } //right of the assignment

}

void CodeGen::ProcessOp(OpRec& op)
{
    if(scan.tokenBuffer == "+"){op.oper = PLUS;}
    else if(scan.tokenBuffer == "-")
    {
        op.oper = MINUS;
    }
    else if(scan.tokenBuffer == "*")
    {
        op.oper =MULT;
    }
    else if(scan.tokenBuffer == "/")
    {
        op.oper =DIV;
    }
    else if(scan.tokenBuffer == "<")
    {
        op.oper = LESS;
    }
    else if(scan.tokenBuffer == "<=")
    {
        op.oper = LESS_EQUAL;
    }
    else if(scan.tokenBuffer == ">")
    {
        op.oper = GREAT;
    }
    else if(scan.tokenBuffer == ">=")
    {
        op.oper = GREAT_EQUAL;
    }
    else if(scan.tokenBuffer == "==" || scan.tokenBuffer == "!!")
    {
        op.oper = EQUAL;
    }
    else if(scan.tokenBuffer == "!=")
    {
        op.oper = NOT_EQUAL;
    }
}

void CodeGen::GenInfix(OpRec op, Expr& result){
    symbolTableEntries leftside = symbolTable[op.leftSide.tableEntryIndex];
    symbolTableEntries rightside = symbolTable[op.rightSide.tableEntryIndex];
    symbolTableEntries mathResult;

  ///Check for the same type
    if(leftside.getDataType() != rightside.getDataType()){
        errorOccurred("GenInfix non matching types");
    } else {
        string val;
        mathResult.setDataType(leftside.getDataType());
        mathResult.setName(getCurrentTempName());
        mathResult.setHipHip(false);
        mathResult.setRelAddress(calcNewRelativeAddress());
        symbolTable.push_back(mathResult);
        result.tableEntryIndex = symbolTable.size() - 1;
        if (leftside.getDataType() == integer){
            IntToAlpha(leftside.getRelAddress(), val);
            Generate("LD ", "R0", " +" + val + "(R15)");
        } else {
            IntToAlpha(leftside.getRelAddress(), val);
            Generate("LD ", "R0", " +" + val + "(R15)");
            IntToAlpha(leftside.getRelAddress() + 2, val);
            Generate("LD ", "R1", " +" + val + "(R15)");
        }
        if (rightside.getDataType() == integer){
            IntToAlpha(rightside.getRelAddress(), val);
            Generate("LD ", "R2", " +" + val + "(R15)");
        } else {
            IntToAlpha(rightside.getRelAddress(), val);
            Generate("LD ", "R2", " +" + val + "(R15)");
            IntToAlpha(rightside.getRelAddress() + 2, val);
            Generate("LD ", "R3", " +" + val + "(R15)");
        }
        switch (op.oper) {
            case (PLUS):
                if (op.leftSide.theType == intType || leftside.getDataType() == integer) {
                    Generate("IA ", "R0", "R2");
                } else {
                    Generate("FA ", "R0", "R2");
                }
                break;
            case (MINUS):
                if (op.leftSide.theType == intType || leftside.getDataType() == integer) {
                    Generate("IS ", "R0", "R2");
                } else {
                    Generate("FS ", "R0", "R2");
                }
                break;
            case (MULT):
                if (op.leftSide.theType == intType || leftside.getDataType() == integer) {
                    Generate("IM ", "R0", "R2");
                } else {
                    Generate("FM ", "R0", "R2");
                }
                break;
            case (DIV):
                if (op.leftSide.theType == intType || leftside.getDataType() == integer) {
                    Generate("ID ", "R0", "R2");
                } else {
                    Generate("FD ", "R0", "R2");
                }
                break;
        }
        IntToAlpha(mathResult.getRelAddress(), val);
        Generate("STO    ", "R0", " +" + val + "(R15)");
        if(leftside.getDataType() != integer){
            IntToAlpha(mathResult.getRelAddress() + 2, val);
            Generate("STO     ", "R1", " +" + val + "(R15)");
        }
    }

}
