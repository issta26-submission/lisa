// Unit test suite for the focal function: SearchMLUEntry
// This test suite is designed for C++11 without using GTest.
// It uses the project's internal LCMS types (cmsMLU, cmsMLUEntry, cmsUInt16Number, etc.)
// and calls the C function SearchMLUEntry via C linkage.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdlib>


extern "C" int SearchMLUEntry(cmsMLU* mlu, cmsUInt16Number LanguageCode, cmsUInt16Number CountryCode);

// Namespace to group test helpers and tests
namespace Test_SearchMLUEntry {

    // Test 1: Sanity check - passing NULL pointer should return -1
    // Verifies that the function handles a NULL mlu input gracefully.
    bool test_NullPointer() {
        int res = SearchMLUEntry(nullptr, 1, 1);
        return res == -1;
    }

    // Test 2: Empty MLU - with UsedEntries == 0 should return -1
    // Ensures that an empty MLU does not crash and reports not found.
    bool test_EmptyMlu() {
        cmsMLU mlu;
        mlu.UsedEntries = 0;
        mlu.Entries = nullptr;
        int res = SearchMLUEntry(&mlu, 1, 1);
        return res == -1;
    }

    // Test 3: Single entry matches exactly
    // The entry at index 0 has matching LanguageCode and CountryCode; expect index 0.
    bool test_SingleMatch() {
        cmsMLU mlu;
        mlu.UsedEntries = 1;
        mlu.Entries = new cmsMLUEntry[1];
        mlu.Entries[0].Language = 0x1234;
        mlu.Entries[0].Country  = 0xABCD;
        int res = SearchMLUEntry(&mlu, 0x1234, 0xABCD);
        delete[] mlu.Entries;
        return res == 0;
    }

    // Test 4: No entry matches due to different LanguageCode
    // The only entry has a different LanguageCode; expect -1.
    bool test_NotFoundDifferentLanguage() {
        cmsMLU mlu;
        mlu.UsedEntries = 1;
        mlu.Entries = new cmsMLUEntry[1];
        mlu.Entries[0].Language = 0x1111;
        mlu.Entries[0].Country  = 0x2222;
        int res = SearchMLUEntry(&mlu, 0x9999, 0x2222);
        delete[] mlu.Entries;
        return res == -1;
    }

    // Test 5: Matching entry is at the end (highest index)
    // Ensures that the loop properly checks all entries up to UsedEntries-1.
    bool test_MatchAtEnd() {
        cmsMLU mlu;
        mlu.UsedEntries = 3;
        mlu.Entries = new cmsMLUEntry[3];
        mlu.Entries[0].Language = 1; mlu.Entries[0].Country = 2;
        mlu.Entries[1].Language = 3; mlu.Entries[1].Country = 4;
        mlu.Entries[2].Language = 5; mlu.Entries[2].Country = 6;
        int res = SearchMLUEntry(&mlu, 5, 6);
        delete[] mlu.Entries;
        return res == 2;
    }

} // namespace Test_SearchMLUEntry

int main() {
    using namespace Test_SearchMLUEntry;

    int total = 0;
    int failed = 0;

    auto run = [&](const char* name, bool ok) {
        ++total;
        if (!ok) {
            ++failed;
            std::cerr << "[FAILED] " << name << "\n";
        } else {
            std::cout << "[PASSED] " << name << "\n";
        }
    };

    // Execute tests
    run("SearchMLUEntry_NullPointer", test_NullPointer());
    run("SearchMLUEntry_EmptyMlu", test_EmptyMlu());
    run("SearchMLUEntry_SingleMatch", test_SingleMatch());
    run("SearchMLUEntry_NotFoundDifferentLanguage", test_NotFoundDifferentLanguage());
    run("SearchMLUEntry_MatchAtEnd", test_MatchAtEnd());

    std::cout << "Total tests: " << total << ", Passed: " << (total - failed)
              << ", Failed: " << failed << "\n";

    // Return non-zero if any test failed
    return failed ? 1 : 0;
}