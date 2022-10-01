%language "c++"
%skeleton "lalr1.cc"

%{
    #include "parser.hpp"
    #include <string>
%}

%defines
%define api.value.type variant
%param {yy::Parser* driver}

%code requires
{
    #include <iostream>
    #include <string>
    #include "parser.hpp"

    namespace yy {class Parser;}
}

%code
{
    extern int yylineno;
    #include <string>
    #include "parser.hpp"

    namespace yy {

    parser::token_type yylex(parser::semantic_type* yylval, parser::location_type* location, Parser *driver);
    }
}

%locations

%token <int>                NUMBER
%token <std::string>        ID

%token                      ASSIGN      "="

%token                      ADD         "+"
%token                      SUB         "-"
%token                      MUL         "*"
%token                      DIV         "/"
%token                      MOD         "%"

%token                      EQ          "=="
%token                      NEQ         "!="
%token                      MORE        ">"
%token                      LESS        "<"
%token                      GTE         ">="
%token                      LTE         "<="

%token                      AND         "&&"
%token                      OR          "||"

%token                      IF          "if"

%token                      WHILE       "while"
%token                      PRINT       "print"

%token                      OPCIRCBRACK "("
%token                      CLCIRCBRACK ")"

%token                      OPCURVBRACK "{"
%token                      CLCURVBRACK "}"

%token                      COMMA       ","
%token                      SEMICOLON   ";"

%token                      SCAN        "?"

%token                      END         0   "end of file"

%token                      LEXERR

%left '+' '-' '*' '/' '%'
%right '='

%%

program:      stms                    {}
stms:         END                     {}

%%


namespace yy {

Parser::Parser (const char *input) : lexer_ (std::unique_ptr<Lexer>{new Lexer}) {

    std::fstream inputFile (input, std::ios_base::in);
    while (inputFile) {
        std::string newLine;
        std::getline (inputFile, newLine);
        code_.push_back (newLine);
    }
}


bool Parser::parse () {

    parser parser (this);
    bool res = parser.parse ();
    return !res;
}

parser::token_type Parser::yylex (parser::semantic_type *yylval, parser::location_type *location) {

    parser::token_type tokenT = static_cast<parser::token_type> (lexer_->yylex ());

    switch (tokenT) {
        //!TODO try catch
        case yy::parser::token_type::NUMBER: {
            yylval->build<int> () = std::stoi (lexer_->YYText ());
            break;
        }
        case yy::parser::token_type::ID: {
            yylval->build<std::string> () = lexer_->YYText ();
            break;
        }
        case yy::parser::token_type::LEXERR: {
            throw std::runtime_error ("Unexpected word");
        }
    }

    *location = lexer_->get_location ();

    return tokenT;
}

void Parser::push_error (yy::location curLocation, const std::string &err)
{
    std::string errPos = std::string ("#") + std::to_string (curLocation.begin.line) + std::string (", ") + std::to_string (curLocation.begin.column) + std::string (": ");
    std::string errMsg = err + std::string (": ");
    std::string codePart = code_[curLocation.begin.line - 1];

    std::string underLine ("\n");
    underLine.insert (1, curLocation.begin.column + errPos.size () + errMsg.size (), '~');
    underLine += std::string ("^");

    error_.push_back (errPos + errMsg + codePart + underLine);
}


parser::token_type yylex (parser::semantic_type* yylval, parser::location_type* location, Parser* parser) {
    
    try {
        return parser->yylex (yylval, location);
    } catch (std::runtime_error& err) {
        throw err; 
    }
}

void parser::error (const parser::location_type& location, const std::string& what) {
    /* DO NOTHING */
}

}//namespace yy