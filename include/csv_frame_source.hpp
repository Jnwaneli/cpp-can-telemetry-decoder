#ifndef CSV_FRAME_SOURCE_HPP
#define CSV_FRAME_SOURCE_HPP

#include <fstream>
#include <string>

#include "can_frame.hpp"
#include "frame_source.hpp"

class CsvFrameSource : public FrameSource {
public:
    explicit CsvFrameSource(const std::string& path);

    bool read_next(CanFrame& frame) override;

    bool is_open() const;
    const std::string& path() const;

private:
    std::ifstream file_;
    std::string path_;
    bool first_line_;
    std::size_t line_number_;
};

#endif