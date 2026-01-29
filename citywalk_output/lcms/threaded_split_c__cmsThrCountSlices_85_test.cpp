// Self-contained unit test suite for the focal method logic (_cmsThrCountSlices)
// Note: This test suite provides a self-contained, deterministic mock of the relevant
// dependencies to exercise the core decision logic of WorkerCount calculation.
// The tests do not rely on GTest or external libraries and are built with C++11.
// Each test includes comments describing its intent and coverage.

// Includes
#include <cstring>
#include <threaded_internal.h>
#include <iostream>
#include <string>
#include <cstdarg>
#include <cstdint>


// Typedefs and minimal structures to mirror the original signature
typedef uint32_t cmsUInt32Number;
typedef int32_t  cmsInt32Number;

struct _cmstransform_struct {}; // opaque in real code; here for signature compatibility

struct cmsStride {
    cmsUInt32Number BytesPerLineIn;
    cmsUInt32Number BytesPerPlaneIn;
    cmsUInt32Number BytesPerLineOut;
    cmsUInt32Number BytesPerPlaneOut;
};

// Constants used by the focal method
static const cmsInt32Number CMS_THREADED_GUESS_MAX_THREADS = -1; // sentinel value

// Global test state to observe warnings/errors from the focal logic
static bool g_warning_seen = false;
static std::string g_last_warning_message;

// Helper to reset warning state between tests
static void reset_warning_state() {
    g_warning_seen = false;
    g_last_warning_message.clear();
}

// Mocked dependency implementations to drive deterministic behavior

// Simulated ideal thread count (as used by _cmsThrCountSlices)
static cmsInt32Number _cmsThrIdealThreadCount() {
    return 4; // deterministic value for testing
}

// Mock of the error signaling facility used by the focal logic
// Accepts a format like the real cmsSignalError; simply records a warning message.
static void cmsSignalError(void* /*clientData*/, int /*errorCode*/, const char* fmt, ...) {
    g_warning_seen = true;
    char buffer[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, ap);
    va_end(ap);
    g_last_warning_message = buffer;
}

// Simulated memory sizing function used by the focal logic
// Returns a deterministic memory footprint based on the request dimensions and formats.
// Also writes preferred bytes-per-line/plane into the provided pointers (if non-null).
static cmsUInt32Number MemSize(cmsUInt32Number /*format*/, 
                               cmsUInt32Number PixelsPerLine, 
                               cmsUInt32Number LineCount, 
                               cmsUInt32Number* BytesPerLine,
                               cmsUInt32Number BytesPerPlane)
{
    // Provide a reasonable, deterministic per-line/plane size
    if (BytesPerLine) *BytesPerLine = PixelsPerLine * 16u; // arbitrary but consistent
    if (BytesPerPlane) *BytesPerPlane = 0u;
    // Deterministic memory estimate: PixelsPerLine * LineCount * 1024 (roughly 1KB per unit)
    return PixelsPerLine * LineCount * 1024u;
}

// The focal logic under test, reproduced in this test file to isolate behavior.
// It mirrors the structure in the provided snippet and uses the mocked dependencies above.
static cmsUInt32Number _cmsThrCountSlices(struct _cmstransform_struct* CMMcargo,
                                   cmsInt32Number MaxWorkers,
                                   cmsUInt32Number PixelsPerLine,
                                   cmsUInt32Number LineCount,
                                   cmsStride* Stride)
{
    cmsInt32Number MaxInputMem,  MaxOutputMem;
    cmsInt32Number WorkerCount;
    cmsInt32Number MaxCPUs = _cmsThrIdealThreadCount();

    if (MaxWorkers == CMS_THREADED_GUESS_MAX_THREADS) {
        MaxWorkers = MaxCPUs;
    } else {
        // We allow large number of threads, but this is not going to work well. Warn it. 
        if (MaxWorkers > MaxCPUs) {
            cmsSignalError(NULL, /*cmsERROR_RANGE*/ 1,
                "Warning: too many threads for actual processor (CPUs=%d, asked=%d)", MaxCPUs, MaxWorkers);
        }
    }

    MaxInputMem = MemSize(0, PixelsPerLine, LineCount, &Stride->BytesPerLineIn, Stride->BytesPerPlaneIn);
    MaxOutputMem = MemSize(0, PixelsPerLine, LineCount, &Stride->BytesPerLineOut, Stride->BytesPerPlaneOut);

    // Each thread takes 128K at least
    WorkerCount = (MaxInputMem + MaxOutputMem) / (128 * 1024);
    if (WorkerCount < 1)
        WorkerCount = 1;
    else
        if (WorkerCount > MaxWorkers)
            WorkerCount = MaxWorkers;
    return WorkerCount;
}

// Simple assertion helper that does not terminate the program on failure
#define ASSERT_EQ(actual, expected, msg) do { \
    if ((actual) != (expected)) { \
        std::cerr << "ASSERT FAILED: " << (msg) \
                  << " | expected: " << (expected) \
                  << ", actual: " << (actual) << std::endl; \
    } else { \
        std::cout << "ASSERT PASSED: " << (msg) << std::endl; \
    } \
} while (0)

// Test 1: When MaxWorkers == CMS_THREADED_GUESS_MAX_THREADS, the code should floor to _cmsThrIdealThreadCount()
void test_case_guessMaxThreads_sets_to_MaxCPUs() {
    // Arrange
    cmsStride stride = {0, 0, 0, 0};
    reset_warning_state();

    // Act
    cmsUInt32Number result = _cmsThrCountSlices(nullptr, CMS_THREADED_GUESS_MAX_THREADS,
                                               64, 8, &stride);

    // The expected behavior:
    // - _cmsThrIdealThreadCount() returns 4
    // - Initial MemSize with PixelsPerLine=64, LineCount=8 yields 64*8*1024 per input
    //   twice (input + output) -> 1,048,576 bytes total -> WorkerCount = 8
    // - Since MaxWorkers is now 4, WorkerCount should be clamped to 4
    cmsUInt32Number expected = 4u;

    // Assert
    ASSERT_EQ(result, expected, "When using CMS_THREADED_GUESS_MAX_THREADS, should clamp to MaxCPUs (4)");
}

// Test 2: When MaxWorkers exceeds CPU count, a warning should be signaled
void test_case_warns_when_workers_exceed_CPUs() {
    // Arrange
    cmsStride stride = {0, 0, 0, 0};
    reset_warning_state();

    // Act
    cmsUInt32Number result = _cmsThrCountSlices(nullptr, 10, // MaxWorkers > MaxCPUs (4)
                                               64, 8, &stride);

    // The expected behavior:
    // - A warning is signaled (g_warning_seen == true)
    // - WorkerCount initial = 8 (as in Test 1); final = min(8, 10) -> 8
    cmsUInt32Number expected = 8u;

    // Assert
    if (!g_warning_seen) {
        std::cerr << "ASSERT FAILED: expected warning when MaxWorkers > CPUs" << std::endl;
    } else {
        std::cout << "ASSERT PASSED: warning signaled when MaxWorkers > CPUs" << std::endl;
    }
    ASSERT_EQ(result, expected, "WorkerCount should be 8 when MaxWorkers = 10 and MemSize yields 8 workers");
}

// Test 3: Very small workload should yield at least 1 worker
void test_case_minimum_worker_count_is_one() {
    // Arrange
    cmsStride stride = {0, 0, 0, 0};
    reset_warning_state();

    // Act
    // Use very small PixelsPerLine/LineCount to minimize memory footprint
    cmsUInt32Number result = _cmsThrCountSlices(nullptr, 2, // MaxWorkers = 2
                                               1, 1, &stride);

    // With 1*1*1024 per input and 1*1*1024 per output, total = 2048
    // WorkerCount = 2048 / 131072 = 0 -> adjusted to 1
    cmsUInt32Number expected = 1u;

    // Assert
    ASSERT_EQ(result, expected, "Minimum worker count should be 1 for tiny workloads");
}

// Test 4: When MemSize yields a larger worker count, it should clamp to MaxWorkers
void test_case_workercount_clamped_to_MaxWorkers() {
    // Arrange
    cmsStride stride = {0, 0, 0, 0};
    reset_warning_state();

    // Act
    // This should produce a larger initial WorkerCount (8) which is then clamped to MaxWorkers (5)
    cmsUInt32Number result = _cmsThrCountSlices(nullptr, 5, // MaxWorkers = 5
                                               64, 8, &stride);

    cmsUInt32Number expected = 5u;

    // Assert
    ASSERT_EQ(result, expected, "WorkerCount should clamp to MaxWorkers when initial count exceeds it");
}

// Entry point: Run all tests
int main() {
    std::cout << "Running test suite for _cmsThrCountSlices (mocked dependencies)\n";

    test_case_guessMaxThreads_sets_to_MaxCPUs();
    test_case_warns_when_workers_exceed_CPUs();
    test_case_minimum_worker_count_is_one();
    test_case_workercount_clamped_to_MaxWorkers();

    std::cout << "Test suite completed.\n";
    return 0;
}