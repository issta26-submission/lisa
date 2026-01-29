// Unit test suite for sqlite3Fts5UnicodeCatParse
// Target: compile with the focal C function implemented in fts5_unicode2.c
// Approach: plain C++11 test harness (no Google Test). Tests are non-terminating on failure
// and report a summary at the end.
// Build note: Ensure fts5_unicode2.c is part of the compilation/linking unit.
// The function uses a 32-element output array (indices 0..31). We initialize with zeros before each test.

#include <vector>
#include <assert.h>
#include <cstring>
#include <iostream>
#include <algorithm>


// Declaration of the focal function (C linkage)
extern "C" int sqlite3Fts5UnicodeCatParse(const char *zCat, unsigned char *aArray);

// Global failure counter
static int g_failures = 0;

// Simple non-terminating check reporter
#define REPORT(cond, desc) do { if(!(cond)) { ++g_failures; std::cerr << "[FAIL] " << desc << " at " << __FILE__ << ":" << __LINE__ << "\n"; } } while(0)

// Helper: reset the 32-byte array to zeros
static void resetArray(unsigned char a[32]) {
    std::memset(a, 0, 32);
}

// Test 1: Cc -> zCat = "Cc": expect aArray[0] = 1, aArray[1] = 1, all others 0, return 0
static void test_case_Cc_sets_bit1() {
    unsigned char a[32];
    resetArray(a);
    const char zCat[] = "Cc"; // 'C' then 'c'
    int ret = sqlite3Fts5UnicodeCatParse(zCat, a);
    bool ok = true;
    if(ret != 0) ok = false;
    if(a[0] != 1) ok = false;
    if(a[1] != 1) ok = false;
    for(int i = 2; i < 32; ++i){
        if(a[i] != 0){ ok = false; break; }
    }
    REPORT(ok, "test_case_Cc_sets_bit1: Cc should set indices [0,1] and return 0");
}

// Test 2: C* -> zCat = "C*": expect bits 0,1,2,3,4,31 set; return 0
static void test_case_CStar_sets_multiple() {
    unsigned char a[32];
    resetArray(a);
    const char zCat[] = "C*";
    int ret = sqlite3Fts5UnicodeCatParse(zCat, a);
    bool ok = true;
    if(ret != 0) ok = false;
    if(a[0] != 1) ok = false;
    // indices 1,2,3,4,31 should be 1
    for(int idx : {1,2,3,4,31}) {
        if(a[idx] != 1) { ok = false; break; }
    }
    // Others should be 0
    for(int i = 0; i < 32 && ok; ++i){
        if(std::find((std::vector<int>){1,2,3,4,31}.begin(), (std::vector<int>::iterator)((std::vector<int>){1,2,3,4,31}).end(), i) == (std::vector<int>::iterator)((std::vector<int>){1,2,3,4,31}).end()) {
            // this approach is flaky for inline vector usage; instead simply check non-specified indices
        }
    }
    // Simpler explicit non-set indices check
    for(int i = 0; i < 32; ++i){
        bool isOneOfExpected = (i==0) || (i==1) || (i==2) || (i==3) || (i==4) || (i==31);
        if(!isOneOfExpected && a[i] != 0) { ok = false; break; }
    }
    REPORT(ok, "test_case_CStar_sets_multiple: C* should set [0,1,2,3,4,31] and return 0");
}

// Test 3: Ll -> zCat = "Ll": expect aArray[5] set; return 0
static void test_case_Ll_sets_bit5() {
    unsigned char a[32];
    resetArray(a);
    const char zCat[] = "Ll";
    int ret = sqlite3Fts5UnicodeCatParse(zCat, a);
    bool ok = true;
    if(ret != 0) ok = false;
    if(a[0] != 1) ok = false;
    if(a[5] != 1) ok = false;
    for(int i=1; i<32; ++i){
        if(i==5 || i==0) continue;
        if(a[i] != 0){ ok = false; break; }
    }
    REPORT(ok, "test_case_Ll_sets_bit5: Ll should set index 5 and return 0");
}

// Test 4: M* -> zCat = "M*": expect indices 10,11,12 set; return 0
static void test_case_MStar_sets_10_11_12() {
    unsigned char a[32];
    resetArray(a);
    const char zCat[] = "M*";
    int ret = sqlite3Fts5UnicodeCatParse(zCat, a);
    bool ok = true;
    if(ret != 0) ok = false;
    if(a[0] != 1) ok = false;
    if(a[10] != 1 || a[11] != 1 || a[12] != 1) ok = false;
    for(int i = 1; i < 32; ++i){
        if(i==10 || i==11 || i==12) continue;
        if(a[i] != 0){ ok = false; break; }
    }
    REPORT(ok, "test_case_MStar_sets_10_11_12: M* should set [10,11,12] and return 0");
}

// Test 5: Nd -> zCat = "Nd": expect aArray[13] set; return 0
static void test_case_Nd_sets_13() {
    unsigned char a[32];
    resetArray(a);
    const char zCat[] = "Nd";
    int ret = sqlite3Fts5UnicodeCatParse(zCat, a);
    bool ok = true;
    if(ret != 0) ok = false;
    if(a[0] != 1) ok = false;
    if(a[13] != 1) ok = false;
    for(int i = 1; i < 32; ++i){
        if(i==13) continue;
        if(a[i] != 0){ ok = false; break; }
    }
    REPORT(ok, "test_case_Nd_sets_13: Nd should set [13] and return 0");
}

// Test 6: P* -> zCat = "P*": indices 16..22 set; return 0
static void test_case_PStar_sets_16_22() {
    unsigned char a[32];
    resetArray(a);
    const char zCat[] = "P*";
    int ret = sqlite3Fts5UnicodeCatParse(zCat, a);
    bool ok = true;
    if(ret != 0) ok = false;
    if(a[0] != 1) ok = false;
    for(int idx = 16; idx <= 22; ++idx){
        if(a[idx] != 1) { ok = false; break; }
    }
    for(int i = 1; i < 32; ++i){
        if(i>=16 && i<=22) continue;
        if(a[i] != 0){ ok = false; break; }
    }
    REPORT(ok, "test_case_PStar_sets_16_22: P* should set [16..22] and return 0");
}

// Test 7: S* -> zCat = "S*": indices 23..26 set; return 0
static void test_case_SStar_sets_23_26() {
    unsigned char a[32];
    resetArray(a);
    const char zCat[] = "S*";
    int ret = sqlite3Fts5UnicodeCatParse(zCat, a);
    bool ok = true;
    if(ret != 0) ok = false;
    if(a[0] != 1) ok = false;
    for(int idx = 23; idx <= 26; ++idx){
        if(a[idx] != 1) { ok = false; break; }
    }
    for(int i = 1; i < 32; ++i){
        if(i>=23 && i<=26) continue;
        if(a[i] != 0){ ok = false; break; }
    }
    REPORT(ok, "test_case_SStar_sets_23_26: S* should set [23..26] and return 0");
}

// Test 8: Z* -> zCat = "Z*": indices 27..29 set; return 0
static void test_case_ZStar_sets_27_29() {
    unsigned char a[32];
    resetArray(a);
    const char zCat[] = "Z*";
    int ret = sqlite3Fts5UnicodeCatParse(zCat, a);
    bool ok = true;
    if(ret != 0) ok = false;
    if(a[0] != 1) ok = false;
    for(int idx = 27; idx <= 29; ++idx){
        if(a[idx] != 1) { ok = false; break; }
    }
    for(int i = 1; i < 32; ++i){
        if(i>=27 && i<=29) continue;
        if(a[i] != 0){ ok = false; break; }
    }
    REPORT(ok, "test_case_ZStar_sets_27_29: Z* should set [27..29] and return 0");
}

// Test 9: Unknown top-level character (e.g., 'A*'): should return 0 and only aArray[0] = 1
static void test_case_UnknownTopLevel() {
    unsigned char a[32];
    resetArray(a);
    const char zCat[] = "A*";
    int ret = sqlite3Fts5UnicodeCatParse(zCat, a);
    bool ok = true;
    if(ret != 0) ok = false;
    if(a[0] != 1) ok = false;
    for(int i = 1; i < 32; ++i){
        if(a[i] != 0){ ok = false; break; }
    }
    REPORT(ok, "test_case_UnknownTopLevel: unknown top-level should return 0 and only aArray[0] = 1");
}

// Test 10: Known top-level with unknown inner (e.g., 'Cx'): should return 1 and only aArray[0] = 1 (no inner match)
static void test_case_KnownTopUnknownInner() {
    unsigned char a[32];
    resetArray(a);
    const char zCat[] = "Cx";
    int ret = sqlite3Fts5UnicodeCatParse(zCat, a);
    bool ok = true;
    if(ret != 1) ok = false;
    if(a[0] != 1) ok = false;
    for(int i = 1; i < 32; ++i){
        if(a[i] != 0){ ok = false; break; }
    }
    REPORT(ok, "test_case_KnownTopUnknownInner: Cx should return 1 and set only aArray[0] = 1");
}

// Run all tests
static void run_all_tests() {
    test_case_Cc_sets_bit1();
    test_case_CStar_sets_multiple();
    test_case_Ll_sets_bit5();
    test_case_MStar_sets_10_11_12();
    test_case_Nd_sets_13();
    test_case_PStar_sets_16_22();
    test_case_SStar_sets_23_26();
    test_case_ZStar_sets_27_29();
    test_case_UnknownTopLevel();
    test_case_KnownTopUnknownInner();
}

// Entry point
int main() {
    run_all_tests();
    if(g_failures == 0) {
        std::cout << "[OK] All tests passed for sqlite3Fts5UnicodeCatParse.\n";
        return 0;
    } else {
        std::cerr << "[SUMMARY] Failures: " << g_failures << "\n";
        return 1;
    }
}