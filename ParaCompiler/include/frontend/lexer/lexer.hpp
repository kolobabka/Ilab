#pragma once

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

#include "location.hh"

namespace yy {

struct Lexer final : public yyFlexLexer {

    yy::location location_{};

    yy::location get_location () const { return location_; }
    void set_location ();
    int yylex () override;
};

}   //namespace yy
