# Software Data Flow

This diagram shows how CAN frames move through the full C++/STM32 telemetry system.

The same desktop decoder pipeline is used for both CSV logs and live Waveshare USB-CAN input. The input backend changes, but the decoded `CanFrame` objects flow through the same validation, dispatch, decoding, statistics, and fault-analysis path.

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
    Z --> AD[fault_summary.json - Week 9 Day 5]
    AD --> AE[AI Diagnostic Report - Week 9 Day 6]
```

## Current Implemented Path

```text
STM32 FreeRTOS sender
        ↓
SN65HVD230 CAN transceiver
        ↓
Waveshare USB-CAN adapter
        ↓
WaveshareSerialFrameSource
        ↓
CanFrame
        ↓
run_decoder_live()
        ↓
process_frame()
        ↓
CanDispatcher
        ↓
TelemetryDecoder
        ↓
FaultAnalyzer
        ↓
terminal summary
```

## CSV Path

```text
CSV CAN log
        ↓
CsvFrameSource
        ↓
CanFrame
        ↓
run_decoder_live()
        ↓
process_frame()
        ↓
CanDispatcher
        ↓
TelemetryDecoder
        ↓
FaultAnalyzer
```

## Next Week 9 Steps

```text
Day 5: Generate output/fault_summary.json
Day 6: Generate output/diagnostic_report.md using the AI diagnostic assistant
```

## Future Work

```text
Linux SocketCAN can0 backend
candump parser
production-grade serial reconnect/error recovery
real ADC sensor telemetry on STM32
```
