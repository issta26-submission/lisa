// Candidate Keywords extracted from the focal method SaveMemoryBlock
// - Buffer: source data to write
// - dwLen: length to write
// - Filename: destination file path
// - fopen: open file for binary write
// - fwrite: perform the write operation
// - fclose: flush and close the file
// - FatalError: error reporting helper used on failure
// - TRUE/FALSE: return values of cmsBool
// These keywords guide the test cases to exercise the key control paths.

#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstdarg>
#include <utils.h>


// Lightweight type aliases to match the focal method's signature
typedef int cmsBool;
typedef unsigned char cmsUInt8Number;
typedef unsigned int  cmsUInt32Number;

// Declaration of the focal function under test (C linkage for interoperability)
extern "C" cmsBool SaveMemoryBlock(const cmsUInt8Number* Buffer, cmsUInt32Number dwLen, const char* Filename);

// Provide a test-friendly FatalError implementation that does not terminate the program.
// It captures the error message for assertions and counts how many times it was invoked.
static int g_fatal_calls = 0;
static std::string g_last_fatal;

extern "C" void FatalError(const char* frm, ...)
{
    ++g_fatal_calls;
    char buffer[1024];
    va_list ap;
    va_start(ap, frm);
    vsnprintf(buffer, sizeof(buffer), frm, ap);
    va_end(ap);
    g_last_fatal = buffer;
}

// Test 1: Validate SaveMemoryBlock succeeds for a normal write path.
// - Writes a known buffer to a test file.
// - Verifies the file contents match the buffer.
// - Verifies that FatalError was not invoked.
// - Cleans up the created file.
bool test_SaveMemoryBlock_Success()
{
    // Exemplar data to persist
    const char data[] = "HelloWorld";
    const cmsUInt32Number len = static_cast<cmsUInt32Number>(sizeof(data) - 1); // exclude NUL

    const char* filename = "test_output.bin";

    // Ensure a clean slate
    std::remove(filename);

    // Reset error capture
    g_fatal_calls = 0;
    g_last_fatal.clear();

    // Act: call the focal function
    cmsBool result = SaveMemoryBlock(reinterpret_cast<const cmsUInt8Number*>(data), len, filename);

    // Assert: successful write
    if (result == 0)
    {
        std::cerr << "[Test SaveMemoryBlock_Success] Expected TRUE return, got FALSE.\n";
        return false;
    }

    // Verify file contents
    FILE* f = std::fopen(filename, "rb");
    if (!f)
    {
        std::cerr << "[Test SaveMemoryBlock_Success] Unable to open written file for verification.\n";
        return false;
    }
    char* readBuf = new char[len];
    size_t rw = std::fread(readBuf, 1, len, f);
    std::fclose(f);

    bool contentsMatch = (rw == static_cast<size_t>(len)) && (std::memcmp(readBuf, data, len) == 0);
    delete[] readBuf;

    if (!contentsMatch)
    {
        std::cerr << "[Test SaveMemoryBlock_Success] File contents do not match expected data.\n";
        return false;
    }

    // FatalError should not have been called in the success path
    if (g_fatal_calls != 0)
    {
        std::cerr << "[Test SaveMemoryBlock_Success] Unexpected FatalError invocation: " << g_fatal_calls
                  << " last: " << g_last_fatal << "\n";
        return false;
    }

    // Cleanup
    std::remove(filename);
    return true;
}

// Test 2: Validate SaveMemoryBlock handles failure to create/open the destination file.
// - Attempts to write to a non-existent directory to trigger fopen failure (out == NULL branch).
// - Verifies the function returns FALSE.
// - Verifies FatalError reports the "Cannot create" message.
// - No file is created in this scenario.
bool test_SaveMemoryBlock_OpenFail()
{
    const char data[] = "abc";
    const cmsUInt32Number len = static_cast<cmsUInt32Number>(sizeof(data) - 1);

    // Deliberately non-existent path to force fopen to fail on most platforms
    const char* filename = "nonexistent_dir/subdir/test.bin";

    // Reset error capture
    g_fatal_calls = 0;
    g_last_fatal.clear();

    cmsBool result = SaveMemoryBlock(reinterpret_cast<const cmsUInt8Number*>(data), len, filename);

    // Assert: failure path triggered
    if (result != 0)
    {
        std::cerr << "[Test SaveMemoryBlock_OpenFail] Expected FALSE return when fopen fails.\n";
        return false;
    }

    // Assert: FatalError was invoked
    if (g_fatal_calls <= 0)
    {
        std::cerr << "[Test SaveMemoryBlock_OpenFail] Expected FatalError to be called on fopen failure.\n";
        return false;
    }

    // Assert: FatalError message contains guidance about creation
    if (g_last_fatal.find("Cannot create") == std::string::npos)
    {
        std::cerr << "[Test SaveMemoryBlock_OpenFail] FatalError message did not contain expected text. Message: "
                  << g_last_fatal << "\n";
        return false;
    }

    return true;
}

// Minimal test harness
int main()
{
    int total = 0;
    int passed = 0;

    std::cout << "Running unit tests for SaveMemoryBlock...\n";

    // Test 1: success path (false branches of all predicates)
    ++total;
    if (test_SaveMemoryBlock_Success()) {
        ++passed;
        std::cout << "[PASS] test_SaveMemoryBlock_Success\n";
    } else {
        std::cout << "[FAIL] test_SaveMemoryBlock_Success\n";
    }

    // Test 2: fopen failure path (true branch of first predicate)
    ++total;
    if (test_SaveMemoryBlock_OpenFail()) {
        ++passed;
        std::cout << "[PASS] test_SaveMemoryBlock_OpenFail\n";
    } else {
        std::cout << "[FAIL] test_SaveMemoryBlock_OpenFail\n";
    }

    std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";
    return (passed == total) ? 0 : 1;
}