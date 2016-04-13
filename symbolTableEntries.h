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

    symbolTableEntries();

    enum DataType{boolean, cheese, integer, floating};
    string getName();
    DataType getDataType();
    int getRelAddress();
    bool getHipHip();
    int getNumComponents();

    vector<string> getInitValues();
    void setName(string n);
    void setDataType(DataType d);
    void setRelAddress(int r);
    void setHipHip(bool h);
    void setNumComponents(int n);



    void setInitVals(vector<string> i);
private:
    string name;
    DataType dataType;
    int relativeAddress;
    bool hiphip;
    int numComponents;
    vector<string> initValues;
};


#endif //MACC_N_CHEESE_SYMBOLTABLEENTRIES_H
