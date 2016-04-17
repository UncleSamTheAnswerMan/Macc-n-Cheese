//
// Created by heyi1sam on 4/4/16.
//

#include "symbolTableEntries.h"

symbolTableEntries::symbolTableEntries() {
    name = "";
    dataType = NULL;
    relativeAddress = 0;
    hiphip = false;
    numComponents = 0;
    initValues = vector<string>();
}

symbolTableEntries::symbolTableEntries(string incomingName, DataType type, int addr, bool hip, int numComp, int strSze) :
name(incomingName), dataType(type), relativeAddress(addr), hiphip(hip), numComponents(numComp), strSize(strSze) {

}

string symbolTableEntries::getName() const {
    return name;
}

DataType symbolTableEntries::getDataType() const {
    return dataType;
}

int symbolTableEntries::getRelAddress() const {
    return relativeAddress;
}

bool symbolTableEntries::getHipHip() const {
    return hiphip;
}

int symbolTableEntries::getNumComponents() const {
    return numComponents;
}

vector<string> symbolTableEntries::getInitValues() const {
    return initValues;
}

int symbolTableEntries::getStrSize() const {
    return strSize;
}

void symbolTableEntries::setName(string n) {
    name = n;
}

void symbolTableEntries::setDataType(DataType d) {
    dataType = d;
}

void symbolTableEntries::setHipHip(bool h) {
    hiphip = h;
}

void symbolTableEntries::setNumComponents(int n) {
    numComponents = n;
}

void symbolTableEntries::setInitVals(vector<string> i) {
    initValues = i;
}

void symbolTableEntries::setRelAddress(int r) {
    relativeAddress = r;
}

void symbolTableEntries::setStrSize(int s) {
    strSize = s;
}