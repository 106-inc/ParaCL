#include "driver.hh"

AST::IScope *glob_cur_scope = nullptr;

yy::Driver::Driver() {}

yy::Driver::Driver(const char *name_of_file) : name_of_file_(name_of_file) {
    plex_ = new OurFlexLexer;
    glob_cur_scope = AST::create_scope();

    std::ifstream in_file;
    in_file.open(name_of_file);

    if (in_file.is_open())
    {
        while (in_file)
        {
            std::string tmp_str;
            std::getline(in_file, tmp_str);
            line_of_prog.push_back(tmp_str);
        }
    }

    // plex_->switch_stream(in_file, std::cout);
}

bool yy::Driver::parse() {
    yy::parser parser_(this); //! it should be just "parser", but its ugly

    bool res = parser_.parse();
    return res;
}


//! There is should be:
yy::parser::token_type yy::Driver::yylex(yy::parser::semantic_type *yylval, parser::location_type* yylloc)
{
    yy::parser::token_type tkn_type = static_cast<yy::parser::token_type>(plex_->yylex());

    switch(tkn_type)
    {
        case yy::parser::token_type::INT:
        {
            yylval->as<int>() = std::stoi(plex_->YYText());
            break;
        }

        case yy::parser::token_type::NAME:
        {
            std::string tmp{plex_->YYText()};
            yylval->as<std::string>() = tmp;
            break;
        }

        default: break;

    }
    *yylloc = plex_->get_cur_location();
    return tkn_type;
}


yy::Driver::~Driver() { delete plex_; }
