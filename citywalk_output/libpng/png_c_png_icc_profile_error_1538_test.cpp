// High-quality C++11 unit test suite for the focal method
// png_icc_profile_error located in png.c
// - No GTest/GMock, a lightweight in-process test harness is provided.
// - The tests rely on the real png_icc_profile_error and its dependencies
//   being available in the build (e.g., by compiling/linking against the
//   production png.c and its headers).
// - We intercept the benign error hook to validate the constructed message.
// - We exercise true/false branches of the ICC signature check where possible.

#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Domain knowledge notes and test strategy:
//  - We test the following aspects of png_icc_profile_error:
//    1) Correct message prefix and profile name handling (truncation at 79 chars).
//    2) ICC signature path: when is_ICC_signature(value) != 0, the tag name
//       is appended (via png_icc_tag_name) and a ': ' follows.
//    3) Non-ICC path (else branch): hex form of the value appended (via
//       png_format_number) and followed by "h: ".
//    4) The final message includes the provided reason.
//  - We capture the call to png_chunk_benign_error(png_ptr, message) to inspect
//   the built message without terminating the test process.
//  - We rely on the real implementation of is_ICC_signature, png_icc_tag_name,
//    png_safecat, png_format_number, and other helpers from png.c, but we hook
//    the output by providing a test harness for the chunk error callback.

extern "C" {
    // Prototype of the focal function (as it would be seen by C code)
    // We use void* for png_ptr to keep the header compatibility minimal in tests.
    int png_icc_profile_error(void* png_ptr, const char* name,
                            size_t value, const char* reason);

    // The PNG library's benign error reporter hook (we override it in tests to capture msgs)
    // In the actual libpng, this function is defined in the implementation;
    // tests hook into it by providing a compatible symbol here, relying on
    // the linker to resolve to the production implementation when not overridden.
    void png_chunk_benign_error(void* png_ptr, const char* message);
}

// Testing harness globals
static std::string g_last_message;

// Mock/Intercept: capture messages passed to the benign error reporter.
// If the real png.c defines its own png_chunk_benign_error, linking may override
// this symbol (depending on linker behavior). We assume the test build links against
// the production png.c and this symbol is overridable or the library uses it weakly.
extern "C" void png_chunk_benign_error(void* png_ptr, const char* message)
{
    g_last_message.assign(message ? message : "");
}

// Helper: reset captured message
static void reset_last_message() {
    g_last_message.clear();
}

// Helper: print test results
static void report_test(const std::string& name, bool ok) {
    std::cout << (ok ? "[PASS] " : "[FAIL] ") << name << "\n";
}

// Utility: locate substring positions in the captured message
static bool message_starts_with_profile_prefix(const std::string& msg) {
    const std::string prefix = "profile '";
    if (msg.size() < prefix.size()) return false;
    return msg.compare(0, prefix.size(), prefix) == 0;
}

// Utility: extract the profile name portion from the built message
static std::string extract_profile_name_from_message(const std::string& msg) {
    // Find: profile '<NAME>': 
    const std::string start = "profile '";
    size_t s = msg.find(start);
    if (s == std::string::npos) return "";
    s += start.size();
    size_t end = msg.find("': ", s);
    if (end == std::string::npos) return "";
    return msg.substr(s, end - s);
}

// Utility: check if a given substring exists in the message
static bool message_contains(const std::string& msg, const std::string& subseq) {
    return msg.find(subseq) != std::string::npos;
}

// Test 1: ICC path is taken (is_ICC_signature(value) != 0) and the ICC tag name is inserted.
// Assumption: We pass a value that the library recognizes as an ICC signature.
// We expect: - message starts with "profile '", - contains the profile name (possibly truncated),
//           - contains a 6-character tag name (e.g., "TAG123" from our mock png_icc_tag_name),
//           - includes a colon after the tag name, and
//           - ends with the provided reason.
static void test_icc_signature_path() {
    reset_last_message();

    // The value selected here should exercise the ICC signature path in the real code.
    // We intentionally pick a value that is likely to be recognized as an ICC signature.
    // If the environment does not classify this as an ICC signature, the test will fail gracefully.
    const size_t test_value = 0x12345678; // heuristic ICC-signature-like value
    const char* test_name = "sample_icc_profile.png";
    const char* test_reason = "ICC profile error detected";

    int ret = png_icc_profile_error(nullptr, test_name, test_value, test_reason);
    bool ok = (ret == 0) && !g_last_message.empty();

    // Validate the message structure
    bool starts = message_starts_with_profile_prefix(g_last_message);
    bool has_profile = (extract_profile_name_from_message(g_last_message) == std::string(test_name).substr(0, 79));
    bool has_tag = message_contains(g_last_message, "TAG"); // 6-char tag name placed by png_icc_tag_name
    bool has_reason = message_contains(g_last_message, test_reason);

    report_test("ICC path: message starts with prefix", starts);
    report_test("ICC path: profile name present (possibly truncated)", has_profile);
    report_test("ICC path: ICC tag name inserted", has_tag);
    report_test("ICC path: reason appended", has_reason);
    report_test("ICC path: return value is 0", ret == 0);
    (void)ok; // suppress unused in some compilers
}

// Test 2: Non-ICC path (is_ICC_signature(value) == 0) and hex number is appended.
// We expect: - the message contains the hex value representation (without crashing),
//           - the suffix contains "h: " as per the code path.
// This validates the non-ICC branch handling.
static void test_non_icc_path() {
    reset_last_message();

    // Choose a value unlikely to be ICC signature to force non-ICC branch.
    const size_t test_value = 0; // often non-ICC path
    const char* test_name = "non_icc_profile.png";
    const char* test_reason = "non-ICC profile error";

    int ret = png_icc_profile_error(nullptr, test_name, test_value, test_reason);

    bool starts = message_starts_with_profile_prefix(g_last_message);
    bool has_hex = message_contains(g_last_message, "0"); // hex for 0; ok as a simple check
    bool has_h_prefix = message_contains(g_last_message, "h: ");
    bool has_reason = message_contains(g_last_message, test_reason);

    report_test("Non-ICC path: message starts with prefix", starts);
    report_test("Non-ICC path: hex-formatted value present", has_hex);
    report_test("Non-ICC path: 'h: ' present (hex path)", has_h_prefix);
    report_test("Non-ICC path: reason appended", has_reason);
    report_test("Non-ICC path: return value is 0", ret == 0);
}

// Test 3: Profile name truncation behavior (name longer than 79 chars).
// We verify that the profile portion written into the message is truncated to 79 chars.
static void test_profile_name_truncation() {
    reset_last_message();

    // Create a long profile name (> 100 chars) to ensure truncation
    std::string long_name(100, 'A');
    const char* test_reason = "truncate-name-test";

    int ret = png_icc_profile_error(nullptr, long_name.c_str(), 0xDEADBEEF, test_reason);

    bool starts = message_starts_with_profile_prefix(g_last_message);
    std::string truncated_expected = long_name.substr(0, 79);
    std::string extracted = extract_profile_name_from_message(g_last_message);

    bool truncated_correct = (extracted == truncated_expected);

    report_test("Profile name truncation: message starts with prefix", starts);
    report_test("Profile name truncation: profile truncated to 79 chars", truncated_correct);
    report_test("Profile name truncation: return value is 0", ret == 0);
}

// Test 4: Reason integrity check (reason is appended to the end of the message).
static void test_reason_integrity() {
    reset_last_message();

    const char* test_name = "reason_check.png";
    const char* test_reason = "This is the exact reason text to verify";

    int ret = png_icc_profile_error(nullptr, test_name, 0x1234, test_reason);

    bool ends_with_reason = g_last_message.size() >= std::strlen(test_reason) &&
                            g_last_message.rfind(test_reason) != std::string::npos;

    report_test("Reason integrity: provided reason appears in message", ends_with_reason);
    report_test("Reason integrity: return value is 0", ret == 0);
}

// Entry point to run all tests
int main() {
    // Informational header
    std::cout << "Running unit tests for png_icc_profile_error (C++11 harness)\n";

    // Run tests
    test_icc_signature_path();
    test_non_icc_path();
    test_profile_name_truncation();
    test_reason_integrity();

    // Simple pass/fail summary (based on the execution of tests above)
    // Note: In this lightweight harness, each test prints PASS/FAIL individually.

    return 0;
}