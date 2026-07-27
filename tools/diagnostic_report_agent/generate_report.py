import json
from pathlib import Path
from typing import Any, Dict, List


DEFAULT_INPUT_PATH = Path("output/fault_summary.json")
DEFAULT_OUTPUT_PATH = Path("output/diagnostic_report.md")


def load_fault_summary(path: Path) -> Dict[str, Any]:
    if not path.exists():
        raise FileNotFoundError(f"Input file not found: {path}")

    with path.open("r", encoding="utf-8") as file:
        return json.load(file)


def get_int(summary: Dict[str, Any], key: str) -> int:
    value = summary.get(key, 0)

    if isinstance(value, bool):
        return int(value)

    if isinstance(value, int):
        return value

    if isinstance(value, float):
        return int(value)

    try:
        return int(value)
    except (TypeError, ValueError):
        return 0


def build_detected_faults(summary: Dict[str, Any]) -> List[str]:
    detected_faults = []

    fault_fields = [
        ("invalid_dlc", "Invalid DLC frames detected"),
        ("unknown_ids", "Unknown CAN IDs detected"),
        ("low_voltage_faults", "Low battery voltage faults detected"),
        ("high_voltage_faults", "High battery voltage faults detected"),
        ("high_temperature_faults", "High temperature faults detected"),
        ("sensor_invalid_faults", "Sensor-invalid faults detected"),
        ("dropped_frames", "Dropped frame counter faults detected"),
        ("possible_stuck_sensors", "Possible stuck sensor warnings detected"),
        ("other_faults", "Other diagnostic faults detected"),
    ]

    for key, label in fault_fields:
        count = get_int(summary, key)

        if count > 0:
            detected_faults.append(f"- {label}: {count}")

    if not detected_faults:
        detected_faults.append("- No faults or warnings were detected in the structured summary.")

    return detected_faults


def build_likely_causes(summary: Dict[str, Any]) -> List[str]:
    likely_causes = []

    if get_int(summary, "invalid_dlc") > 0:
        likely_causes.append(
            "- Invalid DLC may indicate a protocol mismatch, corrupted capture, or sender-side frame packing issue."
        )

    if get_int(summary, "unknown_ids") > 0:
        likely_causes.append(
            "- Unknown CAN IDs may indicate unsupported messages, wrong transmitter configuration, or bus traffic outside the expected protocol."
        )

    if get_int(summary, "low_voltage_faults") > 0:
        likely_causes.append(
            "- Low voltage faults may indicate weak supply voltage, wiring resistance, a power source issue, or incorrect voltage scaling."
        )

    if get_int(summary, "high_voltage_faults") > 0:
        likely_causes.append(
            "- High voltage faults may indicate incorrect scaling, an overvoltage condition, or corrupted battery-voltage bytes."
        )

    if get_int(summary, "high_temperature_faults") > 0:
        likely_causes.append(
            "- High temperature faults may indicate simulated overtemperature data, a bad temperature sensor value, or incorrect temperature scaling."
        )

    if get_int(summary, "sensor_invalid_faults") > 0:
        likely_causes.append(
            "- Sensor-invalid faults may indicate missing sensor-valid bits, disconnected inputs, or firmware status flag issues."
        )

    if get_int(summary, "dropped_frames") > 0:
        likely_causes.append(
            "- Dropped frame faults may indicate missed CAN frames, bus timing issues, capture loss, or a counter ownership bug."
        )

    if get_int(summary, "possible_stuck_sensors") > 0:
        likely_causes.append(
            "- Possible stuck sensor warnings may indicate a frozen ADC value, repeated simulated input, or a real steady-state condition."
        )

    if get_int(summary, "other_faults") > 0:
        likely_causes.append(
            "- Other faults may require checking the detailed terminal output and the specific C++ fault rule that triggered the report."
        )

    if not likely_causes:
        likely_causes.append(
            "- No likely fault causes were identified because the structured summary reports a clean run."
        )

    return likely_causes


def build_recommended_checks(summary: Dict[str, Any]) -> List[str]:
    checks = []

    checks.append("- Confirm that the decoder input source matches the intended test path.")
    checks.append("- Confirm that the CAN bitrate is 500 kbps on both the STM32 sender and the USB-CAN adapter.")
    checks.append("- Confirm that all expected CAN IDs are present: 0x100, 0x101, 0x102, and 0x200.")

    if get_int(summary, "invalid_dlc") > 0:
        checks.append("- Check that every supported CAN frame is transmitted with DLC = 8.")

    if get_int(summary, "unknown_ids") > 0:
        checks.append("- Compare unknown CAN IDs against docs/protocol.md and the STM32 transmit code.")

    if get_int(summary, "low_voltage_faults") > 0 or get_int(summary, "high_voltage_faults") > 0:
        checks.append("- Verify voltage scaling, byte order, and the battery_mV payload field in CAN ID 0x101.")

    if get_int(summary, "high_temperature_faults") > 0:
        checks.append("- Verify temperature scaling, byte order, and the temperature_deciC payload field in CAN ID 0x101.")

    if get_int(summary, "sensor_invalid_faults") > 0:
        checks.append("- Check the sensor-valid status bits in CAN ID 0x100 and CAN ID 0x102.")

    if get_int(summary, "dropped_frames") > 0:
        checks.append("- Check CAN counter progression and confirm that CanTxTask owns the transmit counter.")

    if get_int(summary, "possible_stuck_sensors") > 0:
        checks.append("- Compare live signal variation against expected simulated sensor changes.")

    if get_int(summary, "faults") == 0 and get_int(summary, "warnings") == 0:
        checks.append("- No immediate corrective action is required for this run.")

    return checks


def build_severity_notes(summary: Dict[str, Any]) -> List[str]:
    faults = get_int(summary, "faults")
    warnings = get_int(summary, "warnings")
    dropped_frames = get_int(summary, "dropped_frames")
    stuck_sensors = get_int(summary, "possible_stuck_sensors")

    notes = []

    if faults == 0 and warnings == 0:
        notes.append("- Severity: clean run. No faults or warnings were reported.")
    else:
        notes.append(f"- Severity: {faults} fault(s) and {warnings} warning(s) were reported.")

    if dropped_frames > 0:
        notes.append("- Dropped frames should be treated as high priority because they may indicate lost telemetry data.")

    if stuck_sensors > 0:
        notes.append("- Stuck sensor detections are warnings because a stable sensor value can be valid in some operating conditions.")

    notes.append(
        "- The C++ decoder performs deterministic parsing and fault detection. The diagnostic assistant only explains the structured fault summary."
    )

    return notes


def build_next_steps(summary: Dict[str, Any]) -> List[str]:
    next_steps = []

    if get_int(summary, "faults") == 0 and get_int(summary, "warnings") == 0:
        next_steps.append("- Save this report as evidence of a clean live or logged decoder run.")
        next_steps.append("- Use the same workflow with injected fault data to demonstrate fault-report behavior.")
        next_steps.append("- Reference this report in project documentation or demos as proof of the full diagnostic pipeline.")
    else:
        next_steps.append("- Review each detected fault category and compare it against docs/fault_rules.md.")
        next_steps.append("- Re-run the decoder after correcting the suspected issue.")
        next_steps.append("- Save both the faulted report and the corrected report to show diagnostic workflow improvement.")

    return next_steps


def build_report(summary: Dict[str, Any]) -> str:
    source = summary.get("source", "unknown")
    frames_processed = get_int(summary, "frames_processed")
    valid_frames = get_int(summary, "valid_frames")
    faults = get_int(summary, "faults")
    warnings = get_int(summary, "warnings")

    lines = []

    lines.append("# AI-Assisted CAN Diagnostic Report")
    lines.append("")
    lines.append("## Overview")
    lines.append("")
    lines.append(f"- Source: `{source}`")
    lines.append(f"- Frames processed: {frames_processed}")
    lines.append(f"- Valid frames: {valid_frames}")
    lines.append(f"- Faults: {faults}")
    lines.append(f"- Warnings: {warnings}")
    lines.append("")
    lines.append(
        "The C++ decoder performs deterministic parsing and fault detection. The AI assistant only explains the structured fault summary."
    )
    lines.append("")

    lines.append("## Detected Faults")
    lines.append("")
    lines.extend(build_detected_faults(summary))
    lines.append("")

    lines.append("## Likely Causes")
    lines.append("")
    lines.extend(build_likely_causes(summary))
    lines.append("")

    lines.append("## Recommended Checks")
    lines.append("")
    lines.extend(build_recommended_checks(summary))
    lines.append("")

    lines.append("## Severity Notes")
    lines.append("")
    lines.extend(build_severity_notes(summary))
    lines.append("")

    lines.append("## Next Steps")
    lines.append("")
    lines.extend(build_next_steps(summary))
    lines.append("")

    return "\n".join(lines)


def write_report(report: str, output_path: Path) -> None:
    output_path.parent.mkdir(parents=True, exist_ok=True)

    with output_path.open("w", encoding="utf-8") as file:
        file.write(report)


def main() -> int:
    input_path = DEFAULT_INPUT_PATH
    output_path = DEFAULT_OUTPUT_PATH

    summary = load_fault_summary(input_path)
    report = build_report(summary)
    write_report(report, output_path)

    print(f"Read fault summary: {input_path}")
    print(f"Wrote diagnostic report: {output_path}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
