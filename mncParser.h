// MACC_N_CHEESE Grammar 2016
// 
// Parser Class Definition File

// SHELL GENERATED BY PARSERGEN VERSION 5.0.3

#ifndef PARSER
#define PARSER

#include <string>
using namespace std;

#include "mncScan.h" // scanner class definition

#include "mncCode.h" // code generator class definition

class Parser
{
public:

	Parser();

	void SystemGoal();

private:

	Token savedToken;
	bool  tokenAvailable;

	void SyntaxError(Token t, string msg);

	Token NextToken();

	void Match(Token t);

	void InitTail();

	void VarDecTail(ExprType& type, bool HipOrNah, int HipHip_Size, int cheeseSize);

	void VarDecList(ExprType& type, bool HipOrNah, int HipHip_Size, int cheeseSize);

	void DecTail();

	void Declaration();

	void DecList();

	void BoolLit(Expr& expr);

	void CheeseTypeTail(int& Cheese_size);

	void CheeseType(int& Cheese_size);

	void Type(ExprType& type, int& Cheese_size);

	void Literal(Expr& expr);

	void MultOp();

	void FactorTail(Expr& expr);

	void Primary(Expr& expr);

	void AddOp();

	void ExprTail(Expr& expr);

	void Factor(Expr& expr);

	void RelOp();

	void CondTail(OpRec& opRec);

	void IntListTail();

	void IntList();

	void CaseListTail();

	void Case();

	void CaseList();

	void ForAssign();

	void ElseClause(bool& isElse, string& numLbl);

	void Condition(OpRec& opRec);

	void VarDecs();

	void SelectStmt();

	void ForStmt();

	void WhileStmt();

	void LoopStmt();

	void IfStmt();

	void ItemListTail();

	void ItemList();

	void VariableTail(Expr& expr);

	void VarListTail();

	void VarList();

	void InitList();

	void Expression(Expr& expr);

	void AssignTail(Expr & assignTailExpr);

	void Variable(Expr& expr);

	void BreakStmt();

	void ShoutStmt();

	void ListenStmt();

	void AssignStmt();

	void StructStmt();

	void SimpleStmt();

	void StmtTail();

	void Statement();

	void StmtList();

	void Program();

	void SyntaxError(Token t);

}; // end of Parser class

#endif
