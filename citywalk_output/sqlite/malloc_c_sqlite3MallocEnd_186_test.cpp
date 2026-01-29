// Unit test suite for sqlite3MallocEnd using C++11 (no Google Test)
 // This test focuses on the behavior of sqlite3MallocEnd as defined in malloc.c:
 // - If sqlite3GlobalConfig.m.xShutdown is non-NULL, it must be invoked with sqlite3GlobalConfig.m.pAppData.
 // - mem0 is cleared via memset (not directly verifiable here due to its static scope in malloc.c).

#include <iostream>
#include <sqliteInt.h>
#include <cstdint>
#include <stdarg.h>

extern "C" {
}

// Simple non-terminating test harness
static int gTestTotal = 0;
static int gTestFailures = 0;

// Convenience macros for non-terminating checks
#define TEST_EXPECT_TRUE(cond, desc) do { \
    ++gTestTotal; \
    if(!(cond)) { \
        ++gTestFailures; \
        std::cerr << "Test failed: " << (desc) << std::endl; \
    } \
} while(0)

#define TEST_EXPECT_PTR_EQ(a, b, desc) do { \
    ++gTestTotal; \
    if((void*)(a) != (void*)(b)) { \
        ++gTestFailures; \
        std::cerr << "Test failed: " << (desc) \
                  << " (expected " << (void*)(b) \
                  << ", got " << (void*)(a) << ")" << std::endl; \
    } \
} while(0)

// Globals used by the mock shutdown function to observe behavior
static bool gShutdownCalled = false;
static void* gShutdownArg = nullptr;

// Mock shutdown function to capture callback invocation and argument
static void mockShutdown(void* arg) {
    gShutdownCalled = true;
    gShutdownArg = arg;
}

// Test 1: When xShutdown is non-NULL, sqlite3MallocEnd should call it with pAppData
void test_sqlite3MallocEnd_callsShutdown_whenNonNull() {
    // Arrange
    gShutdownCalled = false;
    gShutdownArg = nullptr;

    // Configure the global config to point to our mock and a known app data pointer
    sqlite3GlobalConfig.m.xShutdown = mockShutdown;
    void* const expectedAppData = reinterpret_cast<void*>(0x12345678);
    sqlite3GlobalConfig.m.pAppData = expectedAppData;

    // Act
    sqlite3MallocEnd();

    // Assert
    TEST_EXPECT_TRUE(gShutdownCalled, "sqlite3MallocEnd should call xShutdown when non-NULL");
    TEST_EXPECT_PTR_EQ(gShutdownArg, expectedAppData, "Shutdown callback should be invoked with pAppData");
}

// Test 2: When xShutdown is NULL, sqlite3MallocEnd should not call it
void test_sqlite3MallocEnd_doesNotCallShutdown_whenNull() {
    // Arrange
    sqlite3GlobalConfig.m.xShutdown = NULL;
    gShutdownCalled = false;
    gShutdownArg = nullptr;

    // Act
    sqlite3MallocEnd();

    // Assert
    TEST_EXPECT_TRUE(!gShutdownCalled, "sqlite3MallocEnd should not call xShutdown when it is NULL");
}

// Main: Run tests and report summary
int main() {
    // Run tests
    test_sqlite3MallocEnd_callsShutdown_whenNonNull();
    test_sqlite3MallocEnd_doesNotCallShutdown_whenNull();

    // Summary
    std::cout << "Test Summary: " << gTestTotal << " tests run, "
              << gTestFailures << " failures." << std::endl;

    // Return non-zero if any test failed, to integrate with CI tooling if desired
    return (gTestFailures != 0) ? 1 : 0;
}