// Minimal C++11 test suite for the focal function DebugMemDontCheckThis
// This test is designed to be compiled alongside the project that provides
// the actual implementation and definitions (e.g., _cmsMemoryBlock, SIZE_OF_MEM_HEADER, etc.).
// No GTest usage; a lightweight, non-terminating assertion style is used.

#include <cstring>
#include <iostream>
#include <cstdint>
#include <testcms2.h>


// Include the project header that defines the dependent types and the focal function.
// The header (testcms2.h) is assumed to provide:
// - the _cmsMemoryBlock structure with at least a member 'DontCheck'
// - the macro SIZE_OF_MEM_HEADER
// - the declaration: void DebugMemDontCheckThis(void* Ptr)

// Ensure linkage with the focal C function
extern "C" void DebugMemDontCheckThis(void *Ptr);

// Helper macro for non-terminating assertions (prints on failure but continues)
#define ASSERT(cond, msg) do { if(!(cond)) { std::cerr << "[ASSERT FAIL] " << (msg) << "\n"; test_suite_failed = true; } } while(0)

static bool test_suite_failed = false;

/*
1) Test: DebugMemDontCheckThis should set the DontCheck flag to 1 for the header located SIZE_OF_MEM_HEADER bytes before Ptr.

   Rationale:
   - This validates the core contract of the focal function: it locates the memory header directly preceding Ptr
     by SIZE_OF_MEM_HEADER and marks DontCheck = 1.
   - We simulate a realistic header region and a payload pointer, then verify the flag is changed.
*/
bool test_DebugMemDontCheckThis_SetsDontCheckFlag() {
    // Determine header/payload layout using the project's macro.
    const std::size_t headerSize = SIZE_OF_MEM_HEADER;
    // Allocate a contiguous region: header region + payload region (header must precede payload)
    // Place the _cmsMemoryBlock header at the very start of the region (region[0]).
    // Ptr (payload) will point just after the header region.
    std::size_t region_total = headerSize + sizeof(_cmsMemoryBlock);

    // Allocate raw memory; use a type with proper alignment by using a byte buffer.
    uint8_t* region = new uint8_t[region_total];
    std::memset(region, 0, region_total);

    // Interpret the header region as _cmsMemoryBlock
    _cmsMemoryBlock* headerBlock = reinterpret_cast<_cmsMemoryBlock*>(region);
    // Initialize header fields as baseline (DontCheck assumed to be 0 beforehand)
    headerBlock->DontCheck = 0;

    // Ptr should point to the region just after the header
    void* payloadPtr = region + headerSize;

    // Invoke the focal function
    DebugMemDontCheckThis(payloadPtr);

    // Validate: header's DontCheck must now be 1
    bool ok = (headerBlock->DontCheck == 1);

    // Cleanup
    delete[] region;

    if (!ok) {
        std::cerr << "Test Failed: DebugMemDontCheckThis did not set DontCheck to 1 as expected.\n";
    }

    // Return result for reporting
    return ok;
}

/*
2) Test: DebugMemDontCheckThis should be idempotent with respect to the DontCheck flag
   (i.e., if DontCheck is already 1, calling DebugMemDontCheckThis again should leave it as 1).

   Rationale:
   - Ensures no side effects beyond setting the flag; function should not flip it back or corrupt memory.
*/
bool test_DebugMemDontCheckThis_Idempotent() {
    const std::size_t headerSize = SIZE_OF_MEM_HEADER;
    std::size_t region_total = headerSize + sizeof(_cmsMemoryBlock);

    uint8_t* region = new uint8_t[region_total];
    std::memset(region, 0, region_total);

    _cmsMemoryBlock* headerBlock = reinterpret_cast<_cmsMemoryBlock*>(region);
    headerBlock->DontCheck = 1; // Already set

    void* payloadPtr = region + headerSize;

    DebugMemDontCheckThis(payloadPtr);

    bool ok = (headerBlock->DontCheck == 1);

    delete[] region;

    if (!ok) {
        std::cerr << "Test Failed: DebugMemDontCheckThis altered an already-set DontCheck value.\n";
    }

    return ok;
}

int main() {
    std::cout << "Running tests for DebugMemDontCheckThis...\n";

    test_suite_failed = false;

    bool r1 = test_DebugMemDontCheckThis_SetsDontCheckFlag();
    if (r1) {
        std::cout << "[OK] test_DebugMemDontCheckThis_SetsDontCheckFlag\n";
    } else {
        std::cerr << "[FAILED] test_DebugMemDontCheckThis_SetsDontCheckFlag\n";
    }

    bool r2 = test_DebugMemDontCheckThis_Idempotent();
    if (r2) {
        std::cout << "[OK] test_DebugMemDontCheckThis_Idempotent\n";
    } else {
        std::cerr << "[FAILED] test_DebugMemDontCheckThis_Idempotent\n";
    }

    if (test_suite_failed) {
        std::cerr << "One or more tests FAILED.\n";
        return 1;
    }
    std::cout << "All tests completed.\n";
    return 0;
}