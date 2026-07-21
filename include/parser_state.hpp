#pragma once

enum class ParserState {
    WAIT_FOR_FRAME,
    VALIDATE_ID,
    VALIDATE_DLC,
    DECODE,
    ANALYZE_FAULTS,
    PRINT_RESULT
};

inline const char* parser_state_name(ParserState state) {
    switch (state) {
        case ParserState::WAIT_FOR_FRAME:
            return "WAIT_FOR_FRAME";

        case ParserState::VALIDATE_ID:
            return "VALIDATE_ID";

        case ParserState::VALIDATE_DLC:
            return "VALIDATE_DLC";

        case ParserState::DECODE:
            return "DECODE";

        case ParserState::ANALYZE_FAULTS:
            return "ANALYZE_FAULTS";

        case ParserState::PRINT_RESULT:
            return "PRINT_RESULT";

        default:
            return "UNKNOWN_STATE";
    }
}