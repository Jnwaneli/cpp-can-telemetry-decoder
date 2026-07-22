#pragma once

#include <string>
#include <vector>

#include "can_frame.hpp"

bool parse_can_frame_line(const std::string& line, CanFrame& frame);

std::vector<CanFrame> load_can_log_from_csv(const std::string& path);