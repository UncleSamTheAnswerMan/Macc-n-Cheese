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
    ifstream sourceFile;
    ofstream outFile, listFile, tokenFile;
    sourceFile.open(fName);
    outFile.open(name + ".asm");
    listFile.open(name + ".lst");
    tokenFile.open(name + ".tok");

    if (sourceFile.is_open()){
        do{
            currentToken = scan.GetNextToken();
            tokenFile << currentToken <<endl;
        }while(currentToken != EOF_SYM);
    }

}