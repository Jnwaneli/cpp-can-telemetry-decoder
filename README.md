# C++ CAN Telemetry Decoder and Fault Analyzer

## Overview

This project is a C++17 CAN telemetry decoder and fault analyzer for simulated, logged, STM32-generated, and live Waveshare USB-CAN frames.

The desktop C++ application validates CAN message IDs and DLC values, decodes little-endian payloads, extracts status flags with bit masks, tracks running signal statistics, detects faults, and prints structured diagnostic summaries.

The project also includes an STM32 FreeRTOS CAN telemetry sender that generates simulated live telemetry values and transmits CAN frames through an SN65HVD230 CAN transceiver to a Waveshare USB-CAN adapter.

The current implementation includes a `FrameSource` input architecture, CSV log input, direct Waveshare USB-CAN live serial ingestion on Windows, structured JSON output, and a diagnostic report agent that explains the C++ decoder's fault summary.

The C++ decoder performs deterministic parsing and fault detection. The diagnostic report agent only explains the structured fault summary.

---

## Current Status

```text
C++17 desktop decoder: working
CSV log decoding: working
FrameSource interface: working
CsvFrameSource backend: working
Live-style frame loop: working
WaveshareSerialFrameSource backend: working on Windows
STM32 FreeRTOS CAN sender: working
SN65HVD230 CAN transceiver path: tested
Waveshare USB-CAN live ingestion: tested
Button-gated STM32/MAX7219 LED fault demo: working
fault_summary.json output: working
Diagnostic report agent: working after fault_summary.json is generated
Demo screenshots: committed
Muted demo videos: committed
SocketCAN can0 backend: future work
```

---

## Hardware Wiring Schematic

The tested hardware wiring connects the STM32 NUCLEO-G431RB to the Waveshare USB-CAN adapter through an SN65HVD230 CAN transceiver.

<img src="./media/freertos_can_hardware_wiring.png" alt="STM32 FreeRTOS CAN hardware wiring schematic" width="900">

**Figure 1. STM32 FreeRTOS CAN hardware wiring schematic.** The NUCLEO-G431RB connects to the SN65HVD230 CAN transceiver using FDCAN1_TX, FDCAN1_RX, 3.3 V, and GND. The SN65HVD230 connects to the Waveshare USB-CAN adapter through CANH, CANL, and common ground. The Waveshare adapter connects to the PC over USB for live receive testing and C++ decoder integration.

Key wiring notes:

```text
PA12 / FDCAN1_TX -> SN65HVD230 TXD
PA11 / FDCAN1_RX <- SN65HVD230 RXD
NUCLEO 3.3V      -> SN65HVD230 VCC
NUCLEO GND       -> SN65HVD230 GND
SN65HVD230 CANH  -> Waveshare CANH
SN65HVD230 CANL  -> Waveshare CANL
SN65HVD230 GND   -> Waveshare GND
```

The STM32 FDCAN pins do not connect directly to CANH and CANL. The SN65HVD230 transceiver converts STM32 logic-level CAN signals into physical CAN bus signals.

The CAN bus uses Classic CAN at 500 kbps with standard 11-bit identifiers and 8-byte DLC frames. CANH-to-CANL measured about 60 ohms with power off, confirming two active 120 ohm terminations in parallel.

---

## Live Demo Proof

The project was validated using direct live CAN ingestion from the Waveshare USB-CAN adapter into the C++ decoder.

Command used:

```powershell
./main.exe --waveshare-serial COM4 2000
```

Final live result:

```text
Frames processed: 2000
Valid frames: 2000
Unknown IDs: 0
Invalid DLC: 0
Faults: 0
Warnings: 0
Dropped frames: 0
```

Because the STM32 firmware transmits four CAN frame types per transmit cycle, this run represents 500 telemetry cycles.

```text
2000 total frames / 4 frame types = 500 telemetry cycles
```

---

## Demo Screenshots

### C++ Live Decoder 2,000-Frame Summary

The desktop C++ decoder processed 2,000 live STM32-generated CAN frames through the Waveshare live input path with no unknown IDs, invalid DLC errors, dropped-frame faults, diagnostic faults, or warnings.

<img src="./media/live_decoder_summary.png?cache=live-demo-final" alt="C++ live decoder summary showing 2,000 valid CAN frames" width="900">

**Figure 2. C++ live decoder 2,000-frame summary.** The terminal summary shows 2,000 valid live CAN frames processed through `WaveshareSerialFrameSource` with zero unknown IDs, invalid DLC errors, faults, warnings, or dropped frames.

### Waveshare USB-CAN Receive Software

The Waveshare USB-CAN software confirmed live CAN traffic from the STM32 FreeRTOS telemetry sender at 500 kbps.

<img src="./media/waveshare_receive.png?cache=receive-final" alt="Waveshare USB-CAN software receiving live CAN frames" width="900">

**Figure 3. Waveshare USB-CAN receive software.** The receive window confirms that the STM32 FreeRTOS sender is transmitting live CAN frames through the SN65HVD230 transceiver into the Waveshare USB-CAN adapter.

### Physical Hardware Test Setup

The physical hardware setup shows the NUCLEO-G431RB connected to the SN65HVD230 CAN transceiver on a breadboard, with CANH/CANL connected to the Waveshare USB-CAN adapter.

<img src="./media/physical_hardware_setup.jpg" alt="Physical STM32 FreeRTOS CAN hardware test setup" width="700">

**Figure 4. Physical STM32 FreeRTOS CAN hardware test setup.** The NUCLEO-G431RB is wired to the SN65HVD230 CAN transceiver on a breadboard. The transceiver connects over CANH/CANL to the Waveshare USB-CAN adapter for PC-side live CAN receive testing.

### Fully Powered LED Fault Demo Circuit

The fully powered hardware setup shows the STM32 NUCLEO-G431RB, MAX7219 LED matrix, SN65HVD230 CAN transceiver, and Waveshare USB-CAN adapter connected together during the LED fault demo workflow.

<img src="./media/powered_led_fault_demo_setup.jpg" alt="Fully powered STM32 MAX7219 LED fault demo circuit" width="900">

**Figure 5. Fully powered STM32/MAX7219 LED fault demo circuit.** The MAX7219 LED matrix, STM32 FreeRTOS sender, CAN transceiver, and Waveshare USB-CAN adapter are powered and connected for the synchronized LED display and live CAN fault-injection demo.

---

## Demo Videos

The demo videos are intentionally muted and use GitHub attachment links so they can play directly in the README. For better quality, use the original MP4 links below.

### Decoder Live Ingestion Demo

https://github.com/user-attachments/assets/247298cb-0269-4862-9e76-c4814260db50

[Watch in higher quality / download original decoder MP4](https://github.com/Jnwaneli/cpp-can-telemetry-decoder/raw/main/media/decoder_demo.mp4?cache=muted-final)

**Video 1. Decoder live-ingestion demo.** The C++ decoder reads live Waveshare serial frames from the STM32 FreeRTOS CAN sender and processes them through the normal validation, dispatch, decode, statistics, and fault-analysis pipeline.

### Waveshare Receive Demo

https://github.com/user-attachments/assets/7c140fec-3e94-411c-a61a-abba91898925

[Watch in higher quality / download original Waveshare MP4](https://github.com/Jnwaneli/cpp-can-telemetry-decoder/raw/main/media/waveshare_demo.mp4?cache=muted-final)

**Video 2. Waveshare receive demo.** The Waveshare USB-CAN receive software shows live CAN traffic from the STM32 FreeRTOS telemetry sender at 500 kbps.

### STM32 FreeRTOS LED Fault Demo

This demo shows the STM32 FreeRTOS firmware, MAX7219 8x8 LED matrix, and C++ CAN decoder working together in real time. The NUCLEO user button starts the demo sequence. Before the button press, the LED matrix runs a scanner sweep startup animation. After the button press, the firmware cycles through demo fault modes and injects matching values into the CAN payload.

The LED matrix and decoder are not directly connected to each other. Instead, both are driven by the same firmware demo mode:

- Check mark: normal telemetry values
- Exclamation mark: high-temperature demo fault
- X symbol: low-voltage or invalid-sensor demo fault

The C++ decoder receives the live CAN frames through the Waveshare USB-CAN adapter and reports the corresponding fault summary.

https://github.com/user-attachments/assets/3a10cdd9-0971-4c32-9f4d-37c49d1fd2c0

[Watch in higher quality / download original LED fault demo MP4](https://github.com/Jnwaneli/cpp-can-telemetry-decoder/raw/main/media/led_fault_demo_side_by_side.mp4?cache=led-fault-demo-final)

**Video 3. Button-gated STM32 FreeRTOS LED fault demo.** The left side shows the C++ live CAN decoder processing frames from the Waveshare USB-CAN adapter. The right side shows the STM32/MAX7219 LED matrix displaying the synchronized fault state. Pressing the NUCLEO user button starts the fault demo cycle.

Suggested demo caption:

```text
Live CAN test: decoded 2,000 consecutive STM32-generated CAN frames through a Waveshare USB-CAN live serial backend at 500 kbps, with 0 unknown IDs, 0 invalid DLC errors, 0 dropped-frame faults, and 0 diagnostic warnings.
```

---

## Software/Data Flow Diagram

```mermaid
flowchart TD
    A[STM32 FreeRTOS Sender] --> B[SignalGeneratorTask]
    B -->|SensorSample every 10 ms| C[sensorQueueHandle]
    C --> D[ProcessingTask]
    D -->|mutex-protected update| E[latestTelemetry]
    E -->|local copy| F[CanTxTask]
    F -->|4 CAN frames every ~100 ms| G[STM32 FDCAN1]
    G --> H[SN65HVD230 CAN Transceiver]
    H --> I[CANH / CANL Bus]
    I --> J[Waveshare USB-CAN Adapter]
    J -->|Windows COM port| K[WaveshareSerialFrameSource]

    L[CSV CAN Log] --> M[CsvFrameSource]

    K --> N[CanFrame]
    M --> N

    N --> O[run_decoder_live]
    O --> P[process_frame]
    P --> Q[Validate CAN ID and DLC]
    Q --> R{Valid frame?}

    R -->|No| S[Record validation fault]
    R -->|Yes| T[CanDispatcher]

    T --> U[TelemetryDecoder]
    U --> V[Decode by CAN ID]

    V --> V100[0x100 Analog Inputs]
    V --> V101[0x101 Battery and Temperature]
    V --> V102[0x102 Status Flags]
    V --> V200[0x200 Vehicle Telemetry]

    V100 --> W[SignalStats]
    V101 --> W
    V100 --> X[CounterTracker]
    V200 --> X
    V100 --> Y[StuckSensorTracker]

    W --> Z[FaultAnalyzer]
    X --> Z
    Y --> Z
    V101 --> Z
    V102 --> Z
    V200 --> Z
    S --> Z

    Z --> AA[FrameReport]
    Z --> AB[DecoderStats]
    Z --> AC[Terminal Summary]
    Z --> AD[output/fault_summary.json]
    AD --> AE[Diagnostic Report Agent]
    AE --> AF[output/diagnostic_report.md]
```

**Figure 6. Software and data flow diagram.** Live hardware frames and CSV log frames are normalized into `CanFrame` objects before passing through the same validation, dispatch, decoding, statistics, counter tracking, and fault-analysis pipeline. The structured JSON summary then feeds the diagnostic report agent.

---

## Project Structure

```text
cpp-can-telemetry-decoder/
├── README.md
├── main.cpp
├── .gitignore
│
├── include/
│   ├── can_frame.hpp
│   ├── can_validation.hpp
│   ├── can_dispatcher.hpp
│   ├── can_log_parser.hpp
│   ├── circular_buffer.hpp
│   ├── frame_source.hpp
│   ├── csv_frame_source.hpp
│   ├── waveshare_serial_frame_source.hpp
│   ├── fault_summary_writer.hpp
│   ├── telemetry_decoder.hpp
│   ├── telemetry_data.hpp
│   ├── fault_analyzer.hpp
│   ├── fault_report.hpp
│   ├── frame_report.hpp
│   ├── decoder_stats.hpp
│   ├── signal_stats.hpp
│   ├── counter_tracker.hpp
│   ├── stuck_sensor_tracker.hpp
│   ├── bit_utils.hpp
│   └── parser_state.hpp
│
├── src/
│   ├── can_dispatcher.cpp
│   ├── can_log_parser.cpp
│   ├── circular_buffer.cpp
│   ├── csv_frame_source.cpp
│   ├── waveshare_serial_frame_source.cpp
│   ├── fault_summary_writer.cpp
│   ├── telemetry_decoder.cpp
│   ├── fault_analyzer.cpp
│   ├── decoder_stats.cpp
│   ├── signal_stats.cpp
│   ├── counter_tracker.cpp
│   ├── stuck_sensor_tracker.cpp
│   └── bit_utils.cpp
│
├── data/
│   ├── sample_can_log.csv
│   ├── good_frames.csv
│   ├── fault_frames.csv
│   ├── stuck_sensor_test.csv
│   └── freertos_captured_log.csv
│
├── output/
│   ├── fault_summary.json
│   └── diagnostic_report.md
│
├── docs/
│   ├── protocol.md
│   ├── fault_rules.md
│   ├── sample_output.md
│   ├── hardware_log_status.md
│   ├── can_wiring_plan.md
│   ├── stm32_fdcan_config.md
│   ├── can_debug_checklist.md
│   ├── usb_can_capture_notes.md
│   ├── real_time_cpp_ingestion.md
│   ├── software_data_flow.md
│   ├── fault_summary_json.md
│   ├── live_waveshare_test.md
│   └── freertos_live_capture_test.md
│
├── media/
│   ├── freertos_can_hardware_wiring.png
│   ├── physical_hardware_setup.jpg
│   ├── powered_led_fault_demo_setup.jpg
│   ├── live_decoder_summary.png
│   ├── waveshare_receive.png
│   ├── decoder_demo.mp4
│   ├── waveshare_demo.mp4
│   ├── decoder_demo_github.mp4
│   ├── waveshare_demo_github.mp4
│   └── led_fault_demo_side_by_side.mp4
│
├── embedded/
│   └── can_hardware_bridge/
│       └── stm32_can_sender_freertos/
│           ├── Core/
│           ├── docs/
│           └── README.md
│
└── tools/
    └── diagnostic_report_agent/
        ├── README.md
        ├── generate_report.py
        ├── sample_fault_summary.json
        └── sample_report.md
```

---

## CAN Protocol

The decoder supports four standard Classic CAN message IDs.

| CAN ID | Name | Purpose |
|---|---|---|
| `0x100` | Analog Inputs | ADC-style analog sensor values |
| `0x101` | Battery and Temperature | Battery voltage and temperature |
| `0x102` | Status Flags | Sensor validity flags, mode, and error code |
| `0x200` | Vehicle Telemetry | Speed, RPM, gear, throttle, brake, and counter |

All supported frames currently use:

```text
Standard CAN ID
Classic CAN
DLC = 8
Little-endian 16-bit signal packing
CAN bitrate = 500 kbps
```

Full protocol documentation is in `docs/protocol.md`.

---

## Fault Detection

The analyzer currently detects:

```text
Invalid DLC
Unknown CAN ID
Low battery voltage
High battery voltage
High temperature
Sensor-invalid status flags
Dropped frame counters
Possible stuck AIN1 sensor warning
```

Current thresholds:

```text
Battery voltage too low: below 10.5 V
Battery voltage too high: above 14.8 V
Temperature too high: above 80.0 C
Analog raw ADC range: 0 to 4095
Stuck AIN1 warning: same AIN1 value for 20 active frames
```

The stuck sensor check is treated as a warning because a stable sensor value can be valid depending on system conditions.

Full fault rule documentation is in `docs/fault_rules.md`.

---

## Build Instructions

### Requirements

```text
C++17-compatible compiler
PowerShell or terminal
g++ or another compatible C++ compiler
Windows for direct Waveshare serial mode
Python 3 for the diagnostic report agent
```

### Build on Windows with g++

From the repository root:

```powershell
g++ -std=c++17 -Wall -Wextra -Iinclude main.cpp src/circular_buffer.cpp src/telemetry_decoder.cpp src/bit_utils.cpp src/fault_analyzer.cpp src/decoder_stats.cpp src/can_dispatcher.cpp src/can_log_parser.cpp src/csv_frame_source.cpp src/waveshare_serial_frame_source.cpp src/fault_summary_writer.cpp src/signal_stats.cpp src/counter_tracker.cpp src/stuck_sensor_tracker.cpp -o main
```

---

## Running the Decoder

### Default CSV Mode

Run the default sample log:

```powershell
./main.exe
```

Expected sample-log summary:

```text
Frames processed: 10
Valid frames: 8
Unknown IDs: 1
Invalid DLC: 1
Faults: 3
Warnings: 0
Dropped frames: 1
```

After the run finishes, the decoder writes:

```text
output/fault_summary.json
```

### CSV Mode with a Specific File

```powershell
./main.exe --csv data/sample_can_log.csv
```

Other useful test files:

```text
data/good_frames.csv
data/fault_frames.csv
data/stuck_sensor_test.csv
data/freertos_captured_log.csv
```

### Waveshare Live Serial Mode

Close the Waveshare receive software before running this mode because the C++ application needs to open the COM port directly.

Find the adapter in Windows Device Manager:

```text
Device Manager -> Ports (COM & LPT) -> USB-SERIAL / CH340 / Waveshare adapter
```

Then run:

```powershell
./main.exe --waveshare-serial COM4 100
```

Replace `COM4` with the actual COM port assigned to the Waveshare adapter.

The final argument is the frame limit. For example, this command reads and decodes 2,000 live CAN frames:

```powershell
./main.exe --waveshare-serial COM4 2000
```

After a successful live run, the decoder writes:

```text
output/fault_summary.json
```

The clean live run should show:

```json
{
  "source": "waveshare_live",
  "frames_processed": 2000,
  "valid_frames": 2000,
  "invalid_dlc": 0,
  "unknown_ids": 0,
  "faults": 0,
  "warnings": 0,
  "low_voltage_faults": 0,
  "high_voltage_faults": 0,
  "voltage_faults": 0,
  "high_temperature_faults": 0,
  "sensor_invalid_faults": 0,
  "dropped_frames": 0,
  "possible_stuck_sensors": 0,
  "other_faults": 0
}
```

---

## End-to-End Diagnostic Report Pipeline

This is the full flow from live C++ decoding to the generated diagnostic report.

### 1. Start from the repository root

```powershell
cd C:\Users\joshw\Documents\cpp-can-telemetry-decoder
```

### 2. Build the C++ decoder

```powershell
g++ -std=c++17 -Wall -Wextra -Iinclude main.cpp src/circular_buffer.cpp src/telemetry_decoder.cpp src/bit_utils.cpp src/fault_analyzer.cpp src/decoder_stats.cpp src/can_dispatcher.cpp src/can_log_parser.cpp src/csv_frame_source.cpp src/waveshare_serial_frame_source.cpp src/fault_summary_writer.cpp src/signal_stats.cpp src/counter_tracker.cpp src/stuck_sensor_tracker.cpp -o main
```

### 3. Run the decoder

For the live hardware path:

```powershell
./main.exe --waveshare-serial COM4 2000
```

For a logged CSV path:

```powershell
./main.exe --csv data/sample_can_log.csv
```

The C++ decoder prints the terminal summary and writes:

```text
output/fault_summary.json
```

### 4. Check the JSON summary

```powershell
type output\fault_summary.json
```

For the live hardware run, confirm the source says:

```json
"source": "waveshare_live"
```

### 5. Run the diagnostic report agent

```powershell
python tools\diagnostic_report_agent\generate_report.py
```

If `python` is not recognized, use:

```powershell
py tools\diagnostic_report_agent\generate_report.py
```

Expected terminal output:

```text
Read fault summary: output/fault_summary.json
Wrote diagnostic report: output/diagnostic_report.md
```

### 6. Open the generated report

```powershell
type output\diagnostic_report.md
```

The generated report contains:

```text
Overview
Detected Faults
Likely Causes
Recommended Checks
Severity Notes
Next Steps
```

The C++ decoder performs deterministic parsing and fault detection. The diagnostic report agent only explains the structured fault summary.

---

## Diagnostic Report Agent

The diagnostic report agent is located at:

```text
tools/diagnostic_report_agent/
```

Files:

```text
README.md
generate_report.py
sample_fault_summary.json
sample_report.md
```

Input:

```text
output/fault_summary.json
```

Output:

```text
output/diagnostic_report.md
```

The agent does not decode raw CAN bytes. It does not decide whether a frame is valid. It only explains the structured summary produced by the deterministic C++ decoder.

---

## STM32 FreeRTOS Telemetry Sender

The STM32 firmware project is located at:

```text
embedded/can_hardware_bridge/stm32_can_sender_freertos/
```

The FreeRTOS sender uses:

```text
SignalGeneratorTask
ProcessingTask
CanTxTask
StatusLedTask
SensorSample queue
telemetry mutex
latestTelemetry shared structure
100 ms CAN transmit period
```

The sender transmits these CAN IDs repeatedly:

```text
0x100 Analog Inputs
0x101 Battery and Temperature
0x102 Status Flags
0x200 Vehicle Telemetry
```

`CanTxTask` owns the CAN transmit counter so the desktop decoder does not falsely detect dropped frames.

The LED fault demo adds a button-gated sequence that uses a scanner startup animation on the MAX7219 matrix before cycling through normal, high-temperature, low-voltage, and invalid-sensor demo modes. The same firmware demo mode drives both the LED symbol and the CAN payload values sent to the C++ decoder.

---

## Hardware Path

```text
NUCLEO-G431RB FDCAN
        ↓
SN65HVD230 CAN transceiver
        ↓
CANH/CANL bus
        ↓
Waveshare USB-CAN adapter
        ↓
Windows COM port
        ↓
WaveshareSerialFrameSource
        ↓
Desktop C++ decoder
        ↓
output/fault_summary.json
        ↓
Diagnostic report agent
        ↓
output/diagnostic_report.md
```

Hardware status:

```text
CAN bitrate: 500 kbps
Frame type: Standard Classic CAN
DLC: 8 bytes
Termination: about 60 ohms measured CANH-to-CANL with power off
```

---

## Implemented

```text
C++ CAN frame structure
FrameSource interface
CsvFrameSource backend
WaveshareSerialFrameSource backend
Live-style decoder loop
CSV CAN log parser
CANDispatcher
TelemetryDecoder
FaultAnalyzer
FaultReport and FrameReport
SignalStats
CounterTracker
StuckSensorTracker
0x100 decoder
0x101 decoder
0x102 decoder
0x200 decoder
Dropped counter detection
Possible stuck sensor warning
Structured fault_summary.json output
Diagnostic report agent
STM32 FreeRTOS telemetry sender
FreeRTOS queue and mutex architecture
Direct Waveshare USB-CAN live ingestion on Windows
Button-gated STM32/MAX7219 LED fault demo
Synchronized LED fault display and CAN fault injection
Hardware wiring schematic
Physical hardware setup photo
Live demo screenshots
Muted live demo videos
```

---

## Future Work

```text
Linux SocketCAN support
candump log parser
live can0 input backend
more realistic vehicle signal scaling
more fault injection patterns
real ADC or sensor-based telemetry on STM32
optional FreeRTOS FaultInjectTask
unit tests for parser, decoder, counters, and fault rules
production-grade serial reconnect/error recovery
```

---

## Current Limitations

```text
The live Waveshare serial reader is implemented for the current Windows/Waveshare workflow.
SocketCAN can0 support is not implemented yet.
The STM32 firmware currently generates simulated telemetry values rather than real ADC sensor values.
Speed scaling for 0x200 is not implemented yet.
0x102 system fault byte rules are not fully implemented yet.
The serial backend works for the demo workflow but does not yet include production-grade reconnect/recovery behavior.
The diagnostic report agent explains the JSON summary; it does not perform raw CAN decoding or independent fault detection.
```

---

## Final Project Story

This project connects desktop C++ decoding, fault analysis, STM32 FreeRTOS firmware, real CAN hardware, structured JSON output, and a diagnostic report layer into one end-to-end telemetry workflow.

The STM32 FreeRTOS firmware generates live simulated telemetry frames and sends them over CAN through an SN65HVD230 transceiver. The Waveshare USB-CAN adapter receives those frames, and the C++ application reads them directly through a live serial backend. Each received packet becomes a `CanFrame` and flows through the same validation, dispatcher, decoder, statistics, and fault-analysis pipeline used for CSV logs. The decoder then writes `output/fault_summary.json`, and the diagnostic report agent converts that structured summary into `output/diagnostic_report.md`.

The LED fault demo extends this story by showing that the STM32 firmware can drive both a physical MAX7219 LED status display and matching CAN fault payloads from the same FreeRTOS demo mode. This makes the visual embedded output and the C++ decoder's fault summary stay synchronized without requiring the LED matrix and decoder to communicate directly.
