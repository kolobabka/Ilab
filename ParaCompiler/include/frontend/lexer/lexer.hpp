#pragma once

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

#include "common/locator.hpp"

namespace yy {

struct Lexer final : public Locator, public yyFlexLexer {

    void set_location ();
    int yylex () override;
};

}   //namespace yy
