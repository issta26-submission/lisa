// Lightweight unit test suite for cmsRegisterMultiProcessElementPlugin
// This test does not rely on GoogleTest. It uses a small in-file test harness
// that records failures and continues execution to maximize coverage.
// The tests rely on a local mock of RegisterTypesPlugin to observe how the
// wrapper forwards its parameters (id, Data, pos) and what it returns.

#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <cstdint>
#include <sstream>


// Forward declarations of production types (from cms headers)

// Declaration of the focal function (wrapper under test) with C linkage
extern "C" cmsBool _cmsRegisterMultiProcessElementPlugin(cmsContext id, cmsPluginBase* Data);

// Provide a mock of RegisterTypesPlugin to observe wrapper behavior.
// We declare it here with C linkage. In a real build, this would need to override
// the actual implementation in cmstypes.c. Depending on linker behavior, you may
// need to adapt the build to allow this override (e.g., via linker wrap options).
extern "C" cmsBool RegisterTypesPlugin(cmsContext id, cmsPluginBase* Data, _cmsMemoryClient pos);

// Global mock state (accessible to test checks)
static cmsBool g_mock_ret = true;
static cmsContext g_mock_last_id = 0;
static cmsPluginBase* g_mock_last_Data = nullptr;
static _cmsMemoryClient g_mock_last_pos = 0;

// Mock implementation of RegisterTypesPlugin to intercept calls from the wrapper.
// It records the parameters and returns a value controlled by g_mock_ret.
extern "C" cmsBool RegisterTypesPlugin(cmsContext id, cmsPluginBase* Data, _cmsMemoryClient pos)
{
    g_mock_last_id = id;
    g_mock_last_Data = Data;
    g_mock_last_pos = pos;
    return g_mock_ret;
}

// Simple test harness (non-terminating): records failures but continues.
// Provides a minimal assertion facility suitable for environments without GTest.
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void log_failure(const std::string& message, const char* file, int line)
{
    ++g_failed_tests;
    std::cerr << "TEST FAIL: " << file << ":" << line << " - " << message << std::endl;
}

template <typename T>
static std::string to_string(const T& val)
{
    std::ostringstream os;
    os << val;
    return os.str();
}

// Macros mimicking EXPECT style assertions (non-terminating)
#define EXPECT_TRUE(cond) do { \
    ++g_total_tests; \
    if(!(cond)) log_failure("EXPECT_TRUE failed: " #cond, __FILE__, __LINE__); \
} while(0)

#define EXPECT_EQ(a, b) do { \
    ++g_total_tests; \
    if(!((a) == (b))) log_failure(std::string("EXPECT_EQ failed: ") + to_string(a) + " != " + to_string(b), __FILE__, __LINE__); \
} while(0)

// Test 1: Ensure wrapper forwards id, Data, and MPEPlugin as pos and returns underlying result (true case)
static void test_wrapper_forwards_parameters_true()
{
    // Arrange
    g_mock_ret = true;
    g_mock_last_id = 0;
    g_mock_last_Data = nullptr;
    g_mock_last_pos = 0;

    cmsContext test_id = 0x1234ABCD;                 // arbitrary test id
    cmsPluginBase* test_Data = reinterpret_cast<cmsPluginBase*>(0xA5A5A5A5); // arbitrary pointer

    // Act
    cmsBool res = _cmsRegisterMultiProcessElementPlugin(test_id, test_Data);

    // Assert
    EXPECT_TRUE(res == true);
    EXPECT_EQ(g_mock_last_id, test_id);
    EXPECT_EQ(g_mock_last_Data, test_Data);
    EXPECT_EQ(g_mock_last_pos, MPEPlugin);
}

// Test 2: Ensure wrapper forwards parameters and returns false when underlying function returns false
static void test_wrapper_forwards_parameters_false()
{
    // Arrange
    g_mock_ret = false;
    g_mock_last_id = 0;
    g_mock_last_Data = nullptr;
    g_mock_last_pos = 0;

    cmsContext test_id = 0xDEADBEEF;
    cmsPluginBase* test_Data = reinterpret_cast<cmsPluginBase*>(0xCAFEBABE);

    // Act
    cmsBool res = _cmsRegisterMultiProcessElementPlugin(test_id, test_Data);

    // Assert
    EXPECT_TRUE(res == false);
    EXPECT_EQ(g_mock_last_id, test_id);
    EXPECT_EQ(g_mock_last_Data, test_Data);
    EXPECT_EQ(g_mock_last_pos, MPEPlugin);
}

// Test 3: Ensure wrapper handles null Data pointer gracefully and forwards pos correctly
static void test_wrapper_null_Data_pointer()
{
    // Arrange
    g_mock_ret = true;
    g_mock_last_id = 0;
    g_mock_last_Data = reinterpret_cast<cmsPluginBase*>(0x0);
    g_mock_last_pos = 0;

    cmsContext test_id = 0x0;
    cmsPluginBase* test_Data = nullptr;

    // Act
    cmsBool res = _cmsRegisterMultiProcessElementPlugin(test_id, test_Data);

    // Assert
    EXPECT_TRUE(res == true);
    EXPECT_EQ(g_mock_last_id, test_id);
    EXPECT_EQ(g_mock_last_Data, test_Data);
    EXPECT_EQ(g_mock_last_pos, MPEPlugin);
}

// Test 4: Repeated calls verify wrapper consistently forwards parameters and uses the same pos
static void test_wrapper_repeated_calls_consistency()
{
    // Arrange
    g_mock_ret = true;

    // First call
    cmsContext id1 = 0x11111111;
    cmsPluginBase* data1 = reinterpret_cast<cmsPluginBase*>(0x11111111);
    _cmsRegisterMultiProcessElementPlugin(id1, data1);
    EXPECT_EQ(g_mock_last_id, id1);
    EXPECT_EQ(g_mock_last_Data, data1);
    EXPECT_EQ(g_mock_last_pos, MPEPlugin);

    // Second call with different values
    cmsContext id2 = 0x22222222;
    cmsPluginBase* data2 = reinterpret_cast<cmsPluginBase*>(0x22222222);
    _cmsRegisterMultiProcessElementPlugin(id2, data2);
    EXPECT_EQ(g_mock_last_id, id2);
    EXPECT_EQ(g_mock_last_Data, data2);
    EXPECT_EQ(g_mock_last_pos, MPEPlugin);
}

// Entry point: run all tests
int main()
{
    // Run tests
    test_wrapper_forwards_parameters_true();
    test_wrapper_forwards_parameters_false();
    test_wrapper_null_Data_pointer();
    test_wrapper_repeated_calls_consistency();

    // Summary
    std::cout << "Total tests run: " << g_total_tests << std::endl;
    std::cout << "Total failures: " << g_failed_tests << std::endl;

    if(g_failed_tests != 0)
    {
        std::cerr << "Some tests FAILED." << std::endl;
        return 1;
    }

    std::cout << "All tests PASSED." << std::endl;
    return 0;
}