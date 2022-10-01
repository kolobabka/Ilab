#include "frontend.hpp"

#include <fstream>
#include <iostream>

Frontend::Frontend (const int argc, char **argv) : argc_(argc), argv_(argv) {

    if (argc_ != 2) {
        throw std::runtime_error ("You have to enter input filename only!");
    }

    code_.open (argv_[1], std::ios::in);
    if (!code_.is_open ()) {
        throw std::runtime_error ("Can't open input file");
    }

    std::streambuf *cinbuf = std::cin.rdbuf ();
    std::cin.rdbuf (code_.rdbuf ());
}

int Frontend::build_ast () {

    yy::Parser parser (argv_[1]);
    try {
        parser.parse ();
    }
    catch (std::runtime_error &err) {
        std::cout << err.what () << std::endl;
        return 0;
    }

    auto error_st = parser.err_begin ();
    auto error_fin = parser.err_end ();

    if (error_st != error_fin) {
        while (error_st != error_fin) {
            std::cout << *error_st << std::endl;
            error_st = std::next (error_st, 1);
        }

        return 0;
    }

    return 0;
}

