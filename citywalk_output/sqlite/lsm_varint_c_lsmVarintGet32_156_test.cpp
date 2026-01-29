// High-quality unit tests for lsmVarintGet32 (C function) using a small DIY test harness (no GTest).
// This test suite focuses on exercising the branches inside lsmVarintGet32 as per the provided focal method.
// The tests are written for C++11 and link against the C implementation (lsm_varint.c).
// Each test includes explanatory comments.

/*
Notes:
- We target the following branches inside lsmVarintGet32:
  1) z[0] <= 240 -> ret = 1, piVal = z[0]
  2) z[0] <= 248 (i.e., 241..248) -> ret = 2, piVal = (z0-241)*256 + z1 + 240
  3) z[0] == 249 -> ret = 3, piVal = 2288 + 256*z1 + z2
  4) z[0] == 250 -> ret = 4, piVal = (z1<<16) + (z2<<8) + z3
  5) else path -> ret returned by lsmSqlite4GetVarint64(z, &i); piVal = (int)i
- We avoid private/static access and rely on the public C API exposed by lsmVarintGet32.
- For the else-path (5th branch), the exact result depends on lsmSqlite4GetVarint64's behavior in this TU; to maximize determinism, tests cover only the first four branches explicitly and a basic check for the else-path (positive return) without asserting a specific piVal.
*/

#include <lsmInt.h>
#include <iostream>
#include <cstring>
#include <cstdint>


// Declarations for C functions are provided with C linkage to ensure proper linkage when compiling with C++.
extern "C" {
    // Function under test: int lsmVarintGet32(u8 *z, int *piVal);
    // We use unsigned char as a stand-in for u8 (consistent with typical definitions: typedef unsigned char u8).
    int lsmVarintGet32(unsigned char *z, int *piVal);
}

// Simple test harness (non-GTest, non-GMock)
static int g_total = 0;
static int g_passed = 0;

static void logFail(const std::string &msg) {
    std::cerr << "TEST_FAIL: " << msg << std::endl;
}

static void logPass(const std::string &msg) {
    std::cout << "TEST_PASS: " << msg << std::endl;
}

// Test 1: z[0] <= 240 branch
static void test_lsVarintGet32_branch_le240() {
    g_total++;
    unsigned char z[4] = {0x00, 0, 0, 0}; // z[0] = 0 -> ret = 1, piVal = 0
    int piVal = -1;
    int ret = lsmVarintGet32(z, &piVal);
    bool ok = (ret == 1) && (piVal == 0);

    if (ok) {
        g_passed++;
        logPass("lsmVarintGet32(branch <=240): ret==1 and piVal==0 for z[0]=0");
    } else {
        logFail("lsmVarintGet32(branch <=240): unexpected result for z[0]=0 (ret=" +
                std::to_string(ret) + ", piVal=" + std::to_string(piVal) + ")");
    }
}

// Test 2: z[0] in [241,248] branch
static void test_lsVarintGet32_branch_241_248() {
    g_total++;
    // Use z0 = 241, z1 = 5 -> piVal = (241-241)*256 + 5 + 240 = 245
    unsigned char z[2] = {241, 5};
    int piVal = -1;
    int ret = lsmVarintGet32(z, &piVal);
    bool ok = (ret == 2) && (piVal == 245);

    if (ok) {
        g_passed++;
        logPass("lsmVarintGet32(branch 241..248): ret==2 and piVal==245 for z={241,5}");
    } else {
        logFail("lsmVarintGet32(branch 241..248): unexpected result for z={241,5} (ret=" +
                std::to_string(ret) + ", piVal=" + std::to_string(piVal) + ")");
    }
}

// Test 3: z[0] == 249 branch
static void test_lsVarintGet32_branch_249() {
    g_total++;
    // z0=249, z1=12, z2=34 -> piVal = 2288 + 256*12 + 34 = 2288 + 3072 + 34 = 5394
    unsigned char z[3] = {249, 12, 34};
    int piVal = -1;
    int ret = lsmVarintGet32(z, &piVal);
    bool ok = (ret == 3) && (piVal == 5394);

    if (ok) {
        g_passed++;
        logPass("lsmVarintGet32(branch 249): ret==3 and piVal==5394 for z={249,12,34}");
    } else {
        logFail("lsmVarintGet32(branch 249): unexpected result for z={249,12,34} (ret=" +
                std::to_string(ret) + ", piVal=" + std::to_string(piVal) + ")");
    }
}

// Test 4: z[0] == 250 branch
static void test_lsVarintGet32_branch_250() {
    g_total++;
    // z0=250, z1=0, z2=1, z3=2 -> piVal = (0<<16) + (1<<8) + 2 = 258
    unsigned char z[4] = {250, 0, 1, 2};
    int piVal = -1;
    int ret = lsmVarintGet32(z, &piVal);
    bool ok = (ret == 4) && (piVal == 258);

    if (ok) {
        g_passed++;
        logPass("lsmVarintGet32(branch 250): ret==4 and piVal==258 for z={250,0,1,2}");
    } else {
        logFail("lsmVarintGet32(branch 250): unexpected result for z={250,0,1,2} (ret=" +
                std::to_string(ret) + ", piVal=" + std::to_string(piVal) + ")");
    }
}

// Test 5: else-path (z[0] not in any special range)
static void test_lsVarintGet32_elsePath() {
    g_total++;
    // z0 = 251 should trigger the else path. We cannot rely on exact piVal/ret due to the external function.
    unsigned char z[1] = {251};
    int piVal = -1;
    int ret = lsmVarintGet32(z, &piVal);

    // We at least expect a successful return (positive integer), and the function should set piVal.
    bool ok = (ret > 0);

    if (ok) {
        g_passed++;
        logPass("lsmVarintGet32(else path): ret > 0 for z={251} (piVal may vary due to external varint64 getter)");
    } else {
        logFail("lsmVarintGet32(else path): unexpected failure for z={251} (ret=" + std::to_string(ret) +
                ", piVal=" + std::to_string(piVal) + ")");
    }
}

int main() {
    // Run all tests
    test_lsVarintGet32_branch_le240();
    test_lsVarintGet32_branch_241_248();
    test_lsVarintGet32_branch_249();
    test_lsVarintGet32_branch_250();
    test_lsVarintGet32_elsePath();

    // Summary
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Total tests executed: " << g_total << std::endl;
    std::cout << "Tests passed: " << g_passed << std::endl;
    std::cout << "Tests failed: " << (g_total - g_passed) << std::endl;
    return (g_total == g_passed) ? 0 : 1;
}