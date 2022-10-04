#include <iostream>

extern "C" void __pcl_start();

extern "C" void __pcl_print(int n) { std::cout << n << std::endl; }

extern "C" int __pcl_scan() {
    int num;
    std::cin >> num;
    if (!std::cin) {
        std::cerr << "Wrong input from stdin\n";
        exit(1);
    }
    return num;
}

int main() { __pcl_start(); }