%option c++
%option yylineno
%option yyclass="yy::Lexer"

%{
    #include <iostream>
    #include "parser.hpp"    
    #include "lexer.hpp"

    int yyFlexLexer::yywrap () { return 1; }
    namespace yy { 

    void Lexer::set_location () {

        location_.begin.line = location_.end.line = lineno ();
        int endCol = location_.end.column;

        for (size_t charIdx = 0; yytext[charIdx]; charIdx++) {
            char curChar = yytext[charIdx];
            switch (curChar) {
                case '\n':
                case '\v':
                case '\r':
                    location_.begin.column = location_.end.column = 1;
                    location_.end.line = lineno ();
                    break;
                case '\t':
                    location_.begin.column += 8;
                    location_.end.column += 8;
                    break;
                case ' ':
                    location_.begin.column++;
                    location_.end.column++;
                    break;
                default:
                    location_.begin.column = endCol;
                    location_.end.column = location_.begin.column + YYLeng ();
            }
        }
    }

    }// namespace yy

%}

WS                      [ \t\r\n\v]+
DIGIT                   [0-9]
ALPHA                   [a-zA-Z]
WORD                    ({ALPHA}|"_")({ALPHA}|{DIGIT}|"_")*
ONE_LINE_COMMENT        "//".*
/* TODO: multiline comments */
%%

{WS}                    {   /*skip*/    }
{ONE_LINE_COMMENT}      {   /*skip*/    }

{DIGIT}*                {   return yy::parser::token_type::NUMBER;      }

"if"                    {   return yy::parser::token_type::IF;          }
"while"                 {   return yy::parser::token_type::WHILE;       }

"print"                 {   return yy::parser::token_type::PRINT;       }

"?"                     {   return yy::parser::token_type::SCAN;        }

","                     {   return yy::parser::token_type::COMMA;       }
";"                     {   return yy::parser::token_type::SEMICOLON;   }

"("                     {   return yy::parser::token_type::OPCIRCBRACK; }
")"                     {   return yy::parser::token_type::CLCIRCBRACK; }

"{"                     {   return yy::parser::token_type::OPCURVBRACK; }
"}"                     {   return yy::parser::token_type::CLCURVBRACK; }

"+"                     {   return yy::parser::token_type::ADD;         }
"-"                     {   return yy::parser::token_type::SUB;         }
"*"                     {   return yy::parser::token_type::MUL;         }
"/"                     {   return yy::parser::token_type::DIV;         }
"%"                     {   return yy::parser::token_type::MOD;         }

"=="                    {   return yy::parser::token_type::EQ;          }
"!="                    {   return yy::parser::token_type::NEQ;         }
">"                     {   return yy::parser::token_type::MORE;        }
"<"                     {   return yy::parser::token_type::LESS;        }
">="                    {   return yy::parser::token_type::GTE;         }
"<="                    {   return yy::parser::token_type::LTE;         }

"&&"                    {   return yy::parser::token_type::AND;         }
"||"                    {   return yy::parser::token_type::OR;          }

"="                     {   return yy::parser::token_type::ASSIGN;      }

{WORD}                  {   return yy::parser::token_type::ID;          }

.                       {   return yy::parser::token_type::LEXERR;      }
%%