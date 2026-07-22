#include "can_log_parser.hpp"

#include <cctype>
#include <cstdint>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

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

std::string strip_optional_hex_prefix(const std::string& token) {
    std::string cleaned = trim(token);

    if (cleaned.size() >= 2 &&
        cleaned[0] == '0' &&
        (cleaned[1] == 'x' || cleaned[1] == 'X')) {
        return cleaned.substr(2);
    }

    return cleaned;
}

std::uint32_t parse_hex_u32_token(const std::string& token) {
    std::string cleaned = strip_optional_hex_prefix(token);

    if (cleaned.empty()) {
        throw std::invalid_argument("empty hex token");
    }

    return static_cast<std::uint32_t>(std::stoul(cleaned, nullptr, 16));
}

std::uint8_t parse_hex_u8_token(const std::string& token) {
    std::uint32_t value = parse_hex_u32_token(token);

    if (value > 0xFF) {
        throw std::out_of_range("byte token larger than 0xFF");
    }

    return static_cast<std::uint8_t>(value);
}

std::uint8_t parse_decimal_u8_token(const std::string& token) {
    std::string cleaned = trim(token);

    if (cleaned.empty()) {
        throw std::invalid_argument("empty decimal token");
    }

    std::uint32_t value = static_cast<std::uint32_t>(
        std::stoul(cleaned, nullptr, 10)
    );

    if (value > 0xFF) {
        throw std::out_of_range("decimal token larger than 255");
    }

    return static_cast<std::uint8_t>(value);
}
}

bool parse_can_frame_line(const std::string& line, CanFrame& frame) {
    std::stringstream stream(line);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(stream, token, ',')) {
        tokens.push_back(trim(token));
    }

    if (tokens.size() != 10) {
        return false;
    }

    frame.id = parse_hex_u32_token(tokens[0]);
    frame.dlc = parse_decimal_u8_token(tokens[1]);

    for (int i = 0; i < 8; i++) {
        frame.data[i] = parse_hex_u8_token(tokens[i + 2]);
    }

    return true;
}

std::vector<CanFrame> load_can_log_from_csv(const std::string& path) {
    std::ifstream file(path);

    if (!file) {
        std::cout << "CAN log file not found: "
                  << path
                  << std::endl;

        return {};
    }

    std::vector<CanFrame> log;
    std::string line;
    bool first_line = true;

    while (std::getline(file, line)) {
        line = trim(line);

        if (line.empty()) {
            continue;
        }

        if (first_line && looks_like_header(line)) {
            first_line = false;
            continue;
        }

        first_line = false;

        CanFrame frame{};

        try {
            if (parse_can_frame_line(line, frame)) {
                log.push_back(frame);
            } else {
                std::cout << "Skipped malformed CAN log line: "
                          << line
                          << std::endl;
            }
        } catch (const std::exception& error) {
            std::cout << "Skipped CAN log line due to parse error: "
                      << line
                      << std::endl;

            std::cout << "Parse error: "
                      << error.what()
                      << std::endl;
        }
    }

    std::cout << "Loaded CAN log from: "
              << path
              << std::endl;

    std::cout << "Frames loaded: "
              << log.size()
              << std::endl
              << std::endl;

    return log;
}