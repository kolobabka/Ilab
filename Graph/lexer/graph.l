%option c++

%option yylineno
%option yyclass="GraphLexer"

%{
    #include "../customLexer/customLexer.hpp"
%}

TRASH_SYMBS             [ \t\r\n\v]+
DIGIT                   [0-9]
SEPARATOR               ("-"|",")
ALPHA                   [a-zA-Z]
WORD                    ({ALPHA}|"_"|{DIGIT})({ALPHA}|{DIGIT}|"_")*

%%
{SEPARATOR}             {               }
{TRASH_SYMBS}           {   /*skip*/    }

{WORD}                  {  return pushData(yytext);}
%%