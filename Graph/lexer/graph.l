%option c++

%option yylineno
%option yyclass="GraphLexer"

%{
    #include "../customLexer/customLexer.hpp"
%}

TRASH_SYMBS             [ \t\r\n\v]+
DIGIT                   [0-9]
DIGIT1                  [1-9]
WORD                    ("-"|",")

%%
{WORD}                    {               }
{TRASH_SYMBS}           {   /*skip*/    }

{DIGIT1}{DIGIT}*        {   return pushNumber(yytext);  }
%%