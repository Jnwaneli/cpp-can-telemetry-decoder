#include <iostream>
#include <vector>
#include "can_frame.hpp"
#include "can_validation.hpp"

void process_frame(const CanFrame& frame) {
    print_frame(frame);

    bool has_fault = false;

    if (!is_known_id(frame.id)) {
        std::cout << "FAULT: Unknown CAN ID" << std::endl;
        has_fault = true;
    }

    if (!has_valid_dlc(frame)) {
        std::cout << "FAULT: Invalid DLC" << std::endl;
        has_fault = true;
    }

    if (!has_fault) {
        std::cout << "Frame status: OK" << std::endl;
    }
}

int main() {
    std::vector<CanFrame> log = {
        {0x100, 8, {0x00, 0x08, 0x10, 0x00, 0xFF, 0x0A, 0x01, 0x05}},
        {0x101, 8, {0x88, 0x13, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00}},
        {0x102, 8, {0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
        {0x999, 8, {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08}},
        {0x100, 4, {0x00, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00}},
        {0x200, 8, {0x34, 0x12, 0x78, 0x56, 0x03, 0x64, 0x01, 0x00}}
    };

    std::cout << "Simulated CAN log validation" << std::endl;
    std::cout << "============================" << std::endl;

    for (const CanFrame& frame : log) {
        process_frame(frame);
        std::cout << std::endl;
    }

    return 0;
}