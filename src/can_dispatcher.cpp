#include "can_dispatcher.hpp"

#include <iostream>

CanDispatcher::CanDispatcher(TelemetryDecoder& decoder)
    : decoder_(decoder) {
}

std::size_t CanDispatcher::dispatch(const CanFrame& frame) {
    decoder_.record_frame_seen();

    switch (frame.id) {
        case 0x100:
            return decoder_.decode_0x100(frame);

        case 0x101:
            return decoder_.decode_0x101(frame);

        case 0x102:
            return decoder_.decode_0x102(frame);

        case 0x200:
            return decoder_.decode_0x200(frame);

        default:
            std::cout << "Type: Unknown dispatch route" << std::endl;
            return 0;
    }
}