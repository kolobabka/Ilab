#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "frontend.hpp"
#include "backend.hpp"

int main(int argc, char *argv[]) try {

    Frontend front{argc, argv};
    auto ast = front.build_ast ();
    std::unique_ptr<Codegen> codegen;
    codegen.reset(Codegen::createCodeGen("pcl.module"));
    codegen->StartFunction("__pcl_start");
    codegen->codegen(ast);
    codegen->EndCurrentFunction();
    codegen->SaveModule("file.pcl.ll");

} catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
} catch (...) {
    std::cerr << "Exception unknown\n";
}