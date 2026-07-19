#pragma once

#include <cstddef>

class DecoderStats {
public:
    DecoderStats();

    void record_frame_received();
    void record_valid_frame();
    void record_unknown_id();
    void record_invalid_dlc();
    void add_faults(std::size_t count);

    void print() const;

    std::size_t total_frames() const;
    std::size_t valid_frames() const;
    std::size_t invalid_dlc_frames() const;
    std::size_t unknown_id_frames() const;
    std::size_t fault_count() const;

private:
    std::size_t total_frames_;
    std::size_t valid_frames_;
    std::size_t invalid_dlc_frames_;
    std::size_t unknown_id_frames_;
    std::size_t fault_count_;
};