#include "csv_frame_source.hpp"

#include <cctype>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <cstddef>
#include "can_log_parser.hpp"

namespace {
std::string trim(const std::string& text) {
    std::size_t start = 0;

    while (start < text.size() &&
           std::isspace(static_cast<unsigned char>(text[start]))) {
        start++;
    }

    std::size_t end = text.size();

    while (end > start &&
           std::isspace(static_cast<unsigned char>(text[end - 1]))) {
        end--;
    }

    return text.substr(start, end - start);
}

std::string to_lower_copy(const std::string& text) {
    std::string result;

    for (char ch : text) {
        result.push_back(static_cast<char>(
            std::tolower(static_cast<unsigned char>(ch))
        ));
    }

    return result;
}

bool looks_like_header(const std::string& line) {
    std::stringstream stream(line);
    std::string first_token;
    std::string second_token;

    if (!std::getline(stream, first_token, ',')) {
        return false;
    }

    if (!std::getline(stream, second_token, ',')) {
        return false;
    }

    first_token = to_lower_copy(trim(first_token));
    second_token = to_lower_copy(trim(second_token));

    return first_token == "id" && second_token == "dlc";
}
}

CsvFrameSource::CsvFrameSource(const std::string& path)
    : file_(path),
      path_(path),
      first_line_(true),
      line_number_(0) {
    if (!file_) {
        std::cout << "CAN log file not found: "
                  << path_
                  << std::endl;
    } else {
        std::cout << "Opened CSV CAN frame source: "
                  << path_
                  << std::endl
                  << std::endl;
    }
}

bool CsvFrameSource::read_next(CanFrame& frame) {
    if (!file_) {
        return false;
    }

    std::string line;

    while (std::getline(file_, line)) {
        line_number_++;

        line = trim(line);

        if (line.empty()) {
            continue;
        }

        if (first_line_ && looks_like_header(line)) {
            first_line_ = false;
            continue;
        }

        first_line_ = false;

        try {
            if (parse_can_frame_line(line, frame)) {
                return true;
            }

            std::cout << "Skipped malformed CAN log line "
                      << line_number_
                      << ": "
                      << line
                      << std::endl;
        } catch (const std::exception& error) {
            std::cout << "Skipped CAN log line "
                      << line_number_
                      << " due to parse error: "
                      << line
                      << std::endl;

            std::cout << "Parse error: "
                      << error.what()
                      << std::endl;
        }
    }

    return false;
}

bool CsvFrameSource::is_open() const {
    return file_.is_open();
}

const std::string& CsvFrameSource::path() const {
    return path_;
}