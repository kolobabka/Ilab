#ifndef CUSTOM_LEXER_H__
#define CUSTOM_LEXER_H__

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif
#include <vector>
#include <string>
#include <utility>

struct GraphLexer final : public yyFlexLexer {

    std::vector<std::string> data_;

    GraphLexer () = default;

    int pushData (const char* data) {
        data_.emplace_back(data);
        return 1;
    }

    int yylex () override;
};


#endif