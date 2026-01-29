// Test suite for sqlite3Fts5PutVarint (C function) using C++11, no GTest.
// The tests focus on verifying the two early branches and handling of the
// "larger than 0x3fff" path by ensuring behavior is deterministic for the
// first two branches and that the third branch triggers a longer varint path.
// Note: fts5PutVarint64 is a static helper inside fts5_varint.c, so we cannot
// directly inspect its content. We nevertheless test the observable behavior of
// sqlite3Fts5PutVarint for representative inputs.

#include <iostream>
#include <cstring>
#include <cstdint>
#include <fts5Int.h>


// Domain knowledge and mapping to the focal method:
// - Candidate Keywords: sqlite3Fts5PutVarint, fts5PutVarint64 (static in file),
//   v <= 0x7f, v <= 0x3fff, 0x7f, 0x3fff, 0x4000 boundary, u64 type.
// - We test true/false branches of the two conditions.
// - We ensure we do not rely on private/static internals beyond the supplied API.
// - We use a small, self-contained test harness (no external testing framework).

typedef unsigned long long u64;

// Declaration must match the C linkage of the focal function.
// The actual implementation is in fts5_varint.c and will be linked in.
extern "C" int sqlite3Fts5PutVarint(unsigned char *p, u64 v);

// Helper: simple assertion that prints an error message but does not terminate tests.
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "Test failed: " << (msg) << " (line " << __LINE__ << ")" << std::endl; \
        return false; \
    } \
} while(0)

// Test 1: v = 0x00 -> first branch (single byte)
static bool test_Varint_Minimum(){
    unsigned char buf[16];
    // Initialize buffer with a recognizable sentinel value
    std::memset(buf, 0xAA, sizeof(buf));

    int ret = sqlite3Fts5PutVarint(buf, 0x0);
    // Expect 1-byte varint
    TEST_ASSERT(ret == 1, "Expected 1-byte varint for v <= 0x7f (0x0)");
    TEST_ASSERT(buf[0] == 0x00, "Expected buffer[0] == 0x00 for v == 0x0");
    // Ensure no overrun beyond produced bytes
    for(size_t i = 1; i < sizeof(buf); ++i){
        TEST_ASSERT(buf[i] == 0xAA, "Buffer should not be modified beyond written bytes (min)");
    }
    return true;
}

// Test 2: v = 0x7f -> first branch (single byte, edge of first branch)
static bool test_Varint_SingleByteEdge(){
    unsigned char buf[16];
    std::memset(buf, 0xBB, sizeof(buf));

    int ret = sqlite3Fts5PutVarint(buf, 0x7f);
    TEST_ASSERT(ret == 1, "Expected 1-byte varint for v == 0x7f");
    TEST_ASSERT(buf[0] == 0x7f, "Expected buffer[0] == 0x7f for v == 0x7f");
    for(size_t i = 1; i < sizeof(buf); ++i){
        TEST_ASSERT(buf[i] == 0xBB, "Buffer should not be modified beyond written bytes (single-byte edge)");
    }
    return true;
}

// Test 3: v = 0x80 -> second branch (two-byte varint)
static bool test_Varint_TwoBytesFirstValue(){
    unsigned char buf[16];
    std::memset(buf, 0xCC, sizeof(buf));

    int ret = sqlite3Fts5PutVarint(buf, 0x80);
    // For v in (0x7f, 0x3fff], the encoding uses two bytes.
    TEST_ASSERT(ret == 2, "Expected 2-byte varint for v <= 0x3fff and > 0x7f (0x80)");
    TEST_ASSERT(buf[0] == 0x81, "Expected buf[0] == 0x81 for v == 0x80");
    TEST_ASSERT(buf[1] == 0x00, "Expected buf[1] == 0x00 for v == 0x80");
    for(size_t i = 2; i < sizeof(buf); ++i){
        TEST_ASSERT(buf[i] == 0xCC, "Buffer should not be modified beyond written bytes (two-byte value)");
    }
    return true;
}

// Test 4: v = 0x3fff -> second branch (two-byte varint, upper bound)
static bool test_Varint_TwoBytesUpperBound(){
    unsigned char buf[16];
    std::memset(buf, 0xDD, sizeof(buf));

    int ret = sqlite3Fts5PutVarint(buf, 0x3fff);
    TEST_ASSERT(ret == 2, "Expected 2-byte varint for v == 0x3fff");
    TEST_ASSERT(buf[0] == 0xff, "Expected buf[0] == 0xff for v == 0x3fff");
    TEST_ASSERT(buf[1] == 0x7f, "Expected buf[1] == 0x7f for v == 0x3fff");
    for(size_t i = 2; i < sizeof(buf); ++i){
        TEST_ASSERT(buf[i] == 0xDD, "Buffer should not be modified beyond written bytes (upper bound)");
    }
    return true;
}

// Test 5: v = 0x4000 -> third path (requires fts5PutVarint64); ensure non-trivial write length
static bool test_Varint_LargeValue(){
    unsigned char buf[16];
    std::memset(buf, 0xEE, sizeof(buf));

    int ret = sqlite3Fts5PutVarint(buf, 0x4000);
    // We cannot know exact length without fts5PutVarint64, but it should be at least 3
    TEST_ASSERT(ret >= 3, "Expected multi-byte varint for v > 0x3fff (0x4000) to be >= 3 bytes");
    // At least the first byte should be modified
    TEST_ASSERT(buf[0] != 0xEE, "First byte should be modified for large varint");
    // We also check that the subsequent bytes exist and not all sentinel
    bool any_more_written = (buf[1] != 0xEE) || (buf[2] != 0xEE);
    TEST_ASSERT(any_more_written, "At least one of the first few bytes should be written beyond sentinel for large varint");
    return true;
}

// Simple test runner
int main() {
    int failures = 0;

    if(!test_Varint_Minimum()) {
        std::cerr << "test_Varint_Minimum failed." << std::endl;
        ++failures;
    } else {
        std::cout << "test_Varint_Minimum passed." << std::endl;
    }

    if(!test_Varint_SingleByteEdge()) {
        std::cerr << "test_Varint_SingleByteEdge failed." << std::endl;
        ++failures;
    } else {
        std::cout << "test_Varint_SingleByteEdge passed." << std::endl;
    }

    if(!test_Varint_TwoBytesFirstValue()) {
        std::cerr << "test_Varint_TwoBytesFirstValue failed." << std::endl;
        ++failures;
    } else {
        std::cout << "test_Varint_TwoBytesFirstValue passed." << std::endl;
    }

    if(!test_Varint_TwoBytesUpperBound()) {
        std::cerr << "test_Varint_TwoBytesUpperBound failed." << std::endl;
        ++failures;
    } else {
        std::cout << "test_Varint_TwoBytesUpperBound passed." << std::endl;
    }

    if(!test_Varint_LargeValue()) {
        std::cerr << "test_Varint_LargeValue failed." << std::endl;
        ++failures;
    } else {
        std::cout << "test_Varint_LargeValue passed." << std::endl;
    }

    if(failures > 0) {
        std::cerr << "Total failures: " << failures << std::endl;
        return 1;
    } else {
        std::cout << "All tests passed." << std::endl;
        return 0;
    }
}