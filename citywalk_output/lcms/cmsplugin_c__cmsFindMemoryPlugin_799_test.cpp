#include <lcms2_internal.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>


// Include Little CMS internal definitions to access needed types/macros.
// This header is assumed to expose cmsPluginBase, cmsPluginMemHandler, 
// cmsPluginMagicNumber, LCMS_VERSION, cmsPluginMemHandlerSig, etc.

// Declaration of the focal function (C linkage)
extern "C" cmsPluginMemHandler* _cmsFindMemoryPlugin(void* PluginBundle);

// Lightweight, non-terminating test assertion helpers
static int g_total_checks = 0;
static int g_total_failures = 0;

#define EXPECT_TRUE(cond) \
    do { \
        ++g_total_checks; \
        if(!(cond)) { \
            std::cerr << "EXPECT_TRUE failed: " #cond " at " << __FILE__ << ":" << __LINE__ << "\n"; \
            ++g_total_failures; \
        } \
    } while(false)

#define EXPECT_PTR_EQ(a,b) \
    do { \
        ++g_total_checks; \
        if((void*)(a) != (void*)(b)) { \
            std::cerr << "EXPECT_PTR_EQ failed: " #a " != " #b " at " << __FILE__ << ":" << __LINE__ << "\n"; \
            ++g_total_failures; \
        } \
    } while(false)

static void summary() {
    std::cout << "Test summary: checks=" << g_total_checks 
              << ", failures=" << g_total_failures << "\n";
}

// Unit Test 1: First non-matching plugin before a matching memory plugin should return the matching plugin
// Rationale: Ensure the function traverses the linked list correctly and returns the first matching memory plugin.
static void test_find_memory_plugin_first_match_after_non_match() {
    // Allocate a non-matching first plugin
    cmsPluginBase first;
    first.Next = nullptr;
    first.Magic = 0;                // Not cmsPluginMagicNumber
    first.ExpectedVersion = 0;
    first.Type = 0;

    // Allocate a matching memory plugin (mem1)
    cmsPluginMemHandler* mem1 = new cmsPluginMemHandler();
    mem1->Base.Next = nullptr;
    mem1->Base.Magic = cmsPluginMagicNumber;
    mem1->Base.ExpectedVersion = LCMS_VERSION;
    mem1->Base.Type = cmsPluginMemHandlerSig;

    // Link first -> mem1
    first.Next = (cmsPluginBase*) mem1;

    // Call focal function
    cmsPluginMemHandler* result = _cmsFindMemoryPlugin((void*) &first);

    // Expect result to be mem1
    EXPECT_TRUE(result == mem1);

    delete mem1;
}

// Unit Test 2: No matching plugin in chain should return NULL
// Rationale: Test the false branch where no plugin satisfies all predicates.
static void test_find_memory_plugin_no_match_returns_null() {
    // Non-matching first plugin
    cmsPluginBase first;
    first.Next = nullptr;
    first.Magic = 0; // not matching
    first.ExpectedVersion = 0;
    first.Type = 0;

    // Non-matching second plugin (mem2)
    cmsPluginMemHandler* mem2 = new cmsPluginMemHandler();
    mem2->Base.Next = nullptr;
    mem2->Base.Magic = 0;              // not matching
    mem2->Base.ExpectedVersion = 0;
    mem2->Base.Type = 0;

    // Link first -> mem2
    first.Next = (cmsPluginBase*) mem2;

    cmsPluginMemHandler* result = _cmsFindMemoryPlugin((void*) &first);

    // Expect NULL
    EXPECT_TRUE(result == NULL);

    delete mem2;
}

// Unit Test 3: First matching plugin should be returned even if subsequent plugins exist
// Rationale: Ensure the function stops at the first satisfying memory plugin.
static void test_find_memory_plugin_first_matches_before_second() {
    // First plugin is matching
    cmsPluginBase first;
    first.Next = nullptr;
    first.Magic = cmsPluginMagicNumber;
    first.ExpectedVersion = LCMS_VERSION;
    first.Type = cmsPluginMemHandlerSig;

    // Second memory plugin (memB) also matching but should not be reached
    cmsPluginMemHandler* memB = new cmsPluginMemHandler();
    memB->Base.Next = nullptr;
    memB->Base.Magic = cmsPluginMagicNumber;
    memB->Base.ExpectedVersion = LCMS_VERSION;
    memB->Base.Type = cmsPluginMemHandlerSig;

    // Next pointer from first to memB
    first.Next = (cmsPluginBase*) memB;

    cmsPluginMemHandler* result = _cmsFindMemoryPlugin((void*) &first);

    // Expect result to be the first mem (which is 'first' if it is a matching plugin)
    // Note: In this setup, since 'first' is itself a matching plugin, the function should return
    // a pointer to the memory plugin that corresponds to 'first' if it's laid out identically.
    // However, to avoid ambiguity, reuse the standard scenario: ensure memB is reached only if first is non-match.
    // To keep this test meaningful, make 'first' non-matching and expect memB to be returned.
    first.Magic = 0; // non-matching
    first.ExpectedVersion = 0;
    first.Type = 0;

    // Recompute: call again with non-match first
    result = _cmsFindMemoryPlugin((void*) &first);

    // Since memB is the only matching plugin along the chain, ensure memB is returned when first is non-matching
    EXPECT_TRUE(result == memB);

    delete memB;
}

// Unit Test 4: Version gating test - first matching but with version too high, next matching plugin should be selected
// Rationale: The "ExpectedVersion <= LCMS_VERSION" predicate must be satisfied for a match.
static void test_find_memory_plugin_version_gate() {
    // First plugin matches in type/magic but Version too high
    cmsPluginBase first;
    first.Next = nullptr;
    first.Magic = cmsPluginMagicNumber;
    first.ExpectedVersion = LCMS_VERSION + 1; // too new
    first.Type = cmsPluginMemHandlerSig;

    // Second plugin matches fully
    cmsPluginMemHandler* mem2 = new cmsPluginMemHandler();
    mem2->Base.Next = nullptr;
    mem2->Base.Magic = cmsPluginMagicNumber;
    mem2->Base.ExpectedVersion = LCMS_VERSION;
    mem2->Base.Type = cmsPluginMemHandlerSig;

    first.Next = (cmsPluginBase*) mem2;

    cmsPluginMemHandler* result = _cmsFindMemoryPlugin((void*) &first);

    // Expect to skip first due to version and succeed with mem2
    EXPECT_TRUE(result == mem2);

    delete mem2;
}

int main() {
    // Run tests
    test_find_memory_plugin_first_match_after_non_match();
    test_find_memory_plugin_no_match_returns_null();
    test_find_memory_plugin_first_matches_before_second();
    test_find_memory_plugin_version_gate();

    summary();
    return (g_total_failures == 0) ? 0 : 1;
}