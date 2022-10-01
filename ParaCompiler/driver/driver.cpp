#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "frontend.hpp"

int main(int argc, char *argv[]) try {

    Frontend front{argc, argv};
    auto ast = front.build_ast ();

} catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
} catch (...) {
    std::cerr << "Exception unknown\n";
}