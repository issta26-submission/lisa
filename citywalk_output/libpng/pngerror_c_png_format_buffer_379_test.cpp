/*
Step 1 (Program Understanding): The focal method png_format_buffer converts a 4-byte PNG chunk name
into a printable representation. If a byte is an ASCII letter, it is emitted as that letter;
otherwise it is emitted as [xy], where xy is the hex value of the byte (lowercase hex digits).
If an error_message is provided, the method appends ": " followed by up to PNG_MAX_ERROR_TEXT-1
characters from error_message and terminates the string. If error_message is NULL, the buffer is
nul-terminated immediately after the chunk representation.

Test strategy (Step 2/3): Create a small C++ test suite (no Google Test) that exercises true/false
branches by feeding different chunk_name values and error_message inputs. Use a minimal fake
struct compatible with the png_ptr parameter's layout just enough for the test to compile and run.
Verify both the exact output strings and boundary behavior (e.g., error_message NULL vs non-NULL,
and truncation behavior using PNG_MAX_ERROR_TEXT).
All tests are written to be executable in a C++11 compliant environment and do not rely on GTest.
*/

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>
#include <png.h>


extern "C" {
}

// We rely on the png_format_buffer implementation being linked from pngerror.c (as per the focal method).

// Simple test harness: non-terminating assertions. Each test returns a bool indicating pass/fail.
static bool test_png_format_buffer_all_alpha();
static bool test_png_format_buffer_mixed_alpha_nonalpha();
static bool test_png_format_buffer_with_error_message();
static bool test_png_format_buffer_error_text_truncation();

// Helper: print test result
static inline void print_test_result(const std::string& name, bool passed) {
    if (passed) {
        std::cout << "[PASS] " << name << "\n";
    } else {
        std::cerr << "[FAIL] " << name << "\n";
    }
}

// Test 1: All four bytes are ASCII letters -> output should be the 4-letter chunk directly.
static bool test_png_format_buffer_all_alpha() {
    // Step 2: Set up a fake png_ptr with chunk_name = "ABCD"
    struct fake_png {
        png_uint_32 chunk_name;
    } s;

    s.chunk_name = (static_cast<uint32_t>('A') << 24) |
                   (static_cast<uint32_t>('B') << 16) |
                   (static_cast<uint32_t>('C') << 8)  |
                   static_cast<uint32_t>('D');

    png_const_structrp p = reinterpret_cast<png_const_structrp>(&s);

    // Buffer to receive output
    char buffer[256];
    // Call focal method (NULL error_message)
    png_format_buffer(p, buffer, nullptr);

    // Expected: "ABCD" with '\0' terminator
    const char* expected = "ABCD";
    bool ok = std::strcmp(buffer, expected) == 0;
    if (!ok) {
        std::cerr << "test_png_format_buffer_all_alpha: got '"
                  << buffer << "' expected '" << expected << "'\n";
    }
    return ok;
}

// Test 2: Mixed ASCII alpha and non-alpha bytes -> non-alpha bytes should be emitted as [xy]
static bool test_png_format_buffer_mixed_alpha_nonalpha() {
    // Chunk: 'A', '1', '?', 'D'  -> "A[31][3f]D"
    struct fake_png {
        png_uint_32 chunk_name;
    } s;

    s.chunk_name = (static_cast<uint32_t>('A') << 24) |
                   (static_cast<uint32_t>('1') << 16) |
                   (static_cast<uint32_t>('?') << 8)  |
                   static_cast<uint32_t>('D');

    png_const_structrp p = reinterpret_cast<png_const_structrp>(&s);

    char buffer[256];
    png_format_buffer(p, buffer, nullptr);

    const char* expected = "A[31][3f]D";
    bool ok = std::strcmp(buffer, expected) == 0;
    if (!ok) {
        std::cerr << "test_png_format_buffer_mixed_alpha_nonalpha: got '"
                  << buffer << "' expected '" << expected << "'\n";
    }
    return ok;
}

// Test 3: Non-null error_message appends after chunk representation with exact formatting.
static bool test_png_format_buffer_with_error_message() {
    // Chunk: 'A','B','C','D' -> "ABCD"
    struct fake_png {
        png_uint_32 chunk_name;
    } s;

    s.chunk_name = (static_cast<uint32_t>('A') << 24) |
                   (static_cast<uint32_t>('B') << 16) |
                   (static_cast<uint32_t>('C') << 8)  |
                   static_cast<uint32_t>('D');

    png_const_structrp p = reinterpret_cast<png_const_structrp>(&s);

    char buffer[256];
    const char* err = "ERR";
    png_format_buffer(p, buffer, err);

    const char* expected = "ABCD: ERR";
    bool ok = std::strcmp(buffer, expected) == 0;
    if (!ok) {
        std::cerr << "test_png_format_buffer_with_error_message: got '"
                  << buffer << "' expected '" << expected << "'\n";
    }
    return ok;
}

// Test 4: Error text truncation behavior when error_message is longer than PNG_MAX_ERROR_TEXT-1.
// We'll construct an error string filled with a repeating char and verify length and content up to the
// truncation point.
static bool test_png_format_buffer_error_text_truncation() {
    // Prepare a chunk with ASCII letters so the initial iout is predictable: "ABCD"
    struct fake_png {
        png_uint_32 chunk_name;
    } s;

    s.chunk_name = (static_cast<uint32_t>('A') << 24) |
                   (static_cast<uint32_t>('B') << 16) |
                   (static_cast<uint32_t>('C') << 8)  |
                   static_cast<uint32_t>('D');

    png_const_structrp p = reinterpret_cast<png_const_structrp>(&s);

    // Build a long error message: length = PNG_MAX_ERROR_TEXT (we will ensure it's longer than needed)
    // We will fill with 'Z's for the first PNG_MAX_ERROR_TEXT-1 characters, and terminator.
    size_t M = PNG_MAX_ERROR_TEXT;
    std::vector<char> error_text(M);
    for (size_t i = 0; i < M - 1; ++i) {
        error_text[i] = 'Z';
    }
    error_text[M - 1] = '\0';

    char buffer[512];
    png_format_buffer(p, buffer, error_text.data());

    // Expected total length: 4 (ABCD) + 2 (": ") + (M-1) = M + 5
    size_t expected_len = 4 + 2 + (M - 1);
    size_t actual_len = std::strlen(buffer);

    if (actual_len != expected_len) {
        std::cerr << "test_png_format_buffer_error_text_truncation: length mismatch, got "
                  << actual_len << " expected " << expected_len << "\n";
        return false;
    }

    // Ensure the content after the "ABCD: " prefix is all 'Z' for the truncated portion
    const char* prefix = "ABCD: ";
    if (std::strncmp(buffer, prefix, 6) != 0) {
        std::cerr << "test_png_format_buffer_error_text_truncation: missing prefix 'ABCD: '\n";
        return false;
    }
    // Check that the next (M-1) characters are 'Z'
    for (size_t i = 6; i < 6 + M - 1; ++i) {
        if (buffer[i] != 'Z') {
            std::cerr << "test_png_format_buffer_error_text_truncation: unexpected char at pos "
                      << i << " = '" << buffer[i] << "'\n";
            return false;
        }
    }
    // Null terminator should be at position 6 + (M-1)
    if (buffer[6 + M - 1] != '\0') {
        std::cerr << "test_png_format_buffer_error_text_truncation: missing NUL terminator\n";
        return false;
    }

    return true;
}

int main() {
    // Run tests (Step 3: Test Case Refinement)
    bool pass_all = true;

    bool t1 = test_png_format_buffer_all_alpha();
    print_test_result("test_png_format_buffer_all_alpha", t1);
    pass_all = pass_all && t1;

    bool t2 = test_png_format_buffer_mixed_alpha_nonalpha();
    print_test_result("test_png_format_buffer_mixed_alpha_nonalpha", t2);
    pass_all = pass_all && t2;

    bool t3 = test_png_format_buffer_with_error_message();
    print_test_result("test_png_format_buffer_with_error_message", t3);
    pass_all = pass_all && t3;

    bool t4 = test_png_format_buffer_error_text_truncation();
    print_test_result("test_png_format_buffer_error_text_truncation", t4);
    pass_all = pass_all && t4;

    if (!pass_all) {
        std::cerr << "One or more tests failed.\n";
        return 1;
    }
    std::cout << "All tests passed.\n";
    return 0;
}