#ifndef PARACL_PARSER_HH
#define PARACL_PARSER_HH

#include <iostream>

#include <FlexLexer.h>

#include "INode.hh"
#include "../flex+bison/compiler.tab.hh"

namespace yy
{

    class OurFlexLexer : public yyFlexLexer
            {
    private:

        yy::location cur_location_;
        std::string cur_str_;

    public:
        OurFlexLexer();

        yy::location get_cur_location();
        std::string get_cur_str_();

        void upd_cur_loc();

        int yylex() override;

        ~OurFlexLexer();

    };
}





#endif // PARACL_PARSER_HH