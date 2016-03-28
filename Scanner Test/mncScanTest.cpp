//
// Created by heyi1sam on 3/20/16.
//
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>
#include "../mncScan.h"
using namespace std;

ifstream sourceFile;
ofstream outFile, listFile, tokenFile;

void outputTokens(string fName);

int main(int argc, char* argv[]){
    DIR* dirFile = opendir(".");
    if (dirFile){
        struct dirent* hFile;
        while ((hFile = readdir(dirFile))!= NULL){
            if (!strcmp(hFile->d_name, ".") || !strcmp(hFile->d_name, "..")) {
                continue;
            }

            //if ( ( hFile->d_name[0] == '.' )) continue;

            // dirFile.name is the name of the file. Do whatever string comparison
            // you want here. Something like:
            string fileName  = hFile->d_name;
            string extension = fileName.substr(fileName.length()-4, 4);
            if (extension == ".mnc") {
                cout << "FOUND: " << fileName << endl;
                outputTokens(fileName);
            }
        }
    }
    return 0;
}

void outputTokens(string fName){
    Scanner scan;
    Token currentToken;
    string name = fName.substr(0,fName.size()-4);

    sourceFile.open(fName);
    outFile.open(name + ".asm");
    listFile.open(name + ".lst");
    tokenFile.open(name + ".tok");

    if (sourceFile.is_open()){
        do{
            currentToken = scan.GetNextToken();
            string theToken = "";
            switch (currentToken){
                case (EOF_SYM):
                    theToken = "EOF";
                    break;
                case (BOOL_SYM):
                    theToken = "BOOL_SYM";
                    break;
                case(BREAK_SYM):
                    theToken = "BREAK_SYM";
                    break;
                case(CASE_SYM):
                    theToken = "CASE_SYM";
                    break;
                case(CHEESE_SYM):
                    theToken = "CHEESE_SYM";
                    break;
                case(DECS_SYM):
                    theToken = "DECS_SYM";
                    break;
                case(DO_SYM):
                    theToken = "DO_SYM";
                    break;
                case(ELSE_SYM):
                    theToken = "ELSE_SYM";
                    break;
                case(END_SYM):
                    theToken = "END_SYM";
                    break;
                case(FALSE_SYM):
                    theToken = "FALSE_SYM";
                    break;
                case(FLOAT_SYM):
                    theToken = "FLOAT_SYM";
                    break;
                case(FOR_SYM):
                    theToken = "FOR_SYM";
                    break;
                case(HIPHIP_SYM):
                    theToken = "HIPHIP_SYM";
                    break;
                case(IF_SYM):
                    theToken = "IF_SYM";
                    break;
                case(INT_SYM):
                    theToken = "INT_SYM";
                    break;
                case(LISTEN_SYM):
                    theToken = "LISTEN_SYM";
                    break;
                case(OTHERWISE_SYM):
                    theToken = "OTHERWISE_SYM";
                    break;
                case(SELECT_SYM):
                    theToken = "SELECT_SYM";
                    break;
                case(SHOUT_SYM):
                    theToken = "SHOUT_SYM";
                    break;
                case(THEN_SYM):
                    theToken = "THEN_SYM";
                    break;
                case(TRUE_SYM):
                    theToken = "TRUE_SYM";
                    break;
                case(WHILE_SYM):
                    theToken = "WHILE_SYM";
                    break;
                case(LSTAPLE):
                    theToken = "LSTAPLE";
                    break;
                case(RSTAPLE):
                    theToken = "RSTAPLE";
                    break;
                case(LBANANA):
                    theToken = "LBANANA";
                    break;
                case(RBANANA):
                    theToken = "RBANANA";
                    break;
                case(LMUSTACHE):
                    theToken = "LMUSTACHE";
                    break;
                case(RMUSTACHE):
                    theToken = "RMUSTACHE";
                    break;
                case(COLON):
                    theToken = "COLON";
                    break;
                case(SEMICOLON):
                    theToken = "SEMICOLON";
                    break;
                case(COMMA):
                    theToken = "COMMA";
                    break;
                case(ASSIGN_OP):
                    theToken = "ASSIGN_OP";
                    break;
                case(PLUS_OP):
                    theToken = "PLUS_OP";
                    break;
                case(MINUS_OP):
                    theToken = "MINUS_OP";
                    break;
                case(MULT_OP):
                    theToken = "MULT_OP";
                    break;
                case(DIV_OP):
                    theToken = "DIV_OP";
                    break;
                case(LT_OP):
                    theToken = "LT_OP";
                    break;
                case(LE_OP):
                    theToken = "LE_OP";
                    break;
                case(GT_OP):
                    theToken = "GT_OP";
                    break;
                case(GE_OP):
                    theToken = "GE_OP";
                    break;
                case(EQ_OP1):
                    theToken = "EQ_OP1";
                    break;
                case(EQ_OP2):
                    theToken = "EQ_OP2";
                    break;
                case(NE_OP):
                    theToken = "NE_OP";
                    break;
                case(ID):
                    theToken = "ID";
                    break;
                case(INT_LIT):
                    theToken = "INT_LIT";
                    break;
                case(FLOAT_LIT):
                    theToken = "FLOAT_LIT";
                    break;
                case(CHEESE_LIT):
                    theToken = "CHEESE_LIT";
                    break;
                default:
                    theToken = "NOT A TOKEN";
            }
            tokenFile << theToken <<endl;
        }while(currentToken != EOF_SYM);
    }
    sourceFile.close();
    outFile.close();
    listFile.close();
    tokenFile.close();
}