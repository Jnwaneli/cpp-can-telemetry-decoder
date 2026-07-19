#include "decoder_stats.hpp"

#include <iostream>

DecoderStats::DecoderStats()
    : total_frames_(0),
      valid_frames_(0),
      invalid_dlc_frames_(0),
      unknown_id_frames_(0),
      fault_count_(0) {
}

void DecoderStats::record_frame_received() {
    total_frames_++;
}

void DecoderStats::record_valid_frame() {
    valid_frames_++;
}

void DecoderStats::record_unknown_id() {
    unknown_id_frames_++;
    fault_count_++;
}

void DecoderStats::record_invalid_dlc() {
    invalid_dlc_frames_++;
    fault_count_++;
}

void DecoderStats::add_faults(std::size_t count) {
    fault_count_ += count;
}

void DecoderStats::print() const {
    std::cout << "Decoder Stats:" << std::endl;

    std::cout << "Total frames: "
              << total_frames_
              << std::endl;

    std::cout << "Valid frames: "
              << valid_frames_
              << std::endl;

    std::cout << "Invalid DLC frames: "
              << invalid_dlc_frames_
              << std::endl;

    std::cout << "Unknown ID frames: "
              << unknown_id_frames_
              << std::endl;

    std::cout << "Fault count: "
              << fault_count_
              << std::endl;
}

std::size_t DecoderStats::total_frames() const {
    return total_frames_;
}

std::size_t DecoderStats::valid_frames() const {
    return valid_frames_;
}

std::size_t DecoderStats::invalid_dlc_frames() const {
    return invalid_dlc_frames_;
}

std::size_t DecoderStats::unknown_id_frames() const {
    return unknown_id_frames_;
}

std::size_t DecoderStats::fault_count() const {
    return fault_count_;
}