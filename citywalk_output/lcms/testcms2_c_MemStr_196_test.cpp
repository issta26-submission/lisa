/*
Candidate Keywords (Step 1):
- MemStr function behavior and thresholds: Mb when size > 1024*1024, Kb when size > 1024, otherwise bytes
- Units emitted: "Mb", "Kb", "bytes"
- Static internal buffer: single static Buffer[1024] used across calls
- Input type cmsUInt32Number and internal formatting using %g
- Return type: const char* to a static buffer
- Access via extern "C" signature in test code
- Basic domain types: cmsUInt32Number, cmsFloat64Number (from testcms2.h)
This test suite targets these core aspects to maximize coverage without GTest.
*/

#include <cstring>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <cstdlib>
#include <cctype>
#include <cmath>


// Bring in the project-specific C declarations and types.
// MemStr is defined in testcms2.c (enclosed area). We declare it with C linkage.
extern "C" {
}

// Prototype for the focal function under test (C linkage)
extern "C" const char* MemStr(cmsUInt32Number size);

// Simple non-terminating test harness (no GTest, no exits on failure)
static int g_totalTests = 0;
static int g_failedTests = 0;

static void ExpectTrue(bool condition, const char* description) {
    ++g_totalTests;
    if (condition) {
        std::cout << "[PASS] " << description << std::endl;
    } else {
        ++g_failedTests;
        std::cout << "[FAIL] " << description << std::endl;
    }
}

// Helper to parse MemStr output into numeric value and unit
static bool ParseMemString(const char* s, double& value, std::string& unit) {
    if (!s) return false;
    char* end = nullptr;
    value = std::strtod(s, &end);
    if (end == s) return false; // no numeric value parsed
    // Move past any whitespace to read the unit
    while (end && *end && std::isspace(static_cast<unsigned char>(*end))) ++end;
    if (end) {
        unit.assign(end);
        // trim trailing spaces in unit
        while (!unit.empty() && std::isspace(static_cast<unsigned char>(unit.back()))) unit.pop_back();
    } else {
        unit.clear();
    }
    return true;
}

// Test 1: Basic correctness across size thresholds (Mb, Kb, bytes)
static void Test_MemStr_Basic_Behaviors() {
    // 0 bytes should be reported as "0 bytes"
    const char* s0 = MemStr(0);
    double val = 0.0;
    std::string unit;
    bool ok = ParseMemString(s0, val, unit);
    ExpectTrue(ok, "MemStr(0) returns parsable string");
    if (ok) {
        ExpectTrue(std::abs(val - 0.0) < 1e-9, "MemStr(0) numeric value is 0");
        ExpectTrue(unit == "bytes", "MemStr(0) unit is 'bytes'");
    }

    // 999 bytes -> "999 bytes"
    const char* s1 = MemStr(999);
    ok = ParseMemString(s1, val, unit);
    ExpectTrue(ok, "MemStr(999) returns parsable string");
    if (ok) {
        ExpectTrue(std::abs(val - 999.0) < 1e-9, "MemStr(999) numeric value 999");
        ExpectTrue(unit == "bytes", "MemStr(999) unit 'bytes'");
    }

    // 1024 bytes should be reported as Kb (not Mb)
    const cmsUInt32Number oneKB = 1024;
    const char* s2 = MemStr(oneKB);
    ok = ParseMemString(s2, val, unit);
    ExpectTrue(ok, "MemStr(1024) returns parsable string");
    if (ok) {
        ExpectTrue(std::abs(val - 1024.0) < 1e-9, "MemStr(1024) numeric value 1024");
        ExpectTrue(unit == "Kb", "MemStr(1024) unit 'Kb'");
    }

    // 5 MB should be reported as Mb
    const cmsUInt32Number fiveMB = static_cast<cmsUInt32Number>(5u * 1024u * 1024u);
    const char* s3 = MemStr(fiveMB);
    ok = ParseMemString(s3, val, unit);
    ExpectTrue(ok, "MemStr(5MB) returns parsable string");
    if (ok) {
        ExpectTrue(std::abs(val - 5.0) < 1e-9, "MemStr(5MB) numeric value 5");
        ExpectTrue(unit == "Mb", "MemStr(5MB) unit 'Mb'");
    }
}

// Test 2: Static buffer reuse semantics (pointer identity across calls)
static void Test_MemStr_PointerReuse() {
    const char* a = MemStr(100);
    const char* b = MemStr(200);
    // MemStr uses a single static buffer; pointer should be identical across calls
    ExpectTrue(a == b, "MemStr uses a single static buffer (pointer stable across calls)");
}

// Test 3: Additional boundary verification (1MB edge case to ensure correct Kb vs Mb decision)
static void Test_MemStr_Boundary_1MB_As_Kb() {
    // 1 MB (1024*1024 bytes) should be reported as 1024 Kb
    const cmsUInt32Number oneMB = static_cast<cmsUInt32Number>(1024u * 1024u);
    const char* s = MemStr(oneMB);
    double val = 0.0;
    std::string unit;
    bool ok = ParseMemString(s, val, unit);
    ExpectTrue(ok, "MemStr(1MB) returns parsable string");
    if (ok) {
        ExpectTrue(std::abs(val - 1024.0) < 1e-9, "MemStr(1MB) numeric value 1024");
        ExpectTrue(unit == "Kb", "MemStr(1MB) unit 'Kb'");
    }
}

// Runner for all tests
static void RunAllTests() {
    Test_MemStr_Basic_Behaviors();
    Test_MemStr_PointerReuse();
    Test_MemStr_Boundary_1MB_As_Kb();
}

// Entry point
int main() {
    RunAllTests();
    std::cout << "Total tests run: " << g_totalTests << std::endl;
    std::cout << "Total failures: " << g_failedTests << std::endl;
    return g_failedTests; // non-zero if any test failed
}