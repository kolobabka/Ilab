#ifndef CUSTOM_LEXER_H__
#define CUSTOM_LEXER_H__

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif
#include <vector>

struct GraphLexer final : public yyFlexLexer {
    std::vector<int> nums_;
    GraphLexer () = default;

    int pushNumber (const char* num) {

        nums_.push_back(std::stoi(num));
        return 1;
    }
    int yylex () override;
};


#endif