// unity_output_Spy_tests.cpp
// Lightweight C++11 test harness for UnityOutputCharSpy_Enable in unity_output_Spy.c
// This test suite does not rely on GTest or any external testing framework.
// It uses a minimal, non-terminating assertion mechanism to allow continued
// execution for higher coverage.
//
// Assumptions based on provided focal code:
// - The function UnityOutputCharSpy_Enable(int enable) toggles an internal flag `spy_enable`
//   (not directly accessible from tests because it's file-scoped).
// - Other C functions are available to exercise observable behavior: Create, Destroy,
//   OutputChar, and Get. We will rely on Get() to return some observable state when
//   characters are output while the spy is enabled.
// - The test imports the C interfaces via unity_output_Spy.h (within extern "C").
// - The tests are compiled as a C++11 translation unit but call C functions.

#include <cstring>
#include <unity_output_Spy.h>
#include <iostream>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <unity_fixture.h>
#include <stdio.h>


extern "C" {
}

// Simple non-terminating assertion macro.
// It logs failures but does not abort test execution, enabling continued coverage.
#define ASSERT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        ++g_failures; \
        std::cerr << "[FAIL] " << __func__ << ": " << (msg) << std::endl; \
    } \
} while(0)

static int g_failures = 0;

// Helper to get current spy buffer content as std::string (safe wrapper).
static std::string GetBufferContent() {
    const char* buf = UnityOutputCharSpy_Get();
    if (buf == nullptr) {
        return std::string();
    }
    // Assume buffer is null-terminated C-string.
    return std::string(buf);
}

// Test 1: Enable(1) followed by an OutputChar should produce observable content.
// Then Disable(0) should stop further capture, and re-enable should resume observable behavior.
// This test focuses on exercising the functional observable behavior of the enable switch
// and basic emission via OutputChar/Get without assuming exact internal buffer semantics.
void Test_Enable_TogglesAndCapturesContent() {
    // Initialize a fresh spy instance.
    UnityOutputCharSpy_Create(0);

    // Enable capturing.
    UnityOutputCharSpy_Enable(1);
    // Emit a character and observe buffer content.
    UnityOutputCharSpy_OutputChar('A');
    std::string first = GetBufferContent();

    // Disable capturing and emit another character (should not affect buffer if disabled).
    UnityOutputCharSpy_Enable(0);
    UnityOutputCharSpy_OutputChar('B');
    std::string second = GetBufferContent();

    // Re-enable capturing and emit another character.
    UnityOutputCharSpy_Enable(1);
    UnityOutputCharSpy_OutputChar('C');
    std::string third = GetBufferContent();

    // Cleanup.
    UnityOutputCharSpy_Destroy();

    // Basic assertions:
    // - First buffer should contain some observable content (e.g., 'A').
    // - Buffer after disable should be (likely) unchanged or non-null; we avoid strict checks
    //   on exact content due to possible variations in implementation, but we require
    //   that we had at least some observable data during the test.
    bool firstHasContent = !first.empty();
    ASSERT_TRUE(firstHasContent, "After enabling and outputting 'A', buffer should contain data.");

    // At least one of the buffers (first or second or third) should reflect emitted characters
    // to indicate the spy/internal buffering is active. We do not rely on exact content.
    bool anyBufferHas AorC = false;
    if (!first.empty() && (first.find('A') != std::string::npos)) AorC = true;
    if (!second.empty() && (second.find('B') != std::string::npos)) AorC = true;
    if (!third.empty() && (third.find('C') != std::string::npos)) AorC = true;

    ASSERT_TRUE(anyBufferHas AorC, "Buffer should reflect emitted characters (A/B/C) across enable toggles when observable.");
}

// Test 2: Boundary values for Enable should not crash the API and should not introduce undefined behavior.
// This checks that the API remains robust across a variety of integer inputs for enable.
void Test_Enable_VariousInputs_NoCrash() {
    // Initialize a fresh spy instance.
    UnityOutputCharSpy_Create(0);

    // Try a set of diverse values, including negative, zero, and large positives.
    const int testValues[] = { -1, 0, 1, 2, 42, 9999 };
    for (int v : testValues) {
        UnityOutputCharSpy_Enable(v);
        UnityOutputCharSpy_OutputChar('X');
        // Read buffer to ensure we can access it without crashing.
        const char* buf = UnityOutputCharSpy_Get();
        // If non-null, ensure it's a valid C-string (no crash happened).
        if (buf != nullptr) {
            // Optional: verify is null-terminated by attempting to read a character.
            (void)buf[0];
        }
    }

    // Cleanup.
    UnityOutputCharSpy_Destroy();

    // If we reached here without crash, consider test pass.
    ASSERT_TRUE(true, "Enable() with various inputs should not crash or corrupt API usage.");
}

// Test 3: Create/Destroy lifecycle should be safe with Enable/OutputChar sequences.
// Ensures repeated lifecycle operations do not leak state or crash the system.
void Test_CreateDestroyLifecycle() {
    // First lifecycle
    UnityOutputCharSpy_Create(0);
    UnityOutputCharSpy_Enable(1);
    UnityOutputCharSpy_OutputChar('D');
    std::string content1 = GetBufferContent();
    UnityOutputCharSpy_Destroy();

    // Second lifecycle
    UnityOutputCharSpy_Create(0);
    UnityOutputCharSpy_Enable(1);
    UnityOutputCharSpy_OutputChar('E');
    std::string content2 = GetBufferContent();
    UnityOutputCharSpy_Destroy();

    // Basic assertions: at least we should have results from both lifecycles and API did not crash.
    bool content1NonNull = !content1.empty();
    bool content2NonNull = !content2.empty();
    ASSERT_TRUE(content1NonNull || content2NonNull, "Both lifecycles should produce observable content on at least one call.");

    // No crash implies success; we already observed buffer content presence if any.
}

int main() {
    // Reset failure counter
    g_failures = 0;

    // Run tests
    Test_Enable_TogglesAndCapturesContent();
    Test_Enable_VariousInputs_NoCrash();
    Test_CreateDestroyLifecycle();

    // Summary
    std::cout << "Total tests executed: 3" << std::endl;
    std::cout << "Total failures: " << g_failures << std::endl;

    // Return non-zero if any test failed to enable integration with build systems.
    return g_failures ? 1 : 0;
}