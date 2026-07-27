#include "waveshare_serial_frame_source.hpp"

#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {
#ifdef _WIN32
HANDLE to_handle(void* handle) {
    return static_cast<HANDLE>(handle);
}
#endif

std::string make_windows_port_name(const std::string& port_name) {
    if (port_name.rfind("\\\\.\\", 0) == 0) {
        return port_name;
    }

    return "\\\\.\\" + port_name;
}

std::uint8_t low_byte_sum(const std::array<std::uint8_t, 20>& bytes) {
    std::uint32_t sum = 0;

    for (std::size_t i = 2; i <= 18; i++) {
        sum += bytes[i];
    }

    return static_cast<std::uint8_t>(sum & 0xFF);
}
}

WaveshareSerialFrameSource::WaveshareSerialFrameSource(const std::string& port_name,
                                                       std::uint32_t serial_baud,
                                                       std::uint8_t can_baud_code)
    : serial_handle_(nullptr),
      port_name_(port_name),
      serial_baud_(serial_baud),
      can_baud_code_(can_baud_code),
      rx_buffer_() {
#ifdef _WIN32
    if (!open_port()) {
        return;
    }

    if (!configure_port()) {
        CloseHandle(to_handle(serial_handle_));
        serial_handle_ = nullptr;
        return;
    }

    if (!send_start_command()) {
        std::cout << "Warning: Waveshare start/config command failed." << std::endl;
        std::cout << "The port opened, but CAN frames may not stream until the device is configured." << std::endl;
    }

    std::cout << "Opened Waveshare serial frame source on "
              << port_name_
              << " at "
              << serial_baud_
              << " baud."
              << std::endl
              << std::endl;
#else
    std::cout << "WaveshareSerialFrameSource direct COM reading is implemented for Windows in this version."
              << std::endl;
#endif
}

WaveshareSerialFrameSource::~WaveshareSerialFrameSource() {
#ifdef _WIN32
    if (serial_handle_ != nullptr) {
        CloseHandle(to_handle(serial_handle_));
        serial_handle_ = nullptr;
    }
#endif
}

bool WaveshareSerialFrameSource::open_port() {
#ifndef _WIN32
    return false;
#else
    std::string windows_port = make_windows_port_name(port_name_);

    HANDLE handle = CreateFileA(
        windows_port.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr
    );

    if (handle == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to open serial port: "
                  << port_name_
                  << std::endl;

        std::cout << "Make sure the Waveshare software is closed and the COM port is correct."
                  << std::endl;

        return false;
    }

    serial_handle_ = handle;

    SetupComm(handle, 4096, 4096);
    PurgeComm(handle, PURGE_RXCLEAR | PURGE_TXCLEAR);

    return true;
#endif
}

bool WaveshareSerialFrameSource::configure_port() {
#ifndef _WIN32
    return false;
#else
    HANDLE handle = to_handle(serial_handle_);

    DCB dcb{};
    dcb.DCBlength = sizeof(DCB);

    if (!GetCommState(handle, &dcb)) {
        std::cout << "GetCommState failed." << std::endl;
        return false;
    }

    dcb.BaudRate = serial_baud_;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    dcb.fBinary = TRUE;
    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    dcb.fRtsControl = RTS_CONTROL_ENABLE;

    if (!SetCommState(handle, &dcb)) {
        std::cout << "SetCommState failed. The adapter/driver may not accept "
                  << serial_baud_
                  << " baud."
                  << std::endl;
        return false;
    }

    COMMTIMEOUTS timeouts{};
    timeouts.ReadIntervalTimeout = 1;
    timeouts.ReadTotalTimeoutMultiplier = 1;
    timeouts.ReadTotalTimeoutConstant = 10;
    timeouts.WriteTotalTimeoutMultiplier = 1;
    timeouts.WriteTotalTimeoutConstant = 100;

    if (!SetCommTimeouts(handle, &timeouts)) {
        std::cout << "SetCommTimeouts failed." << std::endl;
        return false;
    }

    return true;
#endif
}

bool WaveshareSerialFrameSource::send_start_command() {
#ifndef _WIN32
    return false;
#else
    if (serial_handle_ == nullptr) {
        return false;
    }

    // Waveshare variable protocol configuration command.
    //
    // Byte 0:  0xAA
    // Byte 1:  0x55
    // Byte 2:  0x12 = variable-length protocol setting
    // Byte 3:  CAN baud code. 0x03 = 500 kbps
    // Byte 4:  0x01 = standard frame
    // Byte 5-8: filter ID = 0x00000000
    // Byte 9-12: block/mask ID = 0x00000000
    // Byte 13: 0x00 = normal mode
    // Byte 14: 0x00 = auto retransmit enabled
    // Byte 15-18: backup/reserved
    // Byte 19: checksum = low 8 bits of sum from byte 2 through byte 18
    std::array<std::uint8_t, 20> command{
        0xAA, 0x55,
        0x12,
        can_baud_code_,
        0x01,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00,
        0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00
    };

    command[19] = low_byte_sum(command);

    DWORD bytes_written = 0;

    BOOL ok = WriteFile(
        to_handle(serial_handle_),
        command.data(),
        static_cast<DWORD>(command.size()),
        &bytes_written,
        nullptr
    );

    if (!ok || bytes_written != command.size()) {
        std::cout << "Failed to send Waveshare configuration command." << std::endl;
        return false;
    }

    Sleep(100);

    std::cout << "Sent Waveshare variable-protocol start command."
              << std::endl;

    std::cout << "CAN baud code: 0x"
              << std::hex
              << std::uppercase
              << static_cast<int>(can_baud_code_)
              << std::dec
              << " (0x03 = 500 kbps)"
              << std::endl;

    return true;
#endif
}

bool WaveshareSerialFrameSource::read_byte(std::uint8_t& byte) {
#ifndef _WIN32
    return false;
#else
    if (serial_handle_ == nullptr) {
        return false;
    }

    DWORD bytes_read = 0;

    BOOL ok = ReadFile(
        to_handle(serial_handle_),
        &byte,
        1,
        &bytes_read,
        nullptr
    );

    if (!ok) {
        std::cout << "Serial read failed." << std::endl;
        return false;
    }

    return bytes_read == 1;
#endif
}

bool WaveshareSerialFrameSource::parse_buffer(CanFrame& frame) {
    while (!rx_buffer_.empty() && rx_buffer_[0] != 0xAA) {
        rx_buffer_.erase(rx_buffer_.begin());
    }

    if (rx_buffer_.size() < 2) {
        return false;
    }

    std::uint8_t type = rx_buffer_[1];

    std::uint8_t dlc = type & 0x0F;
    bool is_extended_frame = (type & 0x20) != 0;
    bool is_remote_frame = (type & 0x10) != 0;

    if (dlc > 8) {
        rx_buffer_.erase(rx_buffer_.begin());
        return false;
    }

    std::size_t id_length = is_extended_frame ? 4 : 2;
    std::size_t packet_length = 1 + 1 + id_length + dlc + 1;

    if (rx_buffer_.size() < packet_length) {
        return false;
    }

    if (rx_buffer_[packet_length - 1] != 0x55) {
        rx_buffer_.erase(rx_buffer_.begin());
        return false;
    }

    if (is_remote_frame) {
        rx_buffer_.erase(rx_buffer_.begin(), rx_buffer_.begin() + static_cast<long long>(packet_length));
        return false;
    }

    std::uint32_t id = 0;

    for (std::size_t i = 0; i < id_length; i++) {
        id |= static_cast<std::uint32_t>(rx_buffer_[2 + i]) << (8 * i);
    }

    frame.id = id;
    frame.dlc = dlc;

    for (int i = 0; i < 8; i++) {
        frame.data[i] = 0;
    }

    std::size_t data_start = 2 + id_length;

    for (std::size_t i = 0; i < dlc; i++) {
        frame.data[i] = rx_buffer_[data_start + i];
    }

    rx_buffer_.erase(rx_buffer_.begin(), rx_buffer_.begin() + static_cast<long long>(packet_length));

    return true;
}

bool WaveshareSerialFrameSource::read_next(CanFrame& frame) {
    if (!is_open()) {
        return false;
    }

    while (true) {
        if (parse_buffer(frame)) {
            return true;
        }

        std::uint8_t byte = 0;

        if (read_byte(byte)) {
            rx_buffer_.push_back(byte);
        } else {
#ifdef _WIN32
            Sleep(1);
#endif
        }
    }
}

bool WaveshareSerialFrameSource::is_open() const {
#ifdef _WIN32
    return serial_handle_ != nullptr;
#else
    return false;
#endif
}

const std::string& WaveshareSerialFrameSource::port_name() const {
    return port_name_;
}