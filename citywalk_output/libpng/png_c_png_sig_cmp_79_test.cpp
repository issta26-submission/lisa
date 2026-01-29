#include <iostream>
#include <pngpriv.h>
#include <cstring>
#include <cstddef>


// Declare the C function under test with C linkage so the linker can find it.
// png_sig_cmp takes a pointer to a PNG signature array, a start offset, and a length to check.
// We mirror the expected signature as used in the original C source.
extern "C" int png_sig_cmp(const unsigned char* sig, size_t start, size_t num_to_check);

// A local identical copy of the true PNG signature used by the function under test.
// This is only for constructing test inputs in C++ tests; the function itself
// compares against its internal static signature.
static const unsigned char kPngSignature[8] = {137, 80, 78, 71, 13, 10, 26, 10};

// Test 1: Full-length match at exact boundaries - expect 0 (equal)
bool test_full_signature_match() {
    unsigned char sig[8];
    std::memcpy(sig, kPngSignature, 8);
    int res = png_sig_cmp(sig, 0, 8);
    return (res == 0);
}

// Test 2: Partial match (first 4 bytes) at start 0 - expect 0 (equal for the checked portion)
bool test_partial_match_first4() {
    unsigned char sig[8];
    std::memcpy(sig, kPngSignature, 8);
    int res = png_sig_cmp(sig, 0, 4);
    return (res == 0);
}

// Test 3: Mismatch in the signature should yield non-zero (negative or positive depending on first mismatch)
bool test_mismatch_returns_nonzero() {
    // Change the very first byte to ensure a mismatch with the true signature
    unsigned char sig[8] = {0, 80, 78, 71, 13, 10, 26, 10};
    int res = png_sig_cmp(sig, 0, 8);
    return (res != 0);
}

// Test 4: num_to_check < 1 should return -1 (invalid length)
bool test_num_to_check_less_than_one() {
    unsigned char sig[8];
    std::memcpy(sig, kPngSignature, 8);
    int res = png_sig_cmp(sig, 0, 0); // 0 is invalid per implementation
    return (res == -1);
}

// Test 5: start > 7 should return -1 (invalid start index)
bool test_start_out_of_range() {
    unsigned char sig[8];
    std::memcpy(sig, kPngSignature, 8);
    int res = png_sig_cmp(sig, 8, 4); // start beyond the last valid index
    return (res == -1);
}

// Test 6: start + num_to_check > 8 should clamp to 8 - start; verify clamp behavior
// We purposely use a sig variant that would differ only in the clamped portion.
bool test_length_clamp_and_mismatch() {
    // Create a signature that matches up to the clamped length but differs in the last two bytes
    unsigned char sig[8] = {137, 80, 78, 71, 13, 10, 26, 9}; // last byte differs: 9 instead of 10
    // start=6, num_to_check=4 -> clamps to 2 bytes to compare (positions 6 and 7)
    int res = png_sig_cmp(sig, 6, 4);
    // We expect a mismatch in the clamped portion (26 vs 26 matches, 9 vs 10 mismatches on second byte)
    // If the clamp works and the bytes differ, res should be non-zero.
    return (res != 0);
}

int main() {
    int total = 0;
    int passed = 0;

    auto run = [&](const char* name, bool result) {
        ++total;
        if (result) {
            ++passed;
            std::cout << "[PASS] " << name << "\n";
        } else {
            std::cout << "[FAIL] " << name << "\n";
        }
    };

    // Execute tests with explanatory comments embedded in the test names
    run("test_full_signature_match (exact full-match)", test_full_signature_match());
    run("test_partial_match_first4 (start=0, length=4)", test_partial_match_first4());
    run("test_mismatch_returns_nonzero (mismatch overall)", test_mismatch_returns_nonzero());
    run("test_num_to_check_less_than_one (num_to_check < 1)", test_num_to_check_less_than_one());
    run("test_start_out_of_range (start > 7)", test_start_out_of_range());
    run("test_length_clamp_and_mismatch (length clamp when start near end)", test_length_clamp_and_mismatch());

    std::cout << "Tests completed: " << passed << " passed, " << (total - passed) << " failed." << std::endl;
    return (total == passed) ? 0 : 1;
}