//
// Created by heyi1sam on 3/28/16.
//

#ifndef MACC_N_CHEESE_MNCCODE_H
#define MACC_N_CHEESE_MNCCODE_H
#include <string>
#include <vector>
#include <map>
#include "symbolTableEntries.h"
using namespace std;

enum ExprType {floatType, intType, cheeseType, boolType, IDType, TempType, hiphipType};

struct Expr {
    ExprType theType;
    int tableEntryIndex;
    int intVal;
    float floatVal;
    bool boolVal;
    string cheeseVal;
    string ID;
};

class CodeGen{
public:
    void Shout(Expr& shoutStuff);

    void ProcessLit(Expr& expr);

    CodeGen();
    // Initializes the code generator;

/* _____________________________________________________________________________
*/

    //void Assign(const ExprRec & target, const ExprRec & source);
    // Produces the assembly code for an assignment from Source to Target.

    void Finish();
    // Generates code to finish the program.

    //void GenInfix(const ExprRec & e1, const OpRec & op,
    //              const ExprRec & e2, ExprRec& e);
    // Produces the assembly code for an infix operation.

    void NewLine();
    // Produces the assembly code for starting a new output line.

    void ProcessVar(Expr& e);
    // Declares the variable in the token buffer and builds a
    // corresponding semantic record e.

    //void ProcessLiteral(ExprRec& e);
    // Converts the literal found in the token buffer into numeric form
    // and builds a corresponding semantic record e.

    //void ProcessOp(OpRec& o);
    // Produces an operator descriptor O for the operator in the token
    // buffer.

    void Listen(const Expr& InExpr);
    //Produces the assembly code for reading a value for InExpr.

    void Start();
    // Initializes the compiler.

    void DefineVar(const ExprType type, bool HipOrNah, int HipHip_Size, int Cheese_Size);

    void ProcessOp();
    //Boolean expression evaluation

private:
    void IntToAlpha(int val, string& str);
    // Makes a string representation for a positive integer val.

    vector<symbolTableEntries> symbolTable;

    //vector<string> intSymbolTable;
    //vector<string> floatSymbolTable;
//    vector<string> CheeseSymbolTable;
//    vector<string> boolSymbolTable;
//    vector<string> HipHipSymbolTable;
//
//    ///
//    static map<string,int> Cheeselength;
//    static map<string,int> HipHiplength;


    int strSum;

    int maxTemp;     // max temporary allocated so far; initially 0

    int calcNewRelativeAddress();

    string getCurrentTempName();

    int getSymbolTableIndex(const string s);

    void createSymbolTableEntry(Expr& expr);//creates a symbolTableEntries object and places into symbolTable


    void CheckId(const symbolTableEntries & s);
//     Declares s as a new variable and enters it into the symbol table when s
//     is not already in the symbol table.

    void Enter(const symbolTableEntries & s);
    // Enters s unconditionally into the symbol table.

    void ExtractExpr(const Expr & e, string& s);
    // Returns an operand representation s for the expression e.

    //string ExtractOp(const OpRec& o);
    // Returns a representation for the operator o.

    void Generate(const string & s1, const string & s2, const string & s3);
    // Produces the SAM assembly code for one or two operand instructions.
    // s1 is the opcode; s2 and s3 are operands.

    //string GetTemp();
    // Creates a temporary variable and returns its name.

    bool LookUp(const symbolTableEntries &s);
    // Returns true if s is in the symbol table; otherwise,
    // false is returned.

    //int strSize(const string findSize);//returns size of string in words, including null bytes

    void Assign(Expr &Assign, Expr &AssignTail);
    ///Takes the variable and the value it is assigned and generates the code for assembly.

};

#endif //MACC_N_CHEESE_MNCCODE_H
