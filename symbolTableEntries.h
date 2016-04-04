//
// Created by heyi1sam on 4/4/16.
//

#ifndef MACC_N_CHEESE_SYMBOLTABLEENTRIES_H
#define MACC_N_CHEESE_SYMBOLTABLEENTRIES_H

#include <string>
#include <vector>
using namespace std;

class symbolTableEntries {
public:
    enum DataType{boolean, cheese, integer, floating};

    symbolTableEntries();
private:
    string name;
    DataType dataType;
    int relativeAddress;
    bool hiphip;
    int numComponents;
    vector<string> initValues;
};


#endif //MACC_N_CHEESE_SYMBOLTABLEENTRIES_H
