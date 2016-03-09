//
// Created by heyi1sam on 3/7/16.
//
/*	____________________________________________________________________________

	         Scanner Component Implementation for the Micro Compiler

	                               mscan.cpp

	                              Version 2007

	                           James L. Richards
	                     Last Update: August 28, 2007

	The routines in this unit are based on those provided in the book
	"Crafting A Compiler" by Charles N. Fischer and Richard J. LeBlanc, Jr.,
	Benjamin Cummings Publishing Co. (1991).

	See Section 2.2, pp. 25-29.
	____________________________________________________________________________
*/
#include "mncScan.h"

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

extern ifstream sourceFile;
extern ofstream outFile, listFile;


// *******************
// **  Constructor  **
// *******************

Scanner::Scanner()
{
    tokenBuffer = "";
    lineBuffer = "";
    lineNumber = 0;
    stringBuffer = "";
}

// ********************************
// **  Private Member Functions  **
// ********************************

void Scanner::BufferChar(char c)
{
    if (tokenBuffer.length() < ID_STRING_LEN)
        tokenBuffer += toupper(c);
}

Token Scanner::CheckReserved()
{
    if (tolower(tokenBuffer) == "bool"){return BOOL_SYM;}
    if (tolower(tokenBuffer) == "break"){return BREAK_SYM;}
    if (tolower(tokenBuffer) == "case"){return CASE_SYM;}
    if (tolower(tokenBuffer) == "cheese"){return CHEESE_SYM;}
    if (tolower(tokenBuffer) == "decs"){return DECS_SYM;}
    if (tolower(tokenBuffer) == "do"){return DO_SYM;}
    if (tolower(tokenBuffer) == "else"){return ELSE_SYM;}
    if (tolower(tokenBuffer) == "end"){return END_SYM;}
    if (tolower(tokenBuffer) == "false"){return FALSE_SYM;}
    if (tolower(tokenBuffer) == "float"){return FLOAT_SYM;}
    if (tolower(tokenBuffer) == "for"){return FOR_SYM;}
    if (tolower(tokenBuffer) == "hiphip"){return HIPHIP_SYM;}
    if (tolower(tokenBuffer) == "if"){return IF_SYM;}
    if (tolower(tokenBuffer) == "int"){return INT_SYM;}
    if (tolower(tokenBuffer) == "listen"){return LISTEN_SYM;}
    if (tolower(tokenBuffer) == "otherwise"){return OTHERWISE_SYM;}
    if (tolower(tokenBuffer) == "select"){return SELECT_SYM;}
    if (tolower(tokenBuffer) == "shout"){return SHOUT_SYM;}
    if (tolower(tokenBuffer) == "then"){return THEN_SYM;}
    if (tolower(tokenBuffer) == "true"){return TRUE_SYM;}
    if (tolower(tokenBuffer) == "while"){return WHILE_SYM;}
    return ID;

}

void Scanner::ClearBuffer()
{
    tokenBuffer = "";
    stringBuffer = "";//need thing to hold strings that isn't limited to 32 characters
}

void Scanner::LexicalError(char& c)
{
    cout << " *** Lexical Error: '" << c
    << "' ignored at position " << int(lineBuffer.size())
    << " on line #" << lineNumber+1 << '.' << endl;
    listFile << " *** Lexical Error: '" << c
    << "' ignored at position " << int(lineBuffer.size())
    << " on line #" << lineNumber+1 << '.' << endl;
    c = NextChar();
}

char Scanner::NextChar()
{
    char c;

    sourceFile.get(c);
    if (c == '\n')
    {
        listFile.width(6);
        listFile << ++lineNumber << "  " << lineBuffer << endl;
        lineBuffer = "";
    }
    else
        lineBuffer += c;
    return c;
}

// *******************************
// **  Public Member Functions  **
// *******************************

Token Scanner::GetNextToken()
{
    char currentChar, c;

    ClearBuffer();
    currentChar = NextChar();
    while (!sourceFile.eof())
    {
        if (isspace(currentChar))
            currentChar = NextChar();     // do nothing
        else if (isalpha(currentChar))
        {                                // identifier
            BufferChar(currentChar);
            c = sourceFile.peek();
            while (isalnum(c) || c == '_')
            {
                currentChar = NextChar();
                BufferChar(currentChar);
                c = sourceFile.peek();
            }
            return CheckReserved();
        }
        else if (isdigit(currentChar))
        {                                // integer literal
            BufferChar(currentChar);
            c = sourceFile.peek();
            while (isdigit(c))
            {
                currentChar = NextChar();
                BufferChar(currentChar);
                c = sourceFile.peek();
            }
            return INT_LIT;
        }
        else if (isdigit(currentChar))
        {
            BufferChar(currentChar);
            c = sourceFile.peek();
            int decimalCount = 0; //a float must have exactly one period - HMH
            while ((isdigit(c) && decimalCount <= 1) || (c == '.' && decimalCount < 1)){
                if (c == '.')
                {
                    decimalCount++;
                }
                currentChar = NextChar();
                BufferChar(currentChar);
                c = sourceFile.peek();

            }
            return FLOAT_LIT;
        }
        else if (currentChar == '"') {//string literal
            do {
                currentChar = NextChar();
                c = sourceFile.peek();
                if (currentChar == '"' && c != '"') {//" followed by not a quote
                    return CHEESE_LIT;
                } else if (currentChar == '"' && c == '"') {//quote followed by quote (escaped quote)
                    currentChar = NextChar();//ignore one quote because assembly escapes with :
                }
                stringBuffer += currentChar;//add string stuff to stringBuffer

            } while (c != '\n');//go until newLine or it hits return

        }
        else if (currentChar == '(')
            return LBANANA;
        else if (currentChar == ')')
            return RBANANA;
        else if (currentChar == ';')
            return SEMICOLON;
        else if (currentChar == ',')
            return COMMA;
        else if (currentChar == '+')
        {
            BufferChar(currentChar);
            return PLUS_OP;
        }
        else if (currentChar == '-'){
            BufferChar(currentChar);
            return MINUS_OP;
        }
        else if (currentChar == '*'){
            BufferChar(currentChar);
            return MULT_OP;
        }
        else if (currentChar == '/'){
            BufferChar(currentChar);
            return DIV_OP;
        }
        else if (currentChar == '<'){
            BufferChar(currentChar);
            c = sourceFile.peek();
            if (c == '='){// <=
                currentChar = NextChar();
                BufferChar(currentChar);
                return LE_OP;
            }
            return LT_OP;
        }
        else if (currentChar == '>'){
            BufferChar(currentChar);
            c = sourceFile.peek();
            if (c == '='){// >=
                currentChar = NextChar();
                BufferChar(currentChar);
                return GE_OP;
            }
            return GT_OP;
        }
        else if (currentChar == '=') {
            BufferChar(currentChar);
            c = sourceFile.peek();
            if (c == '=') {// == operator
                currentChar = NextChar();
                BufferChar(currentChar);
                return EQ_OP1;
            }
            return ASSIGN_OP;
        }
        else if (currentChar == '!'){
            BufferChar(currentChar);
            c = sourceFile.peek();
            if (c == '!') { //!!
                currentChar = NextChar();
                BufferChar(currentChar);
                return EQ_OP2;
            }
            else if (c == '='){
                currentChar = NextChar();
                BufferChar(currentChar);
                return NE_OP;
            }
            else {
                LexicalError(currentChar);
            }
        }
        else
            LexicalError(currentChar);
    } // end while
    return EOF_SYM;
}