#ifndef FRAME_SOURCE_HPP
#define FRAME_SOURCE_HPP

#include "can_frame.hpp"

class FrameSource {
public:
    virtual ~FrameSource() = default;

    virtual bool read_next(CanFrame& frame) = 0;
};

#endif