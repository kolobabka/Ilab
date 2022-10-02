%language "c++"
%skeleton "lalr1.cc"

%{
    #include "parser.hpp"
    #include <string>
%}

%defines
%define api.value.type variant
%param {yy::Parser* pars}
%param {Tree::NAryTree<AST::Node*> &tree}


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

    parser::token_type yylex(parser::semantic_type* yylval, parser::location_type* location, Parser *pars, Tree::NAryTree<AST::Node*> &tree);

    } //namespace yy

    namespace {

    AST::OperNode* makeUnaryOperNode (AST::OperNode::OperType type, AST::Node* child, yy::location loc) {

        AST::OperNode* newOperNode = new AST::OperNode (type, loc);
        newOperNode->addChild (child);
        return newOperNode;
    }

    AST::OperNode* makeBinOperNode (AST::OperNode::OperType type, 
                                    AST::Node* firstChild, AST::Node* secondChild, yy::location loc) {

        AST::OperNode* newOperNode = new AST::OperNode (type, loc);
        newOperNode->addChild (firstChild);
        newOperNode->addChild (secondChild);
        return newOperNode;
    }

    AST::CondNode* makeCondNode (AST::CondNode::ConditionType type, AST::Node* expr, 
                                 AST::Node* body, yy::location loc) {

        AST::CondNode* newCondNode = new AST::CondNode (type, loc);
        newCondNode->addChild (expr);
        newCondNode->addChild (body);
        return newCondNode;
    }

    AST::OperNode* makeAssign (const std::string &firstChild, AST::Node* secondChild, 
                               yy::location assignLoc, yy::location varLoc) {
        AST::OperNode* newNode  = new AST::OperNode (AST::OperNode::OperType::ASSIGN, assignLoc);
        newNode->addChild (new AST::VarNode (firstChild, varLoc));
        newNode->addChild (secondChild);
        return newNode;
    }

    } //anonymous namespace 
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

/* AST TREE */
%type <AST::Node*>                  assignStatement
%type <AST::Node*>                  orStatement
%type <AST::Node*>                  andStatement
%type <AST::Node*>                  eqStatement
%type <AST::Node*>                  cmpStatement
%type <AST::Node*>                  addStatement
%type <AST::Node*>                  mulStatement
%type <AST::Node*>                  unaryStatement

%type <AST::Node*>                  term
%type <AST::Node*>                  atomic

%type <AST::Node*>                  assignment

%type <AST::Node*>                  statement

%type <AST::Node*>                  ifStatement
%type <AST::Node*>                  whileStatement

%type <AST::Node*>                  body

%type <AST::Node*>                  conditionExpression

%type <AST::Node*>                  printStatement

%type <std::vector<AST::Node*>*>    statementHandler

%start translationStart

%%

translationStart            :   statementHandler                {
                                                                    AST::ScopeNode* globalScope = new AST::ScopeNode (@1, nullptr);
                                                                    if ($1) {
                                                                        for (auto curStmtNode: *($1))             
                                                                        {   
                                                                            if (!curStmtNode)
                                                                                continue;
                                                                            
                                                                            globalScope->addChild (curStmtNode);
                                                                        }
                        
                                                                        delete $1;
                                                                    }
                                                                    tree.setRoot (globalScope);
                                                                    tree.dump(std::cout);
                                                                };

statementHandler            :   statement                       {
                                                                    if ($1) {
                                                                        $$ = new std::vector<AST::Node*>;
                                                                        $$->push_back ($1);
                                                                    } else
                                                                        $$ = nullptr;
                                                                }
                            |   statementHandler statement      {
                                                                    if ($1 && $2) {
                                                                        $1->push_back ($2);
                                                                        $$ = $1;
                                                                    } else {
                                                                        $$ = nullptr;
                                                                        if ($1) {
                                                                            for (auto v: *($1))
                                                                                delete v;
                                                                        }
                                                                        delete $1;
                                                                        delete $2;
                                                                    }
                                                                };


statement                   :   assignment                      {   $$ = $1;    }
                            |   ifStatement                     {   $$ = $1;    }
                            |   whileStatement                  {   $$ = $1;    }
                            |   orStatement SEMICOLON           {   $$ = $1;    }
                            |   printStatement                  {   $$ = $1;    }
                            |   error SEMICOLON                 {   pars->push_error (@1, "Undefined statement");  $$ = nullptr;   }
                            |   error END                       {   pars->push_error (@1, "Undefined statement");  $$ = nullptr;   };

printStatement              :   PRINT assignStatement SEMICOLON {   $$ = makeUnaryOperNode (AST::OperNode::OperType::PRINT, $2, @1);     }
                            |   PRINT error SEMICOLON           {   pars->push_error (@2, "Undefined expression in print");    $$ = nullptr;   }
                            |   PRINT error END                 {   pars->push_error (@2, "Undefined expression in print");    $$ = nullptr;   };

ifStatement                 :   IF conditionExpression body     {
                                                                    if ($2 && $3) {
                                                                        $$ = makeCondNode (AST::CondNode::ConditionType::IF, $2, $3, @1);
                                                                    } else {
                                                                        $$ = nullptr;
                                                                        delete $2;
                                                                        delete $3;
                                                                    }
                                                                }
                            |   IF conditionExpression statement {
                                                                    if ($2 && $3) {
                                                                        AST::ScopeNode* newScope = new AST::ScopeNode (@3, nullptr);
                                                                        newScope->addChild ($3);
                                                                        $$ = makeCondNode (AST::CondNode::ConditionType::IF, $2, newScope, @1);
                                                                    } else {
                                                                        $$ = nullptr;
                                                                        delete $2;
                                                                        delete $3;
                                                                    }
                                                                };

whileStatement              :   WHILE conditionExpression body  {
                                                                    if ($2 && $3) {
                                                                        $$ = makeCondNode (AST::CondNode::ConditionType::WHILE, $2, $3, @1);
                                                                    } else {
                                                                        $$ = nullptr;
                                                                        delete $2;
                                                                        delete $3;
                                                                    }
                                                                }
                            |   WHILE conditionExpression statement
                                                                {
                                                                   if ($2 && $3) {
                                                                        AST::ScopeNode* newScope = new AST::ScopeNode (@3, nullptr);
                                                                        newScope->addChild ($3);
                                                                        $$ = makeCondNode (AST::CondNode::ConditionType::WHILE, $2, newScope, @1);
                                                                    } else {
                                                                        $$ = nullptr;
                                                                        delete $2;
                                                                        delete $3;
                                                                    } 
                                                                };

conditionExpression         :   OPCIRCBRACK assignStatement CLCIRCBRACK   
                                                                {  std::cout << "assign" << std::endl; $$ = $2;    }
                            |   OPCIRCBRACK error CLCIRCBRACK   {   pars->push_error (@2, "Bad expression for condition");   $$ = nullptr;   };


body                        :   OPCURVBRACK statementHandler CLCURVBRACK 
                                                                {
                                                                    AST::ScopeNode* newScope = new AST::ScopeNode (@1);
                                                                    if ($2) {
                                                                        for (auto curStmtNode: *($2))
                                                                           newScope->addChild (curStmtNode);
                                                                    }
                                                                    delete $2;
                                                                    $$ = newScope;
                                                                }
                            |   OPCURVBRACK CLCURVBRACK         {   $$ = new AST::ScopeNode (@1);   };


assignment                  :   ID ASSIGN assignStatement SEMICOLON       
                                                                {   $$ = makeAssign ($1, $3, @2, @1);   }
                            |   ID ASSIGN error SEMICOLON       {   pars->push_error (@3, "Bad expression after assignment");  
                                                                    $$ = nullptr;   
                                                                }
                            |   ID ASSIGN   body                {   $$ = makeAssign ($1, $3, @2, @1);  }
                            |   ID ASSIGN   body SEMICOLON      {   $$ = makeAssign ($1, $3, @2, @1);  }
                            |   ID error SEMICOLON              {   pars->push_error (@1, "Unexpected operation with variable");   
                                                                    $$ = nullptr;   
                                                                }
                            |   error ASSIGN assignStatement SEMICOLON 
                                                                {   
                                                                    pars->push_error (@1, "rvalue can't become lvalue"); 
                                                                    $$ = nullptr; 
                                                                };

assignStatement             :   orStatement                     {   $$ = $1;        }
                            |   ID ASSIGN assignStatement       {   $$ = makeAssign ($1, $3, @2, @1);  };

orStatement                 :   andStatement                    {   $$ = $1;        }
                            |   orStatement OR andStatement     {   $$ = makeBinOperNode (AST::OperNode::OperType::OR, $1, $3, @2);     };


andStatement                :   eqStatement                     {   $$ = $1;        }
                            |   andStatement AND eqStatement    {   $$ = makeBinOperNode (AST::OperNode::OperType::AND, $1, $3, @2);    };

eqStatement                 :   cmpStatement                    {   $$ = $1;        }
                            |   eqStatement EQ cmpStatement     {   $$ = makeBinOperNode (AST::OperNode::OperType::EQ, $1, $3, @2);     }
                            |   eqStatement NEQ cmpStatement    {   $$ = makeBinOperNode (AST::OperNode::OperType::NEQ, $1, $3, @2);    };

cmpStatement                :   addStatement                    {   $$ = $1;        }
                            |   cmpStatement MORE addStatement  {   $$ = makeBinOperNode (AST::OperNode::OperType::MORE, $1, $3, @2);  }
                            |   cmpStatement LESS addStatement  {   $$ = makeBinOperNode (AST::OperNode::OperType::LESS, $1, $3, @2);  }
                            |   cmpStatement GTE addStatement   {   $$ = makeBinOperNode (AST::OperNode::OperType::GTE, $1, $3, @2);   }
                            |   cmpStatement LTE addStatement   {   $$ = makeBinOperNode (AST::OperNode::OperType::LTE, $1, $3, @2);   };

addStatement                :   mulStatement                    {   $$ = $1;        }
                            |   addStatement ADD mulStatement   {   $$ = makeBinOperNode (AST::OperNode::OperType::ADD, $1, $3, @2);   }
                            |   addStatement SUB mulStatement   {   $$ = makeBinOperNode (AST::OperNode::OperType::SUB, $1, $3, @2);   };

mulStatement                :   unaryStatement                  {   $$ = $1;        }
                            |   mulStatement MUL unaryStatement {   $$ = makeBinOperNode (AST::OperNode::OperType::MUL, $1, $3, @2);   }
                            |   mulStatement DIV unaryStatement {   $$ = makeBinOperNode (AST::OperNode::OperType::DIV, $1, $3, @2);   }
                            |   mulStatement MOD unaryStatement {   $$ = makeBinOperNode (AST::OperNode::OperType::MOD, $1, $3, @2);   };

unaryStatement              :   term                            {   $$ = $1;        }
                            |   SUB unaryStatement              {   $$ = makeUnaryOperNode (AST::OperNode::OperType::UNARY_M, $2, @1); }
                            |   ADD unaryStatement              {   $$ = makeUnaryOperNode (AST::OperNode::OperType::UNARY_P, $2, @1); };

term                        :   atomic                          {   $$ = $1;    }
                            |   OPCIRCBRACK assignStatement CLCIRCBRACK
                                                                {   $$ = $2;    };

atomic                      :   NUMBER                          {   $$ = new AST::NumNode   ($1);                                   }
                            |   SCAN                            {   $$ = new AST::OperNode  (AST::OperNode::OperType::SCAN, @1);    }
                            |   ID                              {   $$ = new AST::VarNode   ($1, @1);                               };

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
    
    Tree::NAryTree<AST::Node*> tree;
    parser parser (this, tree);
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

    *location = lexer_->location_;

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


parser::token_type yylex (parser::semantic_type* yylval, parser::location_type* location, Parser* pars, Tree::NAryTree<AST::Node*> &tree) {
    
    try {
        return pars->yylex (yylval, location);
    } catch (std::runtime_error& err) {
        throw err; 
    }
}

void parser::error (const parser::location_type& location, const std::string& what) {
    /* DO NOTHING */
}

}//namespace yy