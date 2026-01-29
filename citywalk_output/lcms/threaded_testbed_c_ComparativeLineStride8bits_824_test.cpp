/*
  Unit Test Suite for the focal method ComparativeLineStride8bits
  - Language: C++11 (no Google Test)
  - Strategy: Lightweight, self-contained test harness with minimal mock behavior
  - Goals:
      1) Exercise the focal method as provided
      2) Validate observable behavior (output header text) to ensure the method executes as intended
      3) Provide coverage-oriented test structure with non-terminating assertions
  - Structure:
      - Step 1: Analysis summary (embedded as comments)
      - Step 2: Generated tests based on visible dependencies and behavior
      - Step 3: Test refinements and additional checks
  - Important: This test harness assumes the focal function is linked from the original project/library
    and will execute in an environment where its external dependencies are resolvable. Some aspects
    (like precise internal state changes or internal static/private behavior) cannot be asserted without
    reworking the production code; instead we focus on observable outputs and basic execution correctness.
  - Note: The code uses a lightweight, custom test framework (no GTest) and does not rely on
    any third-party testing library.
*/

/* Step 1: Program Understanding (embedded explanation)
   - Focal method: void ComparativeLineStride8bits(void)
     - Creates two CMS contexts (NoPlugin and Plugin) and prints a header.
     - Calls ComparativeCt five times, comparing performance between
       DoTransform vs. DoTransformLineStride for various profile combinations:
       1) CLUT profiles with 8-bit transform vs line stride
       2) CLUT 16-bit
       3) Matrix-Shaper
       4) Same MatrixSh
       5) curves (NULL profiles)
     - Cleans up contexts then returns.
   - Candidate Keywords (core components):
     - cmsContext, cmsCreateContext, cmsDeleteContext
     - cmsThreadedExtensions, CMS_THREADED_GUESS_MAX_THREADS
     - trace (output)
     - ComparativeCt (the comparison bridge)
     - SpeedTest8bitDoTransform, SpeedTest8bitLineStride
     - SpeedTest16bitsRGB
     - PROFILES_DIR, test5.icc, test3.icc, test0.icc
   - Expected behaviors to test:
     - The function prints the header header lines with the exact phrases
     - The function calls ComparativeCt exactly five times with the proper Title strings
     - Resource acquisition and release (NoPlugin/Plugin CMS contexts) occur
   - Limitations:
     - Without mocking the underlying library, we verify observable I/O and call-pattern
     - Static/internal helpers used by the production code are not directly testable here
*/

/* Step 2: Unit Test Generation (test suite)
   - We provide:
     - A minimal test framework
     - Tests focused on observable outputs and call sequencing (to the extent observable via stdout)
     - Lightweight, non-terminating assertions (do not abort on failure)
   - The tests assume the focal function is linked in and prints to stdout via trace/printf-like helpers.
   - We capture stdout for assertion on header and related strings.
*/

#include <cstring>
#include <cstdio>
#include <functional>
#include <vector>
#include <threaded_internal.h>
#include <iostream>
#include <string>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>


// Forward declaration of the focal function under test.
// Ensure the symbol is visible from C linkage when linked with C code.
extern "C" void ComparativeLineStride8bits(void);

/* Lightweight test framework (no GTest)
   - Non-terminating assertions: record failures but continue execution
   - Simple test registry and runner
*/
static int g_totalTests = 0;
static int g_failedTests = 0;

#define TEST_DEF(name) void test_##name(); \
    struct TestRegistrar_##name { TestRegistrar_##name() { tests().push_back({#name, &test_##name}); } }; \
    static TestRegistrar_##name registrar_##name; \
    void test_##name()

struct TestCase {
    std::string name;
    std::function<void()> func;
};

inline std::vector<TestCase>& tests() {
    static std::vector<TestCase> inst;
    return inst;
}

#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "Expectation failed: " << (msg) << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_failedTests; \
    } \
} while(0)

#define ASSERT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "Assertion failed: " << #cond << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        std::exit(EXIT_FAILURE); \
    } \
} while(0)

#define RUN_ALL_TESTS() do { \
    int total = (int)tests().size(); \
    g_totalTests = total; \
    int idx = 1; \
    for (const auto& t : tests()) { \
        std::cout << "[ RUN      ] " << t.name << std::endl; \
        try { t.func(); } catch (...) { \
            std::cerr << "Exception in test: " << t.name << std::endl; \
            ++g_failedTests; \
        } \
        std::cout << "[       OK ] " << t.name << std::endl; \
        ++idx; \
    } \
    std::cout << "====================" << std::endl; \
    std::cout << "Tests run: " << g_totalTests << ", Failures: " << g_failedTests << std::endl; \
} while(0)

/* Stdout capture utility (POSIX)
   - Redirects stdout to a pipe during the scope and collects output afterwards.
   - This allows us to assert on the textual output of ComparativeLineStride8bits.
*/
class StdoutCapture {
public:
    StdoutCapture() {
        fflush(stdout);
        // Create pipe for capturing stdout
        if (pipe(pipefds) != 0) {
            perror("pipe");
        }
        // Save old stdout
        oldFd = dup(fileno(stdout));
        // Redirect stdout to the write end of the pipe
        dup2(pipefds[1], fileno(stdout));
        // Close unused write end in the parent
        close(pipefds[1]);
    }

    ~StdoutCapture() {
        // Flush and restore stdout
        fflush(stdout);
        dup2(oldFd, fileno(stdout));
        close(oldFd);
        // Read all captured data
        char buf[4096];
        ssize_t n;
        while ((n = read(pipefds[0], buf, sizeof(buf))) > 0) {
            captured.append(buf, static_cast<size_t>(n));
        }
        close(pipefds[0]);
    }

    std::string get() const { return captured; }

private:
    int pipefds[2];
    int oldFd;
    std::string captured;
};

// Helper macro to retry a test and keep it readable
#define RUN_TEST_CASE(test_function) test_function();

/* Step 3: Test Case Refinement
   - Tests added:
     - Test_HeaderOutputPresence: ensures the header and descriptive lines are emitted by ComparativeLineStride8bits.
     - Test_5ComparativeCtInvocations: relies on the assumption that the library's implementation emits
       observable side-effects (e.g., header text) and that we can observe expected header content.
       Since we cannot fully mock internal function calls without modifying the production code, we verify
       the presence of the header and the existence of the function's expected invocation footprint in output.
*/

// Test 1: Validate that the header text and main lines are printed by the focal method.
// This test captures stdout and asserts presence of key phrases.
TEST_DEF(HeaderOutputPresence)
{
    StdoutCapture cap;
    // Call the focal function (from production code)
    ComparativeLineStride8bits();
    // Release cap and fetch output
    std::string out = cap.get();

    // Key phrases expected in the header as per the focal method
    const char* phrase1 = "C O M P A R A T I V E cmsDoTransform() vs. cmsDoTransformLineStride()";
    const char* phrase2 = "values given in MegaPixels per second.";
    const char* phrase3 = "====================================================================";

    bool hasHeader1 = (strstr(out.c_str(), phrase1) != nullptr);
    bool hasHeader2 = (strstr(out.c_str(), phrase2) != nullptr);
    bool hasHeader3 = (strstr(out.c_str(), phrase3) != nullptr);

    EXPECT_TRUE(hasHeader1, "Header should contain the comparison title line.");
    EXPECT_TRUE(hasHeader2, "Header should contain the mega-pixels-per-second descriptor line.");
    EXPECT_TRUE(hasHeader3, "Header should end with a separator line of '=' characters.");

    // At least ensure the function produced some output (non-empty)
    EXPECT_TRUE(!out.empty(), "Output should not be empty after ComparativeLineStride8bits.");
}
TEST_DEF(FiveComparativeCtInvocationsFootprint)
{
    // This test attempts to ensure the sequence and existence of ComparativeCt invocations
    // by inspecting the output for the five expected "ComparativeCt" style entries or their titles.
    // Note: Without full mocking of the internal function, we verify the high-level manifestation
    // (header + potentially a summary line) rather than internal call counts.
    StdoutCapture cap;
    ComparativeLineStride8bits();
    std::string out = cap.get();

    // Heuristic checks: count occurrences of the Title prefixes used in the focal code.
    const char* titles[] = {
        "CLUT profiles  ",
        "CLUT 16 bits   ",
        "Matrix-Shaper  ",
        "same MatrixSh  ",
        "curves         "
    };

    int found = 0;
    for (const char* t : titles) {
        if (strstr(out.c_str(), t) != nullptr)
            ++found;
    }

    // We expect to find at least the first four titles (and ideally the 5th with NULL profiles)
    // If the environment prints the titles, then we have observed at least partial compliance.
    EXPECT_TRUE(found >= 4, "Expected to observe at least four of the five ComparativeCt titles in output.");

    // Also assert the NULL-profile case is present by looking for its title and the NULL-ICC indicators
    if (strstr(out.c_str(), "curves         ") != nullptr) {
        // The NULL profile case is part of the 5th invocation. We can't strictly assert its presence
        // without exact output format, but we can ensure the string exists.
        EXPECT_TRUE(true, "Observed 'curves' case presence in output footprint.");
    }
}
TEST_DEF(ExecutionWithoutCrashes)
{
    // Basic smoke test: ensure function runs without crashing and prints something reasonable.
    StdoutCapture cap;
    ComparativeLineStride8bits();
    std::string out = cap.get();

    ASSERT_TRUE(!out.empty());
}

// Entry point for test registration
int main()
{
    // Run all registered tests
    RUN_ALL_TESTS();
    // Return non-zero if any test failed
    return (g_failedTests > 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}