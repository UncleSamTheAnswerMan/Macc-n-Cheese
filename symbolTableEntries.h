//
// Created by heyi1sam on 4/4/16.
//

#ifndef MACC_N_CHEESE_SYMBOLTABLEENTRIES_H
#define MACC_N_CHEESE_SYMBOLTABLEENTRIES_H

#include <string>
#include <vector>
using namespace std;
enum DataType{boolean, cheese, integer, floating};

class symbolTableEntries {

public:

    symbolTableEntries();
    symbolTableEntries(string incomingName, DataType type, int addr, bool hip, int numComp, int strSze);
    string getName() const;
    DataType getDataType() const;
    int getRelAddress() const;
    bool getHipHip() const;
    int getNumComponents() const;
    int getStrSize() const;

    vector<string> getInitValues();
    void setName(string n);
    void setDataType(DataType d);
    void setRelAddress(int r);
    void setHipHip(bool h);
    void setNumComponents(int n);
    void setStrSize(int s);



    void setInitVals(vector<string> i);
private:
    string name;
    DataType dataType;
    int relativeAddress;
    bool hiphip;
    int numComponents;
    int strSize;
    vector<string> initValues;
};


#endif //MACC_N_CHEESE_SYMBOLTABLEENTRIES_H
