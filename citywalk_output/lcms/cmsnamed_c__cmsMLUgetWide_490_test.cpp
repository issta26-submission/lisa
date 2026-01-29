// cmsnamed_test.cpp
// Lightweight C++11 test suite for _cmsMLUgetWide (no Google Test, no GMock)
// This test harness builds a minimal cmsMLU structure and _cmsMLUentry data
// to exercise the focal function _cmsMLUgetWide from cmsnamed.c.
// It uses a small, custom assertion framework to allow tests to continue after
// failures (non-terminating assertions).

#include <cstring>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cwchar>
#include <lcms2_internal.h>


// Import necessary dependencies and declare the focal function.
// The internal cms types are defined in lcms2 internal headers in the real project.
extern "C" {
    // Path may vary depending on project layout. Adjust if needed.
    #include "lcms2_internal.h"

    // Declaration of the focal function under test.
    // We mirror the signature exactly as in the focal method snippet.
    const wchar_t* _cmsMLUgetWide(const cmsMLU* mlu,
                                  cmsUInt32Number *len,
                                  cmsUInt16Number LanguageCode, cmsUInt16Number CountryCode,
                                  cmsUInt16Number* UsedLanguageCode, cmsUInt16Number* UsedCountryCode);
}

// Simple non-terminating assertion framework
static int g_test_failures = 0;

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " #cond << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_test_failures; \
    } \
} while(false)

#define EXPECT_FALSE(cond) do { \
    if((cond)) { \
        std::cerr << "EXPECT_FALSE failed: " #cond << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_test_failures; \
    } \
} while(false)

#define EXPECT_EQ(a,b) do { \
    if(!((a) == (b))) { \
        std::cerr << "EXPECT_EQ failed: " #a " == " #b " (" << (a) << " != " << (b) << ") at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_test_failures; \
    } \
} while(false)

#define EXPECT_PTR_EQ(a,b) do { \
    if((a) != (b)) { \
        std::cerr << "EXPECT_PTR_EQ failed: " #a " (" << (void*)(a) << ") != " #b " (" << (void*)(b) \
                  << ") at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_test_failures; \
    } \
} while(false)

#define EXPECT_NOT_NULL(p) do { \
    if((p) == nullptr) { \
        std::cerr << "EXPECT_NOT_NULL failed: " #p " is NULL at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_test_failures; \
    } \
} while(false)


// Helper to create a minimal cmsMLU with two entries and a simple memory pool
struct TestMLUBuilder {
    cmsMLU* mlu;
    _cmsMLUentry* entries;
    cmsUInt8Number* pool;

    TestMLUBuilder() : mlu(nullptr), entries(nullptr), pool(nullptr) {}
    ~TestMLUBuilder() { cleanup(); }

    void cleanup() {
        if (entries) { delete[] entries; entries = nullptr; }
        if (mlu) { // Free nested allocations if created
            // MemPool is a void* in cmsMLU; delete the pool
            if (mlu->MemPool) free(mlu->MemPool);
            free(mlu);
            mlu = nullptr;
        }
        // pool points to MemPool; already freed via MemPool if allocated
        pool = nullptr;
    }

    // Build a simple mlu with two entries and a pool big enough for two strings
    cmsMLU* buildTwoEntriesWithStrings(const wchar_t* s1, size_t len1, const wchar_t* s2, size_t len2) {
        // allocate mlu structure
        mlu = (cmsMLU*) malloc(sizeof(cmsMLU));
        if (!mlu) return nullptr;
        mlu->AllocatedEntries = 2;
        mlu->UsedEntries = 2;
        mlu->Entries = new _cmsMLUentry[2];
        mlu->MemPool = nullptr;
        mlu->PoolSize = 0;

        // setup first entry
        entries = mlu->Entries;
        entries[0].Language = 1;
        entries[0].Country  = 2;
        entries[0].Len      = (cmsUInt32Number) len1;
        entries[0].StrW     = 0; // start of pool

        // setup second entry
        entries[1].Language = 1;
        entries[1].Country  = 3;
        entries[1].Len      = (cmsUInt32Number) len2;
        entries[1].StrW     = (cmsUInt32Number) (len1 * sizeof(wchar_t)); // after first string

        // allocate memory pool
        size_t poolBytes = (len1 + len2) * sizeof(wchar_t);
        pool = (cmsUInt8Number*) malloc(poolBytes);
        if (!pool) {
            cleanup();
            return nullptr;
        }
        mlu->MemPool = pool;
        mlu->PoolSize = (cmsUInt32Number) poolBytes;

        // copy strings into pool
        memcpy(pool + entries[0].StrW, s1, len1 * sizeof(wchar_t));
        memcpy(pool + entries[1].StrW, s2, len2 * sizeof(wchar_t));

        return mlu;
    }
};


// Test 1: Guard clause - mlu == NULL should return NULL
void test_null_mlu_returns_null() {
    const wchar_t* res = _cmsMLUgetWide(nullptr, nullptr, 1, 2, nullptr, nullptr);
    EXPECT_TRUE(res == nullptr);
}

// Test 2: Guard clause - AllocatedEntries <= 0 should return NULL
void test_allocated_entries_zero_returns_null() {
    cmsMLU mlu;
    mlu.AllocatedEntries = 0;
    mlu.UsedEntries = 0;
    mlu.Entries = nullptr;
    mlu.MemPool = nullptr;
    mlu.PoolSize = 0;

    const wchar_t* res = _cmsMLUgetWide(&mlu, nullptr, 1, 2, nullptr, nullptr);
    EXPECT_TRUE(res == nullptr);
}

// Test 3: Exact match path - exact LanguageCode + CountryCode found
void test_exact_match_returns_correct_string_and_metadata() {
    TestMLUBuilder builder;
    static const wchar_t helloW[] = L"Hello";
    static const wchar_t worldW[] = L"World";

    // Build an MLU with two entries:
    // Entry 0: Language 1, Country 2 -> "Hello" (len=5, StrW=0)
    // Entry 1: Language 1, Country 3 -> "World" (len=5, StrW=10)
    cmsMLU* mlu = builder.buildTwoEntriesWithStrings(helloW, 5, worldW, 5);
    if (mlu == nullptr) {
        std::cerr << "Memory allocation failed in test_exact_match_returns_correct_string_and_metadata" << std::endl;
        ++g_test_failures;
        return;
    }

    // Call with exact match: LanguageCode=1, CountryCode=2
    cmsUInt32Number lenOut = 0;
    cmsUInt16Number UsedLang = 0, UsedCountry = 0;

    const wchar_t* res = _cmsMLUgetWide(mlu, &lenOut, 1, 2, &UsedLang, &UsedCountry);

    // Validate
    EXPECT_NOT_NULL(res); // non-null pointer expected
    if (res) {
        // Pointer should point to MemPool + 0
        const wchar_t* expectedPtr = (const wchar_t*) ((cmsUInt8Number*) mlu->MemPool + 0);
        EXPECT_PTR_EQ(res, expectedPtr);

        // Content should be exactly "Hello" with length 5
        std::wstring got(res, res + (size_t) lenOut);
        EXPECT_TRUE(got == std::wstring(helloW, helloW + 5)); // compare content
        EXPECT_EQ((size_t)lenOut, (size_t)5);

        // UsedLanguageCode and UsedCountryCode should reflect the exact match
        EXPECT_EQ(UsedLang, (cmsUInt16Number)1);
        EXPECT_EQ(UsedCountry, (cmsUInt16Number)2);
    }

    // cleanup
    builder.cleanup();
}

// Test 4: No exact match but same language exists; Best should point to first entry
void test_no_exact_match_uses_first_entry_when_language_matches() {
    TestMLUBuilder builder;
    static const wchar_t helloW[] = L"Hello";
    static const wchar_t worldW[] = L"World";

    cmsMLU* mlu = builder.buildTwoEntriesWithStrings(helloW, 5, worldW, 5);
    if (mlu == nullptr) {
        std::cerr << "Memory allocation failed in test_no_exact_match_uses_first_entry_when_language_matches" << std::endl;
        ++g_test_failures;
        return;
    }

    // Use LanguageCode=1, CountryCode that doesn't exist (e.g., 99)
    cmsUInt32Number lenOut = 0;
    cmsUInt16Number UsedLang = 0, UsedCountry = 0;

    const wchar_t* res = _cmsMLUgetWide(mlu, &lenOut, 1, 99, &UsedLang, &UsedCountry);

    // Should return the first entry's string ("Hello")
    EXPECT_NOT_NULL(res);
    if (res) {
        const wchar_t* expectedPtr = (const wchar_t*) ((cmsUInt8Number*) mlu->MemPool + 0);
        EXPECT_PTR_EQ(res, expectedPtr);

        std::wstring got(res, res + (size_t) lenOut);
        EXPECT_TRUE(got == std::wstring(helloW, helloW + 5));
        EXPECT_EQ((size_t)lenOut, (size_t)5);

        // The UsedLanguageCode should reflect the first entry's language, Country should reflect first entry's country
        EXPECT_EQ(UsedLang, (cmsUInt16Number)1);
        EXPECT_EQ(UsedCountry, (cmsUInt16Number)2);
    }

    // cleanup
    builder.cleanup();
}

// Test 5: Insufficient pool size triggers NULL return path
void test_insufficient_pool_returns_null() {
    TestMLUBuilder builder;
    static const wchar_t helloW[] = L"Hello";

    // Build with same as above but then call with PoolSize too small to hold string
    cmsMLU* mlu = builder.buildTwoEntriesWithStrings(helloW, 5, L"World", 5);
    if (mlu == nullptr) {
        std::cerr << "Memory allocation failed in test_insufficient_pool_returns_null" << std::endl;
        ++g_test_failures;
        return;
    }

    // Force an exact match scenario but with tiny pool to trigger NULL
    // We'll adjust pool size to be less than Len (per code's (StrW + Len) > PoolSize check)
    mlu->PoolSize = 4; // intentionally too small
    // Note: The test relies on the same entries; Len remains 5 for entry 0

    cmsUInt32Number lenOut = 0;
    cmsUInt16Number UsedLang = 0, UsedCountry = 0;

    const wchar_t* res = _cmsMLUgetWide(mlu, &lenOut, 1, 2, &UsedLang, &UsedCountry);

    // Expect NULL due to too-small pool (as per the code path)
    EXPECT_TRUE(res == nullptr);

    // cleanup
    builder.cleanup();
}

// Test 6: mlu NULL or other edge cases covered already; ensure no crash when len is NULL or UsedX is NULL
void test_null_len_and_null_used_pointers_do_not_crash() {
    TestMLUBuilder builder;
    static const wchar_t helloW[] = L"Hello";
    cmsMLU* mlu = builder.buildTwoEntriesWithStrings(helloW, 5, L"World", 5);
    if (mlu == nullptr) {
        std::cerr << "Memory allocation failed in test_null_len_and_null_used_pointers_do_not_crash" << std::endl;
        ++g_test_failures;
        return;
    }

    // Call with len = NULL and Used* pointers = NULL to ensure no crash
    const wchar_t* res = _cmsMLUgetWide(mlu, nullptr, 1, 2, nullptr, nullptr);
    // This should still attempt to resolve; since len is NULL, function returns pointer without error
    EXPECT_NOT_NULL(res);

    builder.cleanup();
}


// Entry point for tests
int main() {
    test_null_mlu_returns_null();
    test_allocated_entries_zero_returns_null();
    test_exact_match_returns_correct_string_and_metadata();
    test_no_exact_match_uses_first_entry_when_language_matches();
    test_insufficient_pool_returns_null();
    test_null_len_and_null_used_pointers_do_not_crash();

    if (g_test_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_test_failures << " test(s) failed." << std::endl;
        return 1;
    }
}