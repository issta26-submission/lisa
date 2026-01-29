// C++11 test harness for the focal method CheckMD5 (no GTest, simple main-based tests)
// This harness provides minimal stub implementations for the CMS API used by CheckMD5
// to enable deterministic, self-contained unit tests without requiring the real library.
// The tests focus on exercising true/false branches of the MD5-based ID comparison.

#include <cstring>
#include <iostream>
#include <cstdint>
#include <testcms2.h>


#ifdef __cplusplus
extern "C" {
#endif

// Lightweight type aliases to match the expectations in the focal code.
typedef void* cmsHPROFILE;
typedef int cmsBool;
typedef unsigned int cmsUInt32Number;

// Structure used by CheckMD5 to hold the 16-byte header IDs.
typedef struct {
    unsigned char ID8[16];
} cmsProfileID;

// Prototypes of the functions used by the focal method.
// We provide simple stubs below implementing deterministic behavior for tests.
cmsHPROFILE cmsOpenProfileFromFile(const char* FileName, const char* Mode);
int cmsMD5computeID(cmsHPROFILE pProfile);
void cmsGetHeaderProfileID(cmsHPROFILE pProfile, unsigned char* Dest);
void cmsCloseProfile(cmsHPROFILE pProfile);

#ifdef __cplusplus
}
#endif

// Global test harness state to drive deterministic behavior of stubs.
static bool gForceMismatchSlot1 = false; // When true, second profile (Slot 1) will yield a mismatched ID pair to test failure.
static int  g_md5_calls[4] = {0, 0, 0, 0}; // Per-slot counters: 0..3 slots (two profiles per test)
static int  g_nextSlot = 0; // Next slot index to allocate on cmsOpenProfileFromFile

// Simple allocator for fake profiles (slots 0..3). Returns a pointer that encodes the slot index.
cmsHPROFILE cmsOpenProfileFromFile(const char* /*FileName*/, const char* /*Mode*/) {
    if (g_nextSlot >= 4) {
        return nullptr;
    }
    int slot = g_nextSlot++;
    // Encode the slot index into a pointer-like value for use by the stubbed API.
    return reinterpret_cast<cmsHPROFILE>(static_cast<intptr_t>(slot));
}

// Stub: returns true (1) for the first two MD5 computations per slot, then false.
int cmsMD5computeID(cmsHPROFILE pProfile) {
    int slot = static_cast<int>(reinterpret_cast<intptr_t>(pProfile));
    if (slot < 0 || slot >= 4) return 0;
    if (g_md5_calls[slot] < 2) {
        g_md5_calls[slot]++;
        return 1;
    }
    return 0;
}

// Helper to choose deterministic header IDs based on the current MD5 computation state.
// We purposefully craft the IDs to support both passing and failing scenarios in tests.
void cmsGetHeaderProfileID(cmsHPROFILE pProfile, unsigned char* Dest) {
    int slot = static_cast<int>(reinterpret_cast<intptr_t>(pProfile));
    if (slot < 0 || slot >= 4) {
        // Invalid handle; fill zeros to avoid undefined behavior.
        std::memset(Dest, 0, 16);
        return;
    }
    int count = g_md5_calls[slot];
    // Scenario: slot 1 can force a mismatch on the second ID if requested.
    if (slot == 1 && gForceMismatchSlot1 && count >= 2) {
        // Fill with a different value on the second ID to create a mismatch with slot 0's second ID.
        for (int i = 0; i < 16; ++i) Dest[i] = static_cast<unsigned char>(0x12);
        return;
    }
    // Default deterministic fill: first and second MD5 calls fill with 0x11.
    // This ensures ID1 == ID3 and ID2 == ID4 under the default (no-mismatch) scenario.
    if (count >= 1) {
        for (int i = 0; i < 16; ++i) Dest[i] = static_cast<unsigned char>(0x11);
        return;
    }
    // Fallback (shouldn't happen in tested flow): zeroed buffer.
    std::memset(Dest, 0, 16);
}

// The focal method (provided by the subject under test); adapted to rely on the stubs above.
// Note: The original function signature uses C linkage in the tested code; keep the same for compatibility.
int CheckMD5(void)
{
    cmsHPROFILE pProfile = cmsOpenProfileFromFile("sRGBlcms2.icc", "r");
    cmsProfileID ProfileID1, ProfileID2, ProfileID3, ProfileID4;

    if (cmsMD5computeID(pProfile)) cmsGetHeaderProfileID(pProfile, ProfileID1.ID8);
    if (cmsMD5computeID(pProfile)) cmsGetHeaderProfileID(pProfile, ProfileID2.ID8);
    cmsCloseProfile(pProfile);

    pProfile = cmsOpenProfileFromFile("sRGBlcms2.icc", "r");

    if (cmsMD5computeID(pProfile)) cmsGetHeaderProfileID(pProfile, ProfileID3.ID8);
    if (cmsMD5computeID(pProfile)) cmsGetHeaderProfileID(pProfile, ProfileID4.ID8);
    cmsCloseProfile(pProfile);

    return ((memcmp(ProfileID1.ID8, ProfileID3.ID8, sizeof(ProfileID1)) == 0) &&
            (memcmp(ProfileID2.ID8, ProfileID4.ID8, sizeof(ProfileID2)) == 0));
}

// Utility to reset the MD5 slipstream state before each test.
void ResetMD5Simulation()
{
    for (int i = 0; i < 4; ++i) g_md5_calls[i] = 0;
    g_nextSlot = 0;
    gForceMismatchSlot1 = false;
}

// Lightweight test harness (no GTest). Uses plain stdout and simple macros to report status.
#define TEST_ASSERT_EQ(expr, expected, msg)                                     \
    do {                                                                          \
        if ((expr) != (expected)) {                                             \
            std::cerr << "[FAIL] " << (msg) << " | "                               \
                      << "Expected: " << (expected)                                  \
                      << ", Got: " << (expr) << std::endl;                       \
        } else {                                                                  \
            std::cout << "[PASS] " << (msg) << std::endl;                       \
        }                                                                         \
    } while (0)

static void Test_CheckMD5_Passes()
{
    // Scenario: default behavior should produce a passing check (ID1==ID3 and ID2==ID4).
    ResetMD5Simulation();
    // No mismatch flag; both pairs should align.
    int result = CheckMD5();
    TEST_ASSERT_EQ(result, 1, "CheckMD5 should return non-zero (pass) when IDs match");
}

static void Test_CheckMD5_Fails_Slot1SecondID()
{
    // Scenario: force a mismatch on the second ID derived from slot 1 to falsify the result.
    ResetMD5Simulation();
    gForceMismatchSlot1 = true;
    int result = CheckMD5();
    TEST_ASSERT_EQ(result, 0, "CheckMD5 should return zero (fail) when slot1 second ID mismatches ID4");
}

int main(int argc, char* argv[])
{
    // Run tests in a simple, deterministic fashion.
    std::cout << "Running unit tests for CheckMD5 (no external test framework)..." << std::endl;

    Test_CheckMD5_Passes();
    Test_CheckMD5_Fails_Slot1SecondID();

    std::cout << "Unit tests completed." << std::endl;
    return 0;
}