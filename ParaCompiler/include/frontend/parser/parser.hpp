#pragma once

#include <fstream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include "lexer.hpp"
#include "grammar_tab.hh"

namespace yy {

class Parser final {
    std::unique_ptr<Lexer> lexer_;
    std::vector<std::string> error_;
    std::vector<std::string> code_;

public:
    Parser (const char *input);

    parser::token_type yylex (parser::semantic_type *yylval, parser::location_type *location);

    int get_lineno () const noexcept { return lexer_->lineno (); }

    std::string get_YYText () const noexcept { return lexer_->YYText (); }

    void push_error (yy::location curLocation, const std::string &err);

    std::vector<std::string>::const_iterator err_begin () const { return error_.cbegin (); }

    std::vector<std::string>::const_iterator err_end () const   { return error_.cend ();   }

    bool parse ();
};

}  // namespace yy
