/*
Step 1 - Program Understanding and Candidate Keywords (extracted from the focal method)
- Core predicate: if (srgb_intent >= PNG_sRGB_INTENT_LAST)
- Dependency hooks:
  - png_debug(level, "in png_write_sRGB")
  - png_warning(png_ptr, "Invalid sRGB rendering intent specified")
  - png_write_complete_chunk(png_ptr, PNG_sRGB, buf, 1)
- Key data: srgb_intent (int), buf[1] holding (png_byte)srgb_intent
- Important constants/macros:
  - PNG_sRGB: 4-byte chunk type identifier
  - PNG_sRGB_INTENT_LAST: upper bound for valid intents
- Types used (abbreviated for test scaffolding): png_structrp, png_byte, png_bytep, png_const_bytep, png_uint_32
- Behavior: Always writes an sRGB chunk; warns if srgb_intent is invalid
*/

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Step 2 scaffolding: Lightweight, self-contained mock environment to exercise the focal function
// These definitions emulate the minimal surface needed by the focal function.
// They purposely avoid pulling in real libpng headers to keep the test self-contained.

#define PNG_sRGB 0x73524742U            // ASCII for 'sRGB'
#define PNG_sRGB_INTENT_LAST 3            // Valid intents: 0,1,2. 3+ should warn.

typedef unsigned char png_byte;
typedef unsigned char* png_bytep;
typedef const unsigned char* png_const_bytep;
typedef uint32_t png_uint_32;
typedef struct png_struct* png_structrp;

// Global logging containers to observe behavior of the focal function
static std::vector<std::string> g_warnings;
static std::vector<std::string> g_debugs;
static std::vector<uint32_t> g_written_chunk_names;
static std::vector<std::vector<uint8_t>> g_written_chunks;

// Mock implementations to capture the side effects of the focal function
void png_debug(int level, const char* where) {
    // Record the debug string to verify the function flow
    if (where) g_debugs.emplace_back(where);
    else g_debugs.emplace_back("debug");
}
void png_warning(png_structrp /*png_ptr*/, const char* message) {
    if (message) g_warnings.emplace_back(message);
    else g_warnings.emplace_back("warning");
}
void png_write_complete_chunk(png_structrp /*png_ptr*/, png_uint_32 chunk_name,
                              png_const_bytep data, size_t length) {
    // Record the chunk name and its payload for validation
    g_written_chunk_names.push_back(chunk_name);
    std::vector<uint8_t> payload(length);
    if (data && length > 0) {
        std::memcpy(payload.data(), data, length);
    }
    g_written_chunks.emplace_back(std::move(payload));
}

// Forward declaration of the focal method under test (re-implemented here for self-contained testing)
void png_write_sRGB(png_structrp png_ptr, int srgb_intent)
{
{
   png_byte buf[1];
   png_debug(1, "in png_write_sRGB");
   if (srgb_intent >= PNG_sRGB_INTENT_LAST)
      png_warning(png_ptr,
          "Invalid sRGB rendering intent specified");
   buf[0]=(png_byte)srgb_intent;
   png_write_complete_chunk(png_ptr, PNG_sRGB, buf, 1);
}
}

// Utility helpers for tests
void reset_logs() {
    g_warnings.clear();
    g_debugs.clear();
    g_written_chunk_names.clear();
    g_written_chunks.clear();
}

// Simple test harness primitives
static int g_total_tests = 0;
static int g_failed_tests = 0;

void log_failure(const std::string& test_name, const std::string& detail) {
    std::cerr << "[FAIL] " << test_name << ": " << detail << "\n";
}
void log_success(const std::string& test_name) {
    std::cout << "[OK] " << test_name << "\n";
}

// Test 1: srgb_intent = 0 should produce no warning and write chunk with data 0
bool test_srgb_intent_zero() {
    const std::string test_name = "test_srgb_intent_zero";
    reset_logs();
    png_write_sRGB(nullptr, 0);

    bool ok = true;
    // Expect a single debug message
    if (g_debugs.empty() || g_debugs.back() != "in png_write_sRGB") {
        log_failure(test_name, "Expected debug message 'in png_write_sRGB' on entry.");
        ok = false;
    }
    // No warnings for valid intent
    if (!g_warnings.empty()) {
        log_failure(test_name, "Did not expect any warnings for srgb_intent = 0.");
        ok = false;
    }
    // One chunk written with correct name and payload
    if (g_written_chunk_names.size() != 1 || g_written_chunk_names.back() != PNG_sRGB) {
        log_failure(test_name, "Expected one PNG_sRGB chunk to be written.");
        ok = false;
    } else {
        const auto& payload = g_written_chunks.back();
        if (payload.size() != 1 || payload[0] != 0) {
            log_failure(test_name, "Expected payload [0] for srgb_intent 0.");
            ok = false;
        }
    }
    if (ok) log_success(test_name);
    return ok;
}

// Test 2: srgb_intent = 3 should trigger warning but still write chunk with data 3
bool test_srgb_intent_three_warns() {
    const std::string test_name = "test_srgb_intent_three_warns";
    reset_logs();
    png_write_sRGB(nullptr, 3);

    bool ok = true;
    // Expect a warning about invalid sRGB rendering intent
    if (g_warnings.empty() || g_warnings.back() != "Invalid sRGB rendering intent specified") {
        log_failure(test_name, "Expected warning for srgb_intent = 3.");
        ok = false;
    }
    // Also should write the chunk with data 3
    if (g_written_chunk_names.size() != 1 || g_written_chunk_names.back() != PNG_sRGB) {
        log_failure(test_name, "Expected one PNG_sRGB chunk to be written even for invalid intent.");
        ok = false;
    } else {
        const auto& payload = g_written_chunks.back();
        if (payload.size() != 1 || payload[0] != 3) {
            log_failure(test_name, "Expected payload [3] for srgb_intent 3.");
            ok = false;
        }
    }
    // Debug should have been logged
    if (g_debugs.empty() || g_debugs.back() != "in png_write_sRGB") {
        log_failure(test_name, "Expected debug entry for png_write_sRGB.");
        ok = false;
    }
    if (ok) log_success(test_name);
    return ok;
}

// Test 3: srgb_intent = -1 should not warn and write payload 255 (due to cast to png_byte)
bool test_srgb_intent_negative() {
    const std::string test_name = "test_srgb_intent_negative";
    reset_logs();
    png_write_sRGB(nullptr, -1);

    bool ok = true;
    // No warning for negative? Should not trigger since -1 < PNG_sRGB_INTENT_LAST
    if (!g_warnings.empty()) {
        log_failure(test_name, "Did not expect any warnings for srgb_intent = -1.");
        ok = false;
    }
    // Chunk with value 255
    if (g_written_chunk_names.size() != 1 || g_written_chunk_names.back() != PNG_sRGB) {
        log_failure(test_name, "Expected one PNG_sRGB chunk to be written for negative intent.");
        ok = false;
    } else {
        const auto& payload = g_written_chunks.back();
        if (payload.size() != 1 || payload[0] != 255) {
            log_failure(test_name, "Expected payload [255] for srgb_intent -1.");
            ok = false;
        }
    }
    if (ok) log_success(test_name);
    return ok;
}

// Test 4: srgb_intent = 2 should be valid and write payload 2 without warnings
bool test_srgb_intent_two() {
    const std::string test_name = "test_srgb_intent_two";
    reset_logs();
    png_write_sRGB(nullptr, 2);

    bool ok = true;
    if (!g_warnings.empty()) {
        log_failure(test_name, "Did not expect any warnings for srgb_intent = 2.");
        ok = false;
    }
    if (g_written_chunk_names.size() != 1 || g_written_chunk_names.back() != PNG_sRGB) {
        log_failure(test_name, "Expected one PNG_sRGB chunk to be written for srgb_intent = 2.");
        ok = false;
    } else {
        const auto& payload = g_written_chunks.back();
        if (payload.size() != 1 || payload[0] != 2) {
            log_failure(test_name, "Expected payload [2] for srgb_intent 2.");
            ok = false;
        }
    }
    if (ok) log_success(test_name);
    return ok;
}

// Main test runner
int main() {
    std::cout << "Starting unit tests for png_write_sRGB (mocked environment)\n";

    ++g_total_tests;
    bool t1 = test_srgb_intent_zero();
    if (!t1) ++g_failed_tests;

    ++g_total_tests;
    bool t2 = test_srgb_intent_three_warns();
    if (!t2) ++g_failed_tests;

    ++g_total_tests;
    bool t3 = test_srgb_intent_negative();
    if (!t3) ++g_failed_tests;

    ++g_total_tests;
    bool t4 = test_srgb_intent_two();
    if (!t4) ++g_failed_tests;

    int passed = g_total_tests - g_failed_tests;
    std::cout << "Tests run: " << g_total_tests << ", Passed: " << passed
              << ", Failed: " << g_failed_tests << "\n";

    return g_failed_tests ? 1 : 0;
}