// High-quality C++11 test suite for the focal C function zSkipValidUtf8
// This test harness is designed to be compiled alongside the C source (console_io.c)
// containing the function:
//     const char* zSkipValidUtf8(const char *z, int nAccept, long ccm);
// The tests are written in pure C++11, without Google Test, and rely only on the C
// interface provided by the focal function.
//
// Notes:
// - All tests use small, self-contained buffers to avoid reading beyond allocated memory.
// - We cover ASCII paths, multi-byte UTF-8 sequences, error paths, and limit-triggered paths.
// - Assertions are non-terminating by design; test results are reported at the end.
// - The test file declares the focal function with C linkage to avoid name mangling.

#include <iostream>
#include <vector>
#include <string>


// Link to the focal C function
extern "C" const char* zSkipValidUtf8(const char *z, int nAccept, long ccm);

struct TestCase {
    std::string name;
    // Input buffer and parameters
    std::vector<char> buf;
    int nAccept;
    long ccm;

    // Expected result pointer (as offset from buf.data())
    // We store -1 for "unknown/undefined" when we can't easily compute offset;
    // but in our tests we compute exact expected offsets.
    ptrdiff_t expectedOffset;
    bool expectValid; // whether we expect a valid computation (true/false not used for offset here)
};

// Helper: run a single test case and compare result offset
static bool run_case(const TestCase& tc, int& fail_out) {
    const char* base = tc.buf.data();
    const char* res = zSkipValidUtf8(base, tc.nAccept, tc.ccm);
    // Compute actual offset relative to base
    ptrdiff_t actualOffset = (res >= base) ? (res - base) : -1;

    bool ok = (actualOffset == tc.expectedOffset);
    if (!ok) {
        ++fail_out;
        std::cerr << "Test failed: " << tc.name << "\n";
        std::cerr << "  Base address: " << static_cast<const void*>(base) << "\n";
        std::cerr << "  nAccept=" << tc.nAccept << ", ccm=" << tc.ccm << "\n";
        std::cerr << "  Expected offset: " << tc.expectedOffset
                  << ", Actual offset: " << actualOffset << "\n";
        // Optional: print the relevant bytes for debugging
        std::cerr << "  Buffer:";
        for (size_t i = 0; i < tc.buf.size(); ++i) {
            unsigned char b = static_cast<unsigned char>(tc.buf[i]);
            std::cerr << " " << std::hex << static_cast<int>(b) << std::dec;
        }
        std::cerr << "\n";
        // Also print a small ASCII preview
        std::cerr << "  Preview (ASCII):";
        for (size_t i = 0; i < tc.buf.size(); ++i) {
            unsigned char b = static_cast<unsigned char>(tc.buf[i]);
            if (b >= 32 && b <= 126) std::cerr << static_cast<char>(b);
            else std::cerr << '.';
        }
        std::cerr << "\n";
    }
    return ok;
}

// Main test entry
int main() {
    int failures = 0;

    // 1) ASCII simple path within limit: "A B C" with nAccept=3, ccm=0
    // Expect to stop at the end of the 3-byte segment -> offset 3
    {
        TestCase tc;
        tc.name = "ASCII_basic_within_limit";
        tc.buf = {'A','B','C','\0'}; // length 4, limit at +3 points to '\0'
        tc.nAccept = 3;
        tc.ccm = 0;
        tc.expectedOffset = 3; // after 'A','B','C'
        run_case(tc, failures);
    }

    // 2) ASCII path with a control character that is permitted by ccm
    // Buffer: A, 0x02, B, 0 '\0'. nAccept=3, ccm has bit for 0x02
    // Expect to return at the position of 0x02
    {
        TestCase tc;
        tc.name = "ASCII_control_char_blocked_by_ccm";
        tc.buf = {'A', '\x02', 'B', '\0'};
        tc.nAccept = 3;
        tc.ccm = (1L << 2); // allow control 0x02
        tc.expectedOffset = 1; // pointer should stop at the 0x02 byte
        run_case(tc, failures);
    }

    // 3) UTF-8 2-byte sequence valid (é) -> bytes: 0xC3 0xA9
    // nAccept=2, ccm=0; expect to advance past the two bytes -> offset 2
    {
        TestCase tc;
        tc.name = "UTF8_two_byte_sequence_valid";
        tc.buf = {static_cast<char>(0xC3), static_cast<char>(0xA9), '\0'};
        tc.nAccept = 2;
        tc.ccm = 0;
        tc.expectedOffset = 2;
        run_case(tc, failures);
    }

    // 4) UTF-8 invalid trailing byte following a lead (0xE2, 0x28, 0x00)
    // nAccept=2; expect to fail within the sequence and return at the lead (offset 0)
    {
        TestCase tc;
        tc.name = "UTF8_invalid_trailing_byte_should_fail_at_lead";
        tc.buf = {static_cast<char>(0xE2), static_cast<char>(0x28), '\0'};
        tc.nAccept = 2;
        tc.ccm = 0;
        tc.expectedOffset = 0; // lead byte not fully satisfied, we return at z
        run_case(tc, failures);
    }

    // 5) pcLimit prevents finishing a multi-byte sequence
    // Lead 0xC3 followed by 0xA9; nAccept=2 would normally finish, but set limit to 1 byte
    // Expect to stop at the lead (offset 0)
    {
        TestCase tc;
        tc.name = "UTF8_limit_stops_before_continuations";
        tc.buf = {static_cast<char>(0xC3), static_cast<char>(0xA9), '\0'};
        tc.nAccept = 1; // limit is base+1
        tc.ccm = 0;
        tc.expectedOffset = 0; // cannot read continuation due to limit
        run_case(tc, failures);
    }

    // 6) Negative nAccept (-1) => examine only the first character, then stop
    // Input: "A" '\0'; expect pointer after 'A'
    {
        TestCase tc;
        tc.name = "Negative_nAccept_examines_one_char";
        tc.buf = {'A', '\0'};
        tc.nAccept = -1; // unlimited negative means 1 iteration
        tc.ccm = 0;
        tc.expectedOffset = 1; // after 'A'
        run_case(tc, failures);
    }

    // 7) ASCII with nAccept=0 should not advance (no bytes scanned)
    // Input: "ABC\0"; limit is 0 bytes, expect same pointer
    {
        TestCase tc;
        tc.name = "ASCII_with_zero_limit_no_advance";
        tc.buf = {'A','B','C','\0'};
        tc.nAccept = 0;
        tc.ccm = 0;
        tc.expectedOffset = 0; // no progress
        run_case(tc, failures);
    }

    // 8) ASCII with limit exactly at first byte: expect advancement to 1
    // Buffer "A B C \0"; nAccept=1; limit at base+1
    {
        TestCase tc;
        tc.name = "ASCII_limit_at_first_byte";
        tc.buf = {'A','B','C','\0'};
        tc.nAccept = 1;
        tc.ccm = 0;
        tc.expectedOffset = 1; // stop after first byte due to limit
        run_case(tc, failures);
    }

    // 9) UTF-8 3-byte sequence (Euro sign U+20AC): 0xE2 0x82 0xAC
    // nAccept=3, ccm=0 -> expect offset 3
    {
        TestCase tc;
        tc.name = "UTF8_three_byte_sequence_valid";
        tc.buf = {static_cast<char>(0xE2), static_cast<char>(0x82), static_cast<char>(0xAC), '\0'};
        tc.nAccept = 3;
        tc.ccm = 0;
        tc.expectedOffset = 3;
        run_case(tc, failures);
    }

    // Final summary
    if (failures == 0) {
        std::cout << "All tests PASSED (" << 8 << " tests executed).\n";
        return 0;
    } else {
        std::cout << "Tests completed with " << failures << " failure(s).\n";
        return 1;
    }
}