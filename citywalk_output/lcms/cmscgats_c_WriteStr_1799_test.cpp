// Test suite for the focal method: WriteStr in cmscgats.c
// Objective: exercise true/false branches of WriteStr without using GTest.
// Approach:
// - Compile this test together with cmscgats.c (C source) under C++11.
// - Use the actual SAVESTREAM type provided by the project (via cmscgats.c and cms headers).
// - Provide a light-weight, non-terminating test harness (no aborts on failure).
// - Validate stream write path, NULL-string handling, memory buffer path, and
//   memory overflow behavior (branch coverage).

// Notes:
// - We avoid terminating asserts; instead, each test returns a bool indicating success.
// - The tests rely on the real types and behavior defined by cmscgats.c and the lcms2 headers.
// - A minimal cmsSignalError stub is provided to satisfy linkage if the project does not
//   inject its own implementation. We intentionally implement it to do nothing (non-fatal).

#define cmsERROR_WRITE 1  // Ensure the error code constant matches the value expected by tests

#include <cstring>
#include <cmscgats.c>
#include <cstdio>
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <cassert>
#include <lcms2_internal.h>


// Bring in necessary definitions from the project under test.
// We compile this test together with the focal cmscgats.c file.
// Expose C linkage for the included C sources to avoid C++ name mangling issues.
extern "C" {
}

// If the project provides its own cmsSignalError, we override it here to capture/neutralize signals.
// The WriteStr implementation calls cmsSignalError on error paths. We provide a stub that does nothing.
// This ensures tests can run without terminating and without requiring a full CGATS parser environment.
extern "C" void cmsSignalError(int, int, const char*) {
    // Do nothing for test harness. We could log if needed, but we keep non-terminating behavior.
}

// Helper: Non-terminating, simple test result reporter
static void RunTest(const std::string& name, bool ok) {
    std::cout << "[Test] " << name << ": " << (ok ? "PASS" : "FAIL") << std::endl;
}

// Utility: Read entire file content for stream verification
static std::string ReadFileContent(const char* path) {
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs) return "";
    std::string res((std::istreambuf_iterator<char>(ifs)),
                    (std::istreambuf_iterator<char>()));
    return res;
}

/*
Test 1: WriteStr writes to a FILE* stream correctly when a valid string is provided.
- Setup: f.stream is a valid FILE*, f.Base==nullptr, f.Ptr==nullptr, Used==0, Max sufficiently large.
- Action: WriteStr(f, "ABC")
- Verify: File contains exactly "ABC" and Used==3
*/
static bool test_WriteStr_StreamWritesABC() {
    const char* filePath = "test_write_str_stream_abc.cgats";
    // Ensure any previous file is removed
    std::remove(filePath);

    SAVESTREAM f;
    // Initialize fields; the exact SAVESTREAM layout is provided by the project headers.
    f.Used = 0;
    f.stream = std::fopen(filePath, "wb");
    if (f.stream == nullptr) {
        std::cerr << "Failed to open test output file for stream write." << std::endl;
        return false;
    }
    f.Base = nullptr;
    f.Ptr = nullptr;
    f.Max = 1024;

    // Call the focal method
    WriteStr(&f, "ABC");

    // Cleanup and verify
    std::fclose(f.stream);
    std::string content = ReadFileContent(filePath);
    std::remove(filePath);

    // Expect exactly "ABC"
    bool ok = (content == "ABC");
    if (!ok) {
        std::cerr << "Expected file content 'ABC', got: '" << content << "'" << std::endl;
    }
    // Also verify that the internal 'Used' counter reflects the written bytes
    // (Note: We assume the project uses 3 for "ABC")
    ok = ok && (f.Used == 3);
    if (!ok) {
        std::cerr << "Expected Used == 3 after writing 'ABC', got: " << f.Used << std::endl;
    }
    return ok;
}

/*
Test 2: WriteStr handles NULL input by substituting a single space (" ").
- Setup: f.stream is a valid FILE*, f.Base==nullptr, f.Ptr==nullptr, Used==0, Max large.
- Action: WriteStr(f, NULL)
- Verify: File contains a single space " " and Used==1
*/
static bool test_WriteStr_NullStringStream() {
    const char* filePath = "test_write_str_null_space.cgats";
    std::remove(filePath);

    SAVESTREAM f;
    f.Used = 0;
    f.stream = std::fopen(filePath, "wb");
    if (f.stream == nullptr) {
        std::cerr << "Failed to open test output file for NULL string test." << std::endl;
        return false;
    }
    f.Base = nullptr;
    f.Ptr = nullptr;
    f.Max = 1024;

    WriteStr(&f, nullptr);

    std::fclose(f.stream);
    std::string content = ReadFileContent(filePath);
    std::remove(filePath);

    bool ok = (content == " ");
    if (!ok) {
        std::cerr << "NULL input did not produce a single space. Got: '" << content << "'" << std::endl;
        // Provide more debugging detail if needed
    }
    ok = ok && (f.Used == 1);
    if (!ok) {
        std::cerr << "Expected Used == 1 after NULL input, got: " << f.Used << std::endl;
    }
    return ok;
}

/*
Test 3: WriteStr writes to a memory buffer when stream is NULL and Base is provided.
- Setup: f.Base points to a buffer, f.Ptr points to same buffer start, Used==0, Max large.
- Action: WriteStr(f, "DATA")
- Verify: Buffer contains "DATA" at start; Ptr advanced by 4; Used == 4
*/
static bool test_WriteStr_MemoryWritesData() {
    unsigned char mem[16];
    // Initialize with non-empty data to ensure no accidental overwrite beyond "DATA"
    mem[0] = 'W'; mem[1] = 'X'; mem[2] = 'Y'; mem[3] = 'Z'; mem[4] = 0;

    SAVESTREAM f;
    // Memory path
    f.Used = 0;
    f.stream = nullptr;
    f.Base = mem;
    f.Ptr = mem;
    f.Max = sizeof(mem);

    WriteStr(&f, "DATA");

    // Verify memory content
    bool ok = (std::memcmp(mem, "DATA", 4) == 0);
    // Verify pointer advanced and Used updated
    ok = ok && (f.Ptr == mem + 4);
    ok = ok && (f.Used == 4);

    if (!ok) {
        std::cerr << "Memory write test failed. "
                  << "Expected mem[0..3]=='DATA', got '"
                  << std::string(reinterpret_cast<char*>(mem), 4) << "'; "
                  << "Ptr=" << (f.Ptr - mem) << ", Used=" << f.Used << std::endl;
    }
    return ok;
}

/*
Test 4: WriteStr handles overflow condition for the memory path.
- Setup: f.Base points to a buffer; Used is initialized such that after adding len(str) it exceeds Max.
- Action: WriteStr(f, "DATA") with initial Used+len > Max
- Verify: Used increased by len; no memory write occurs (buffer content remains unchanged)
*/
static bool test_WriteStr_MemoryOverflow() {
    unsigned char mem[16] = { 'W','O','R','K', 0 }; // initial content

    SAVESTREAM f;
    f.Used = 13;     // Pre-extend Used
    f.stream = nullptr;
    f.Base = mem;
    f.Ptr = mem;
    f.Max = 16;      // Allow only 3 more bytes; writing 4 bytes will overflow (Used becomes 17)

    // Preserve initial content for comparison after potential overflow
    unsigned char before[16];
    std::memcpy(before, mem, sizeof(mem));

    WriteStr(&f, "DATA"); // len = 4; Used becomes 17, which overflows Max

    // Verify overflow behavior
    bool ok = (f.Used == 17); // Used should have been incremented regardless of overflow
    // Ensure memory was not overwritten on overflow
    ok = ok && (std::memcmp(mem, before, 16) == 0);

    if (!ok) {
        std::cerr << "Memory overflow test failed. Used=" << f.Used
                  << ", Max=" << f.Max << ", BufChanged="
                  << (std::memcmp(mem, before, 16) != 0) << std::endl;
    }
    return ok;
}

int main() {
    // Run all tests; we collect results and print a final summary.
    int total = 0;
    int passes = 0;

    auto run = [&](const std::string& name, bool ok) {
        RunTest(name, ok);
        total++;
        if (ok) passes++;
    };

    // Step 1: Stream write path with normal string
    run("WriteStr_StreamWritesABC", test_WriteStr_StreamWritesABC());

    // Step 2: NULL input path yields a single space
    run("WriteStr_NullStringStream", test_WriteStr_NullStringStream());

    // Step 3: Memory path with normal string
    run("WriteStr_MemoryWritesData", test_WriteStr_MemoryWritesData());

    // Step 4: Memory path overflow handling
    run("WriteStr_MemoryOverflow", test_WriteStr_MemoryOverflow());

    // Final summary
    std::cout << "Summary: " << passes << " / " << total << " tests passed." << std::endl;

    // Return non-zero if any test failed to aid CI systems that check exit codes
    return (passes == total) ? 0 : 1;
}