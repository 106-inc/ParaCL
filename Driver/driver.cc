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
    parser parser(this); //! it should be just "parser", but its ugly

    bool res = parser.parse();
    return res;
}


//! There is should be:
yy::parser::token_type yylex(yy::parser::semantic_type *yylval)
{
    //! 1. Getting token from
    //! 2. Token processing line by line in our ParaCL program


}

yy::Driver::~Driver() { delete plex_; }
