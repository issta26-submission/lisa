// Unit test suite for the AllocTable(cmsIT8* it8) function from cmscgats.c
// This test suite is written for C++11 (no Google Test). It uses a tiny
// non-terminating assertion framework (EXPECT_*) so tests continue on
// failure to improve coverage. The tests rely on the project's existing
// CMS data structures and macros (e.g., cmsBool, MAXTABLES, cmsIT8, TABLE).

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>


// Include core CMS internal declarations to gain access to types/macros

// Declare the focal function with C linkage for linking against cmscgats.c
extern "C" cmsBool AllocTable(cmsIT8* it8);

// Simple non-terminating test harness
static int g_passed = 0;
static int g_failed = 0;

#define EXPECT(cond, desc) do { \
    if(!(cond)) { \
        std::cerr << "[EXPECT_FAIL] " << (desc) \
                  << " (FILE: " << __FILE__ << ", LINE: " << __LINE__ << ")" << std::endl; \
        ++g_failed; \
    } else { \
        ++g_passed; \
    } \
} while(0)

#define RUN_TEST(test) \
    do { \
        std::cout << "Running " #test "..." << std::endl; \
        test(); \
    } while(0)

// Test 1: Exceeding allowed tables should return FALSE (no allocation)
void test_alloctable_exceed_limit_return_false()
{
    // Prepare a cmsIT8 instance with an array of TABLEs
    cmsIT8 it8;
    TABLE TabBuf[MAXTABLES];
    std::memset(&it8, 0, sizeof(it8));   // zero everything to avoid random pointers
    it8.Tab = TabBuf;

    // Set TablesCount to MAXTABLES-1 to exercise the boundary condition
    it8.TablesCount = MAXTABLES - 1;

    cmsBool res = AllocTable(&it8);

    EXPECT(res == FALSE, "AllocTable should return FALSE when TablesCount >= MAXTABLES-1");
}

// Test 2: Normal allocation when under the limit should initialize new table slots
void test_alloctable_basic_allocation_and_fields()
{
    cmsIT8 it8;
    TABLE TabBuf[MAXTABLES];
    std::memset(&it8, 0, sizeof(it8));
    it8.Tab = TabBuf;
    it8.TablesCount = 0;

    cmsBool res = AllocTable(&it8);

    EXPECT(res == TRUE, "AllocTable should return TRUE when TablesCount < MAXTABLES-1");
    EXPECT(it8.TablesCount == 1, "TablesCount should be incremented to 1 after allocation");

    // The newly allocated table should have its pointers set to NULL
    // (HeaderList, DataFormat, Data)
    EXPECT(TabBuf[0].HeaderList == NULL, "Newly allocated Table.HeaderList should be NULL");
    EXPECT(TabBuf[0].DataFormat == NULL, "Newly allocated Table.DataFormat should be NULL");
    EXPECT(TabBuf[0].Data == NULL, "Newly allocated Table.Data should be NULL");
}

// Test 3: Multiple allocations up to the limit should succeed and eventually fail on next call
void test_alloctable_multiple_allocations_up_to_limit()
{
    cmsIT8 it8;
    TABLE TabBuf[MAXTABLES];
    std::memset(&it8, 0, sizeof(it8));
    it8.Tab = TabBuf;
    it8.TablesCount = 0;

    // Perform MAXTABLES-1 allocations (the maximum allowed)
    for (int i = 0; i < (MAXTABLES - 1); ++i) {
        cmsBool res = AllocTable(&it8);
        EXPECT(res == TRUE, "AllocTable should succeed while under the limit");
        // After each successful allocation, TablesCount should equal i+1
        EXPECT(it8.TablesCount == (i + 1), "TablesCount should reflect the number of allocations");
        // The just-allocated slot should be initialized to NULL pointers
        EXPECT(TabBuf[i].HeaderList == NULL, "Allocated slot HeaderList should be NULL");
        EXPECT(TabBuf[i].DataFormat == NULL, "Allocated slot DataFormat should be NULL");
        EXPECT(TabBuf[i].Data == NULL, "Allocated slot Data should be NULL");
    }

    // Next call should fail as we've reached MAXTABLES-1 allocations
    cmsBool res = AllocTable(&it8);
    EXPECT(res == FALSE, "AllocTable should return FALSE after reaching the limit");
}

// Main entry: run all tests and summarize
int main()
{
    std::cout << "Starting AllocTable unit tests (no GTest)." << std::endl;

    RUN_TEST(test_alloctable_exceed_limit_return_false);
    RUN_TEST(test_alloctable_basic_allocation_and_fields);
    RUN_TEST(test_alloctable_multiple_allocations_up_to_limit);

    int total = g_passed + g_failed;
    std::cout << "Tests run: " << total << ", Passed: "
              << g_passed << ", Failed: " << g_failed << std::endl;

    // Non-terminating tests: exit code indicates any failure
    return (g_failed == 0) ? 0 : 1;
}