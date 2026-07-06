#include <iostream>
#include <vector>
#include "can_frame.hpp"

void pointerBasics() {
    int x = 10;

    int* p = &x;
    int& r = x;

    std::cout << "Pointer basics:" << std::endl;
    std::cout << "x value: " << x << std::endl;
    std::cout << "x address: " << &x << std::endl;
    std::cout << "p stores address: " << p << std::endl;
    std::cout << "*p dereferenced value: " << *p << std::endl;
    std::cout << "r reference value: " << r << std::endl;

    *p = 20;
    std::cout << "After *p = 20, x is: " << x << std::endl;

    r = 30;
    std::cout << "After r = 30, x is: " << x << std::endl;
}

int main() {
    pointerBasics();

    std::cout << std::endl;
    std::cout << "Simulated CAN log:" << std::endl;

    std::vector<CanFrame> log = {
        {0x100, 8, {0x00, 0x08, 0x10, 0x00, 0xFF, 0x0A, 0x01, 0x05}},
        {0x101, 8, {0x88, 0x13, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00}},
        {0x102, 8, {0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
        {0x999, 8, {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08}},
        {0x100, 4, {0x00, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00}}
    };

    for (const CanFrame& frame : log) {
        print_frame(frame);
        std::cout << std::endl;
    }

    return 0;
}