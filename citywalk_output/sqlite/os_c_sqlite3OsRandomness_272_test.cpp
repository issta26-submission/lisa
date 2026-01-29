// Test suite for the focal method: sqlite3OsRandomness
// This test file is designed to be compiled with a C++11 compiler
// and linked against the production sqlite3OsRandomness implementation
// (as provided in os.c). It does not rely on GoogleTest; instead it
// uses a lightweight, self-contained test harness with simple assertions.
// See comments above each test for the intent and expected behavior.
//
// Important notes for integration:
// - The tests assume the function signature:
//     int sqlite3OsRandomness(sqlite3_vfs *pVfs, int nByte, char *zBufOut);
// - The tests provide a minimal sqlite3_vfs type with an xRandomness callback.
// - The tests manipulate the global sqlite3Config.iPrngSeed to exercise both
//   the true (seeded) and false (random) branches.
// - The tests are designed to be robust across endianness by reading the seed
//   bytes directly from memory to construct the expected output.
// - To run tests, compile this file together with the real os.c (or a compatible
//   translation unit that provides sqlite3OsRandomness) and link.
// - The tests use non-terminating assertions (they don't exit on failure) to
//   maximize code coverage.

#include <cstdio>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>


// Forward declaration of the focal function (as it would appear in the codebase).
// The function uses C linkage in the original project.
extern "C" int sqlite3OsRandomness(struct sqlite3_vfs *pVfs, int nByte, char *zBufOut);

// Minimal stand-in for the real sqlite3_vfs structure.
// We only rely on the xRandomness function pointer in tests.
struct sqlite3_vfs {
    int (*xRandomness)(sqlite3_vfs *pVfs, int nByte, char *zBufOut);
};

// Minimal stand-in for the sqlite3Config global used by sqlite3OsRandomness.
// We mirror only the iPrngSeed member needed by the focal function.
struct sqlite3ConfigStruct {
    int iPrngSeed;
} sqlite3Config;

// Simple non-terminating assertion macro (does not abort on failure).
#define EXPECT_TRUE(cond) do { if (!(cond)) { \
    std::cerr << "EXPECT_TRUE failed at " << __FILE__ << ":" << __LINE__ << "\n"; \
} } while (0)

#define EXPECT_FALSE(cond) do { if (cond) { \
    std::cerr << "EXPECT_FALSE failed at " << __FILE__ << ":" << __LINE__ << "\n"; \
} } while (0)

#define EXPECT_EQ(a, b) do { \
    if ((a) != (b)) { \
        std::cerr << "EXPECT_EQ failed at " << __FILE__ << ":" << __LINE__ \
                  << " - " << #a << " != " << #b << " (" << (a) << " != " << (b) << ")\n"; \
    } \
} while (0)

#define EXPECT_BUF_EQ(expected, actual, len) do { \
    if (std::memcmp((expected), (actual), (len)) != 0) { \
        std::cerr << "EXPECT_BUF_EQ failed at " << __FILE__ << ":" << __LINE__ \
                  << " - buffers differ in first " << (len) << " bytes\n"; \
    } \
} while (0)

// Helper: a dummy xRandomness that fills the buffer with 0x5A and returns a specified code.
static int dummyRandomness(sqlite3_vfs *pVfs, int nByte, char *zBufOut)
{
    // Mark that this path was taken (used by tests to verify the else-branch is invoked)
    static bool dummyCalledFlag = false;
    dummyCalledFlag = true;

    // Fill with 0x5A and return a success code (we'll override in tests if needed)
    std::memset(zBufOut, 0x5A, nByte);
    return 0; // SQLITE_OK in real code is typically 0
}

// Test 1: When sqlite3Config.iPrngSeed is non-zero, the function should
// seed the output with the seed value (up to sizeof(unsigned int)) and
// zero the rest (up to nByte). It should not call pVfs->xRandomness.
static void test_seed_branch_writes_seed_and_zeros_for_extra_bytes() {
    // Arrange
    sqlite3Config.iPrngSeed = 0x11223344; // non-zero seed
    sqlite3_vfs vfs;
    vfs.xRandomness = nullptr; // should not be called in this branch

    const int N = 8;
    char zBufOut[N];
    std::memset(zBufOut, 0, sizeof(zBufOut)); // ensure clean slate

    // Act
    int rc = sqlite3OsRandomness(&vfs, 6, zBufOut); // nByte > sizeof(unsigned) to trigger clamp

    // Assert
    EXPECT_EQ(rc, 0); // SQLITE_OK
    // Build expected array: first 4 bytes are seed (endian dependent), rest zeros
    unsigned int seedCopy = static_cast<unsigned int>(sqlite3Config.iPrngSeed);
    unsigned char seedBytes[4];
    std::memcpy(seedBytes, &seedCopy,  sizeof(seedBytes)); // seed's raw bytes

    unsigned char expected[8];
    // first 4 bytes: seed, next 4 bytes: zeros
    std::memcpy(expected, seedBytes, 4);
    std::memset(expected + 4, 0, 4);

    EXPECT_BUF_EQ(reinterpret_cast<char*>(expected), zBufOut, 6);
}

// Test 2: When sqlite3Config.iPrngSeed is zero, the function should call the
// VFS's xRandomness and return its result, writing into zBufOut accordingly.
static void test_randomness_branch_calls_vfs_and_returns_its_value() {
    // Arrange
    sqlite3Config.iPrngSeed = 0; // force else branch
    static int dummyCalled = 0;
    auto customRandom = [](sqlite3_vfs *pVfs, int nByte, char *zBufOut) -> int {
        dummyCalled = 1;
        for (int i = 0; i < nByte; ++i) zBufOut[i] = static_cast<char>(0x5A);
        return 123; // arbitrary non-zero to test propagation
    };

    sqlite3_vfs vfs;
    vfs.xRandomness = customRandom;

    const int N = 8;
    char zBufOut[N];

    // Act
    int rc = sqlite3OsRandomness(&vfs, 8, zBufOut);

    // Assert
    EXPECT_TRUE(dummyCalled != 0);
    EXPECT_EQ(rc, 123);
    for (int i = 0; i < 8; ++i) {
        EXPECT_EQ(zBufOut[i], static_cast<char>(0x5A));
    }
}

// Test 3: True branch handles smaller nByte correctly (copies exactly nByte bytes
// from the seed without extra writes beyond nByte).
static void test_seed_branch_copies_exact_nBytes_when_small() {
    // Arrange
    sqlite3Config.iPrngSeed = 0xA1A2A3A4;
    sqlite3_vfs vfs;
    vfs.xRandomness = nullptr;

    const int N = 6;
    char zBufOut[N];
    std::memset(zBufOut, 0, sizeof(zBufOut));

    // Act
    int rc = sqlite3OsRandomness(&vfs, 3, zBufOut);

    // Assert
    EXPECT_EQ(rc, 0);
    unsigned int seedCopy = static_cast<unsigned int>(sqlite3Config.iPrngSeed);
    unsigned char seedBytes[4];
    std::memcpy(seedBytes, &seedCopy, sizeof(seedBytes));

    unsigned char expected[6];
    std::memcpy(expected, seedBytes, 3); // only first 3 bytes should be copied
    EXPECT_BUF_EQ(reinterpret_cast<char*>(expected), zBufOut, 3);
}

// Test 4: When iPrngSeed is non-zero and nByte exceeds the size of unsigned,
// the function clamps to sizeof(unsigned) for the memcpy, and zeros the rest.
static void test_seed_branch_clamps_to_unsigned_size_and_zeros_rest() {
    // Arrange
    sqlite3Config.iPrngSeed = 0x01020304;
    sqlite3_vfs vfs;
    vfs.xRandomness = nullptr;

    const int N = 10;
    char zBufOut[N];
    std::memset(zBufOut, 0xFF, sizeof(zBufOut)); // ensure we can detect zeros

    // Act
    int rc = sqlite3OsRandomness(&vfs, 10, zBufOut);

    // Assert
    EXPECT_EQ(rc, 0);
    unsigned int seedCopy = static_cast<unsigned int>(sqlite3Config.iPrngSeed);
    unsigned char seedBytes[4];
    std::memcpy(seedBytes, &seedCopy, sizeof(seedBytes));

    // First 4 bytes should be seedBytes
    EXPECT_BUF_EQ(reinterpret_cast<char*>(seedBytes), zBufOut, 4);
    // Next bytes (4..9) should be zeros because memcpy wrote only 4 bytes
    unsigned char zeros[6] = {0};
    EXPECT_BUF_EQ(reinterpret_cast<char*>(zeros), zBufOut + 4, 6);
}

// Main test runner
int main() {
    std::cout << "Running sqlite3OsRandomness unit tests...\n";

    test_seed_branch_writes_seed_and_zeros_for_extra_bytes();
    test_randomness_branch_calls_vfs_and_returns_its_value();
    test_seed_branch_copies_exact_nBytes_when_small();
    test_seed_branch_clamps_to_unsigned_size_and_zeros_rest();

    std::cout << "sqlite3OsRandomness unit tests completed.\n";
    return 0;
}