/*
 * Test suite for the focal method:
 * int sqlite3VdbeAddOp4(Vdbe *p, int op, int p1, int p2, int p3,
 *                       const char *zP4, int p4type)
 *
 * This test is designed as a self-contained, C++11 compliant suite
 * without using GoogleTest. It mocks the dependent functions
 * sqlite3VdbeAddOp3 and sqlite3VdbeChangeP4 at the test translation unit
 * level to observe interactions and verify correctness.
 *
 * Important: This is a focused unit test harness intended for environments
 * where the actual SQLite implementation may not be linked. The test
 * redefines the dependencies locally (with C linkage) to enable
 * deterministic observation of behavior.
 *
 * The tests cover:
 * - Normal operation where AddOp3 returns an address and ChangeP4 is invoked with that address.
 * - Case where zP4 is nullptr to verify pointer handling.
 * - Multiple sequential invocations to ensure address increments and propagation.
 *
 * Notes:
 * - Static/global state is used to capture interaction details across calls.
 * - A lightweight test framework is implemented (non-terminating checks).
 * - All test code is contained in a single translation unit for portability.
 */

#include <iostream>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>


// Domain-specific: provide minimal C-compatible types to mimic the real API
extern "C" {
    // Forward declaration of the opaque Vdbe struct used by the API.
    typedef struct Vdbe Vdbe;
}

// Minimal Vdbe definition for test purposes
struct Vdbe {
    // Intentionally empty; the tests mock the dependent operations.
};

// Static domain-global state for test doubles
static int g_nextAddr = 1;                   // Next address returned by AddOp3
static int g_lastAddrFromAddOp3 = -1;        // Addr returned by sqlite3VdbeAddOp3
static int g_addOp3_op = 0;                  // op passed to AddOp3
static int g_addOp3_p1 = 0, g_addOp3_p2 = 0, g_addOp3_p3 = 0; // P1/P2/P3 passed to AddOp3

static const char *g_lastP4 = nullptr;       // zP4 passed to ChangeP4
static int g_lastP4type = 0;                   // p4type passed to ChangeP4
static int g_lastAddrFromChangeP4 = -1;       // addr passed to ChangeP4

// Mocked dependency: sqlite3VdbeAddOp3
extern "C" int sqlite3VdbeAddOp3(Vdbe *p, int op, int p1, int p2, int p3) {
    // Record inputs for assertions
    g_lastAddrFromAddOp3 = g_nextAddr;
    g_addOp3_op = op;
    g_addOp3_p1 = p1;
    g_addOp3_p2 = p2;
    g_addOp3_p3 = p3;

    // Return current address and advance the internal counter
    int addr = g_nextAddr;
    g_nextAddr++;
    return addr;
}

// Mocked dependency: sqlite3VdbeChangeP4
extern "C" void sqlite3VdbeChangeP4(Vdbe *p, int addr, const char *zP4, int p4type) {
    // Capture the call details for verification
    g_lastAddrFromChangeP4 = addr;
    g_lastP4 = zP4;
    g_lastP4type = p4type;
}

// Focal method under test (replicated for standalone testing)
extern "C" int sqlite3VdbeAddOp4(
  Vdbe *p,
  int op,
  int p1,
  int p2,
  int p3,
  const char *zP4,
  int p4type
){
    int addr = sqlite3VdbeAddOp3(p, op, p1, p2, p3);
    sqlite3VdbeChangeP4(p, addr, zP4, p4type);
    return addr;
}

// Lightweight test framework (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define TEST_ASSERT_EQ(expected, actual, msg) do { \
    ++g_tests_run; \
    if (!((expected) == (actual))) { \
        std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " - " << (msg) << " | Expected: " << (expected) \
                  << ", Actual: " << (actual) << std::endl; \
        ++g_tests_failed; \
    } \
} while(0)

#define TEST_ASSERT_PTR_EQ(expected, actual, msg) do { \
    ++g_tests_run; \
    if (!((expected) == (actual))) { \
        std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " - " << (msg) << " | Expected ptr: " \
                  << static_cast<const void*>(expected) \
                  << ", Actual ptr: " << static_cast<const void*>(actual) \
                  << std::endl; \
        ++g_tests_failed; \
    } \
} while(0)

#define TEST_ASSERT_STR_EQ(expected, actual, msg) do { \
    ++g_tests_run; \
    if (!((expected) && (actual) && std::strcmp((expected), (actual)) == 0)) { \
        std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " - " << (msg) << " | Expected: \"" << (expected) \
                  << "\", Actual: \"" << (actual ? actual : "null") << "\"" \
                  << std::endl; \
        ++g_tests_failed; \
    } \
} while(0)

static void test_basic_addop4_normal_case() {
    // Test that a typical call propagates through AddOp3 and ChangeP4
    // and returns the address provided by AddOp3.
    Vdbe v;
    const char *payload = "payload_for_p4";

    // Reset shared state
    g_nextAddr = 1;
    g_lastAddrFromAddOp3 = -1;
    g_lastAddrFromChangeP4 = -1;
    g_lastP4 = nullptr;
    g_lastP4type = -999;

    int ret = sqlite3VdbeAddOp4(&v, 7, 11, 22, 33, payload, 5);

    // Address should come from AddOp3 (first address)
    TEST_ASSERT_EQ(1, ret, "Return address should be equal to first AddOp3 address");

    // Ensure AddOp3 received the correct arguments
    TEST_ASSERT_EQ(7, g_addOp3_op, "AddOp3 should receive correct op value");
    TEST_ASSERT_EQ(11, g_addOp3_p1, "AddOp3 should receive correct p1 value");
    TEST_ASSERT_EQ(22, g_addOp3_p2, "AddOp3 should receive correct p2 value");
    TEST_ASSERT_EQ(33, g_addOp3_p3, "AddOp3 should receive correct p3 value");

    // Ensure ChangeP4 received the same address and correct P4 data
    TEST_ASSERT_EQ(1, g_lastAddrFromChangeP4, "ChangeP4 should be called with same address as AddOp3 result");
    TEST_ASSERT_PTR_EQ(payload, g_lastP4, "ChangeP4 should receive the correct zP4 pointer");
    TEST_ASSERT_EQ(5, g_lastP4type, "ChangeP4 should receive the correct p4type");
}

static void test_basic_addop4_null_p4() {
    // Test with zP4 = nullptr to ensure pointer handling works
    Vdbe v;
    g_nextAddr = 10; // arbitrary start
    g_lastAddrFromAddOp3 = -1;
    g_lastAddrFromChangeP4 = -1;
    g_lastP4 = reinterpret_cast<const char*>(0xDEADBEEF); // sentinel to detect changes
    g_lastP4type = -999;

    int ret = sqlite3VdbeAddOp4(&v, 3, 1, 2, 3, nullptr, 0);

    TEST_ASSERT_EQ(10, ret, "Return address should equal the next AddOp3 address when zeroed");
    TEST_ASSERT_EQ(10, g_lastAddrFromChangeP4, "ChangeP4 addr should match returned address when zP4 is nullptr");
    TEST_ASSERT_PTR_EQ(nullptr, g_lastP4, "ChangeP4 zP4 should be nullptr when passed as nullptr");
    TEST_ASSERT_EQ(0, g_lastP4type, "ChangeP4 p4type should propagate correctly when zP4 is nullptr");
}

static void test_basic_addop4_sequential_calls() {
    // Ensure multiple sequential calls increment addresses and propagate correctly
    Vdbe v;
    g_nextAddr = 100;
    g_lastAddrFromChangeP4 = -1;
    g_lastP4 = nullptr;
    g_lastP4type = -1;

    int ret1 = sqlite3VdbeAddOp4(&v, 1, 10, 20, 30, "first", 1);
    int ret2 = sqlite3VdbeAddOp4(&v, 2, -5, -10, -15, "second", 2);

    // First call should return 100
    TEST_ASSERT_EQ(100, ret1, "First AddOp4 should return initial address 100");
    TEST_ASSERT_EQ(100, g_lastAddrFromChangeP4, "First ChangeP4 should be called with address 100");
    TEST_ASSERT_STR_EQ("first", g_lastP4, "First ChangeP4 should receive correct zP4");
    TEST_ASSERT_EQ(1, g_lastP4type, "First ChangeP4 should receive correct p4type");

    // Second call should return 101 (incremented)
    TEST_ASSERT_EQ(101, ret2, "Second AddOp4 should return next sequential address 101");
    TEST_ASSERT_EQ(101, g_lastAddrFromChangeP4, "Second ChangeP4 should be called with address 101");
    TEST_ASSERT_STR_EQ("second", g_lastP4, "Second ChangeP4 should receive correct zP4");
    TEST_ASSERT_EQ(2, g_lastP4type, "Second ChangeP4 should receive correct p4type");
}

static void run_all_tests() {
    test_basic_addop4_normal_case();
    test_basic_addop4_null_p4();
    test_basic_addop4_sequential_calls();
}

int main() {
    run_all_tests();

    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << std::endl;
    if (g_tests_failed == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cout << "Some tests FAILED." << std::endl;
        return 1;
    }
}