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
enum OpKind {PLUS, MINUS, MULT, DIV, LESS, LESS_EQUAL, GREAT, GREAT_EQUAL, EQUAL, NOT_EQUAL};

struct Expr {
    ExprType theType;
    int tableEntryIndex;
    int intVal;
    float floatVal;
    bool boolVal;
    string cheeseVal;
    string ID;
    bool hiphipIndex=false;
};

struct OpRec {
    Expr leftSide;
    Expr rightSide;
    OpKind oper;
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

    void HipHipIndex(Expr& hiphip, Expr& index);

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

    void Listen(const Expr& InExpr);
    //Produces the assembly code for reading a value for InExpr.

    void Start();
    // Initializes the compiler.

    void DefineVar(const ExprType type, bool HipOrNah, int HipHip_Size, int Cheese_Size);

    void ProcessOp(OpRec& op);
    //Boolean expression evaluation

    void IfThen(OpRec& opRec);
    //Produces the assembly code to handle an if-then statement

    void IfEnd(bool& isElse);
    //Produces the assembly code to handle the end of an if statement

    void IfElse();
    //Produces the assembly code for if there is an else statement

    void GenInfix(OpRec op, Expr& result);

    void Assign(Expr &Assign, Expr &AssignTail);
    ///Takes the variable and the value it is assigned and generates the code for assembly.

    static void makeItLowerCase(string& doIt);

    void SetCondition(OpRec opRec);

private:
    vector<symbolTableEntries> symbolTable;

    vector<float> floatingValues;
    vector<string> stringValues;

    //vector<string> intSymbolTable;

    int strSum;


    int maxTemp;     // max temporary allocated so far; initially 0
    int maxFloat;
    int maxString;
    int maxBoolShout;
    int ifElseEndNum;

//    static map<string,int> HipHiplength;
//    static map<string,int> Cheeselength;
//    ///
//
//    vector<string> HipHipSymbolTable;
//    vector<string> boolSymbolTable;
//    vector<string> CheeseSymbolTable;
    //vector<string> floatSymbolTable;

    int oneOrTwo (int size);
    // Makes a string representation for a positive integer val.
    void IntToAlpha(int val, string& str);

    void genSymbolTableStuff();

    int calcNewRelativeAddress();

    string getCurrentTempName();

    int getSymbolTableIndex(const string s);

    void genFloatStatements();

    string getCurrentFloatName();

    string getCurrentBoolShoutName();

    void genStringStatements();

    string getCurrentStringName();

    void createSymbolTableEntry(Expr& expr);//creates a symbolTableEntries object and places into symbolTable

    string getCurrentEndNumber(); //For jumping to the end of if statements

    string getCurrentElseNumber();


    void CheckId(const symbolTableEntries & s);
//     Declares s as a new variable and enters it into the symbol table when s
//     is not already in the symbol table.

    void Enter(const symbolTableEntries & s);
    // Enters s unconditionally into the symbol table.

    void ExtractExpr(const Expr & e, string& s);
    // Returns an operand representation s for the expression e.

    void Generate(const string & s1, const string & s2, const string & s3);
    // Produces the SAM assembly code for one or two operand instructions.
    // s1 is the opcode; s2 and s3 are operands.

    //string GetTemp();
    // Creates a temporary variable and returns its name.

    bool LookUp(const symbolTableEntries &s);
    // Returns true if s is in the symbol table; otherwise,
    // false is returned.

    //int strSize(const string findSize);//returns size of string in words, including null bytes





};

#endif //MACC_N_CHEESE_MNCCODE_H
