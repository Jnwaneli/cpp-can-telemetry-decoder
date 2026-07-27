#pragma once

#include <string>

#include "decoder_stats.hpp"
#include "fault_analyzer.hpp"

bool write_fault_summary_json(const std::string& output_path,
                              const std::string& source_name,
                              const DecoderStats& stats,
                              const FaultAnalyzer& fault_summary_tracker);
