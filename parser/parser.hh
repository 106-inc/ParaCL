#ifndef PARACL_PARSER_HH
#define PARACL_PARSER_HH

#include <iostream>
#include <FlexLexer.h>
//! #include "numgrammar.tab.hh" - auto generated file from bison


namespace yy {

    class OurFlexLexer : public yyFlexLexer {
    private:

        yy::location cur_location_;
        std::string cur_str_;

    public:
        OurFLexLexer();


        yy::location get_cur_location();

        std::string get_cur_str_();


        void yylex() override;

    };
}





#endif // PARACL_PARSER_HH