#ifndef WAVESHARE_SERIAL_FRAME_SOURCE_HPP
#define WAVESHARE_SERIAL_FRAME_SOURCE_HPP

#include <cstdint>
#include <string>
#include <vector>

#include "can_frame.hpp"
#include "frame_source.hpp"

class WaveshareSerialFrameSource : public FrameSource {
public:
    explicit WaveshareSerialFrameSource(const std::string& port_name,
                                        std::uint32_t serial_baud = 2000000,
                                        std::uint8_t can_baud_code = 0x03);

    ~WaveshareSerialFrameSource() override;

    bool read_next(CanFrame& frame) override;

    bool is_open() const;
    const std::string& port_name() const;

private:
    void* serial_handle_;
    std::string port_name_;
    std::uint32_t serial_baud_;
    std::uint8_t can_baud_code_;
    std::vector<std::uint8_t> rx_buffer_;

    bool open_port();
    bool configure_port();
    bool send_start_command();
    bool read_byte(std::uint8_t& byte);
    bool parse_buffer(CanFrame& frame);
};

#endif