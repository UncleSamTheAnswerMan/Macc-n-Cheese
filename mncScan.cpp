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

string Scanner::MakeItUp(string s)
{
    string tempString = string(s);
    for (int i = 0; i  < tempString.size(); i++) {
        tempString[i] = (char)toupper(tempString[i]);
    }

    return tempString;
}

void Scanner::BufferChar(char c)
{
    if (tokenBuffer.length() < ID_STRING_LEN)
        tokenBuffer += c;
}

Token Scanner::CheckReserved()
{
    string upperTemp = MakeItUp(tokenBuffer);
    if (upperTemp == "BOOL"){return BOOL_SYM;}
    if (upperTemp == "BREAK"){return BREAK_SYM;}
    if (upperTemp == "CASE"){return CASE_SYM;}
    if (upperTemp == "CHEESE"){return CHEESE_SYM;}
    if (upperTemp == "DECS"){return DECS_SYM;}
    if (upperTemp == "DO"){return DO_SYM;}
    if (upperTemp == "ELSE"){return ELSE_SYM;}
    if (upperTemp == "END"){return END_SYM;}
    if (upperTemp == "FALSE"){return FALSE_SYM;}
    if (upperTemp == "FLOAT"){return FLOAT_SYM;}
    if (upperTemp == "FOR"){return FOR_SYM;}
    if (upperTemp == "HIPHIP"){return HIPHIP_SYM;}
    if (upperTemp == "IF"){return IF_SYM;}
    if (upperTemp == "INT"){return INT_SYM;}
    if (upperTemp == "LISTEN"){return LISTEN_SYM;}
    if (upperTemp == "OTHERWISE"){return OTHERWISE_SYM;}
    if (upperTemp == "SELECT"){return SELECT_SYM;}
    if (upperTemp == "SHOUT"){return SHOUT_SYM;}
    if (upperTemp == "THEN"){return THEN_SYM;}
    if (upperTemp == "TRUE"){return TRUE_SYM;}
    if (upperTemp == "WHILE"){return WHILE_SYM;}
    if (upperTemp == "UNTIL"){return UNTIL_SYM;}
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
        {
            BufferChar(currentChar);
            c = sourceFile.peek();
            int decimalCount = 0;
            int minusPlusECount = 0;
            //a float must have exactly one period - HMH
            while ((isdigit(c) && decimalCount <= 1) || (c == '.' && decimalCount < 1) || (c == 'e' || c == 'E') || (c == '-' || c == '+')){
                if (c == '.')
                {
                    decimalCount++;
                }
                else if (c == 'e' || c == 'E'){
                    minusPlusECount++;
                }
                currentChar = NextChar();
                BufferChar(currentChar);
                c = sourceFile.peek();

            }

            if (decimalCount == 0 && minusPlusECount == 0)
            {
                return INT_LIT;
            }
            else if ((minusPlusECount > 0) || decimalCount > 0)
            {
                return FLOAT_LIT;
            }
        }

        else if (currentChar == '"') {//string literal
            do {
                //cout << stringBuffer << endl;
                currentChar = NextChar();
                c = sourceFile.peek();
                if (currentChar == '"' && c != '"') {//" followed by not a quote
                    return CHEESE_LIT;
                } else if (currentChar == '\\') {//quote followed by quote (escaped quote)
                    stringBuffer += ':';
                    currentChar = NextChar();
                    c = sourceFile.peek();
                    if(currentChar == '\\'){
                        stringBuffer += '\\';
                        //currentChar = NextChar();
                    }
                    else if (currentChar == '"'){
                        stringBuffer += '"';
                        //currentChar = NextChar();
                    }
                    else if (currentChar == 'n'){
                        stringBuffer += "010";
                        //currentChar = NextChar();
                    }
                }
                else if (currentChar == ':'){
                    stringBuffer += ':';
                    stringBuffer += currentChar;
                }
                else {
                    stringBuffer += currentChar;
                }//add string stuff to stringBuffer

            } while (c != '\n');//go until newLine or it hits return
        }
        else if (currentChar == '('){
            return LBANANA;
        }
        else if (currentChar == ')'){
            return RBANANA;
        }
        else if (currentChar == ';'){
            return SEMICOLON;
        }
        else if (currentChar == ','){
            return COMMA;
        }
        else if (currentChar == '{'){
            return LMUSTACHE;
        }
        else if (currentChar == '}'){
            return RMUSTACHE;
        }
        else if (currentChar == '['){
            return LSTAPLE;
        }
        else if (currentChar == ']'){
            return RSTAPLE;
        }
        else if (currentChar == ':'){
            return COLON;
        }
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
            c = sourceFile.peek();
            if (c == '/') {
                while (currentChar != '\n') {
                    currentChar = NextChar();
                }
            } else if (c==':') {
                currentChar = NextChar();
                currentChar = NextChar();
                c = sourceFile.peek();
                while (!(currentChar == ':' && c == '/')){
                    currentChar = NextChar();
                    c = sourceFile.peek();
                }
                currentChar = NextChar();
                currentChar = NextChar();
            } else {
                BufferChar(currentChar);
                return DIV_OP;
            }
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