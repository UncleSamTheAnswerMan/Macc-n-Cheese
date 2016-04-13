//
// Created by heyi1sam on 4/4/16.
//

#include "symbolTableEntries.h"

void symbolTableEntries::symbolTableEntries() {
    name = "";
    dataType = NULL;
    relativeAddress = 0;
    hiphip = false;
    numComponents = 0;
    initValues = vector<string>();
}

string symbolTableEntries::getName() {
    return name;
}

symbolTableEntries::DataType symbolTableEntries::getDataType() {
    return dataType;
}

int symbolTableEntries::getRelAddress() {
    return relativeAddress;
}

bool symbolTableEntries::getHipHip() {
    return hiphip;
}

int symbolTableEntries::getNumComponents() {
    return numComponents;
}

vector<string> symbolTableEntries::getInitValues() {
    return initValues;
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