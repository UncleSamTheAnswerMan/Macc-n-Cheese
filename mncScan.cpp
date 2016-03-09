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
    if (tokenBuffer == "BEGIN") return BEGIN_SYM; ///get rid of this @simon
    if (tokenBuffer == "END") return END_SYM; ///get rid of this @simon
    if (tokenBuffer == "READ") return READ_SYM; /// change to listen
    if (tokenBuffer == "WRITE") return WRITE_SYM; /// change to shout
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
            return INT_LITERAL;
        }
        else if (currentChar == '"') {//string literal
            do {
                currentChar = NextChar();
                c = sourceFile.peek();
                if (currentChar == '"' && c != '"') {//" followed by not a quote
                    return STR_LITERAL;
                } else if (currentChar == '"' && c == '"') {//quote followed by quote (escaped quote)
                    currentChar = NextChar();//ignore one quote because assembly escapes with :
                }
                stringBuffer += currentChar;//add string stuff to stringBuffer

            } while (c != '\n');//go until newLine or it hits return

        }
        else if (currentChar == '(')
            return LPAREN;
        else if (currentChar == ')')
            return RPAREN;
        else if (currentChar == ';')
            return SEMICOLON;
        else if (currentChar == ',')
            return COMMA;
        else if (currentChar == '+')
        {
            BufferChar(currentChar);
            return PLUS_OP;
        }
        else if (currentChar == ':')
        if (sourceFile.peek() == '=')
        {                             // := operator
            currentChar = NextChar();
            return ASSIGN_OP;
        }
        else
            LexicalError(currentChar);
        else if (currentChar == '-')
        if (sourceFile.peek() == '-') // comment
            do  // skip comment
                currentChar = NextChar();
            while (currentChar != '\n');
        else
        {
            BufferChar(currentChar);      // minus operator
            return MINUS_OP;
        }
        else
            LexicalError(currentChar);
    } // end while
    return EOF_SYM;
}