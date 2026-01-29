// Test suite for png_set_sCAL_s in a self-contained environment (C++11, no GTest)
// This test creates a minimal fake PNG environment sufficient to exercise
// the focal function's logic, including success paths and various failure paths.
// It intentionally avoids depending on the real libpng headers or runtime.

#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <stdexcept>
#include <cstring>


// Domain knowledge guidelines applied:
// - Use only standard library and provided methods.
// - Create executable tests without GTest; run from main.
// - Include explanatory comments for each test.
// - Cover true/false branches of predicates (unit validity, string validity, etc.).
// - Access static-like helpers via direct calls, not via private internals.

// Lightweight local stubs and structures to mimic the required dependencies of png_set_sCAL_s

// Flags (borrowed style from libpng usage)
#define PNG_FREE_SCAL 1
#define PNG_INFO_sCAL 2

// Macro to cast void* to desired pointer type in place
#ifndef png_voidcast
#define png_voidcast(type, x) ((type)(x))
#endif

// Forward declarations of the minimal surface used by the focal function
struct png_struct_def;
struct png_info_struct; // for infor pointer

typedef png_struct_def* png_structrp;
typedef const png_struct_def* png_const_structrp;

typedef png_info_struct* png_inforp;
typedef const char* png_const_charp;
typedef char* png_charp;
typedef unsigned char png_byte;
typedef unsigned int png_uint_32;
typedef void* png_bytep;

// Lightweight info struct matching required fields by png_set_sCAL_s
struct png_info_struct {
    png_byte scal_unit;
    png_charp scal_s_width;
    png_charp scal_s_height;
    unsigned int free_me;
    unsigned int valid;
};

// Lightweight PNG struct (opaque to the function under test)
struct png_struct_def { int dummy; };

// Global state to simulate controlled allocation failures for unit tests
static int g_malloc_call_count = 0;
static bool g_fail_first_malloc = false;
static bool g_fail_second_malloc = false;

// Utility: minimal strlen wrapper (use std::strlen)
static size_t t_strlen(const char* s) {
    return std::strlen(s);
}

// Memory management simulators
static void* png_malloc_warn(png_structrp /*png_ptr*/, size_t size) {
    ++g_malloc_call_count;
    if (g_fail_first_malloc && g_malloc_call_count == 1) {
        return nullptr; // simulate first allocation failure (width)
    }
    if (g_fail_second_malloc && g_malloc_call_count == 2) {
        return nullptr; // simulate second allocation failure (height)
    }
    // Normal allocation
    return std::malloc(size);
}

static void png_free(png_structrp /*png_ptr*/, void* ptr) {
    if (ptr) std::free(ptr);
}

// Error/warning stubs
static void png_warning(png_const_structrp /*png_ptr*/, const char* /*message*/) {
    // For tests, warnings are non-fatal; just ignore or log if needed
}

struct PngTestError : public std::runtime_error {
    explicit PngTestError(const std::string& msg) : std::runtime_error(msg) {}
};

// Simple error by throwing an exception to emulate libpng error handling in tests
static void png_error(png_const_structrp /*png_ptr*/, const char* message) {
    throw PngTestError(message ? message : "png_error");
}

// Simple debug stub
static void png_debug1(int /*level*/, const char* /*format*/, ...) {
    // No-op for tests
}

// Simple check for a "floating-point" string: digits, '.', 'e'/'E', '+', '-'
static bool png_check_fp_string(const char* s, size_t length) {
    if (s == nullptr || length == 0) return false;
    for (size_t i = 0; i < length; ++i) {
        char c = s[i];
        if (!((c >= '0' && c <= '9') || c == '.' || c == 'e' || c == 'E' || c == '+' || c == '-'))
            return false;
    }
    return true;
}

// Prototype of the focal function under test (pseudocode-compatible signature)
static void png_set_sCAL_s(png_const_structrp png_ptr, png_inforp info_ptr,
    int unit, png_const_charp swidth, png_const_charp sheight);

// Implementation of the focal function under test (cleaned, self-contained)
static void png_set_sCAL_s(png_const_structrp png_ptr, png_inforp info_ptr,
    int unit, png_const_charp swidth, png_const_charp sheight)
{
{
   size_t lengthw = 0, lengthh = 0;
   png_debug1(1, "in %s storage function", "sCAL");
   if (png_ptr == NULL || info_ptr == NULL)
      return;
   /* Double check the unit (should never get here with an invalid
    * unit unless this is an API call.)
    */
   if (unit != 1 && unit != 2)
      png_error(png_ptr, "Invalid sCAL unit");
   if (swidth == NULL || (lengthw = t_strlen(swidth)) == 0 ||
       swidth[0] == 45 /* '-' */ || !png_check_fp_string(swidth, lengthw))
      png_error(png_ptr, "Invalid sCAL width");
   if (sheight == NULL || (lengthh = t_strlen(sheight)) == 0 ||
       sheight[0] == 45 /* '-' */ || !png_check_fp_string(sheight, lengthh))
      png_error(png_ptr, "Invalid sCAL height");
   info_ptr->scal_unit = (png_byte)unit;
   ++lengthw;
   png_debug1(3, "allocating unit for info (%u bytes)", (unsigned int)lengthw);
   info_ptr->scal_s_width = png_voidcast(png_charp,
       png_malloc_warn(png_ptr, lengthw));
   if (info_ptr->scal_s_width == NULL)
   {
      png_warning(png_ptr, "Memory allocation failed while processing sCAL");
      return;
   }
   memcpy(info_ptr->scal_s_width, swidth, lengthw);
   ++lengthh;
   png_debug1(3, "allocating unit for info (%u bytes)", (unsigned int)lengthh);
   info_ptr->scal_s_height = png_voidcast(png_charp,
       png_malloc_warn(png_ptr, lengthh));
   if (info_ptr->scal_s_height == NULL)
   {
      png_free(png_ptr, info_ptr->scal_s_width);
      info_ptr->scal_s_width = NULL;
      png_warning(png_ptr, "Memory allocation failed while processing sCAL");
      return;
   }
   memcpy(info_ptr->scal_s_height, sheight, lengthh);
   info_ptr->free_me |= PNG_FREE_SCAL;
   info_ptr->valid |= PNG_INFO_sCAL;
}
}

// Helper to access the test info struct fields without C++ name mangling
// (not used directly; keeping structure for compatibility with the rest of tests)
#define NULLPTR nullptr

// Candidate Keywords derived from Step 1:
// - sCAL unit validation, width string validity, height string validity
// - Memory allocation for sCAL width/height, error and warning handling
// - info_ptr fields: scal_unit, scal_s_width, scal_s_height, free_me, valid

// Test harness
struct TestSuite {
    int passed;
    int failed;
    std::vector<std::string> failures;

    TestSuite() : passed(0), failed(0) {}
    void fail(const std::string& msg) {
        failed++;
        failures.push_back(msg);
        std::cerr << "FAIL: " << msg << std::endl;
    }
    void succeed(const std::string& msg) {
        passed++;
        std::cout << "PASS: " << msg << std::endl;
    }

    bool run_all();
};

// Test 1: Valid inputs allocate and set info correctly
static bool test_sCAL_s_valid_allocation() {
    // Prepare fake PNG info struct
    png_struct_def* png_ptr = new png_struct_def();
    png_inforp info_ptr = new png_info_struct();

    // Initialize info_ptr fields
    static_cast<png_info_struct*>(info_ptr)->scal_unit = 0;
    static_cast<png_info_struct*>(info_ptr)->scal_s_width = NULL;
    static_cast<png_info_struct*>(info_ptr)->scal_s_height = NULL;
    static_cast<png_info_struct*>(info_ptr)->free_me = 0;
    static_cast<png_info_struct*>(info_ptr)->valid = 0;

    const char* swidth = "1.25";
    const char* sheight = "2.75";

    try {
        png_set_sCAL_s(reinterpret_cast<png_const_structrp>(png_ptr),
                       info_ptr, 1, swidth, sheight);
    } catch (const PngTestError& e) {
        delete info_ptr;
        delete png_ptr;
        std::cerr << "Exception in test_sCAL_s_valid_allocation: " << e.what() << std::endl;
        return false;
    } catch (...) {
        delete info_ptr;
        delete png_ptr;
        std::cerr << "Unknown exception in test_sCAL_s_valid_allocation" << std::endl;
        return false;
    }

    png_info_struct* info = reinterpret_cast<png_info_struct*>(info_ptr);

    bool ok = true;
    if (info->scal_unit != 1) { ok = false; std::cerr << "scal_unit not set to 1" << std::endl; }
    if (info->scal_s_width == NULL) { ok = false; std::cerr << "scal_s_width not allocated" << std::endl; }
    if (info->scal_s_height == NULL) { ok = false; std::cerr << "scal_s_height not allocated" << std::endl; }

    if (info->scal_s_width && std::strcmp(info->scal_s_width, swidth) != 0) {
        ok = false;
        std::cerr << "scal_s_width content mismatch" << std::endl;
    }
    if (info->scal_s_height && std::strcmp(info->scal_s_height, sheight) != 0) {
        ok = false;
        std::cerr << "scal_s_height content mismatch" << std::endl;
    }

    if ((info->free_me & PNG_FREE_SCAL) == 0) { ok = false; std::cerr << "free_me not set for SCAL" << std::endl; }
    if ((info->valid & PNG_INFO_sCAL) == 0) { ok = false; std::cerr << "valid not set for sCAL" << std::endl; }

    // Cleanup
    if (info->scal_s_width) std::free(info->scal_s_width);
    if (info->scal_s_height) std::free(info->scal_s_height);
    delete info_ptr;
    delete png_ptr;

    return ok;
}

// Test 2: Invalid unit triggers error
static bool test_sCAL_s_invalid_unit_triggers_error() {
    png_struct_def* png_ptr = new png_struct_def();
    png_inforp info_ptr = new png_info_struct();

    // Init
    static_cast<png_info_struct*>(info_ptr)->scal_unit = 0;
    static_cast<png_info_struct*>(info_ptr)->scal_s_width = NULL;
    static_cast<png_info_struct*>(info_ptr)->scal_s_height = NULL;
    static_cast<png_info_struct*>(info_ptr)->free_me = 0;
    static_cast<png_info_struct*>(info_ptr)->valid = 0;

    const char* swidth = "1.0";
    const char* sheight = "2.0";

    bool caught = false;
    try {
        png_set_sCAL_s(reinterpret_cast<png_const_structrp>(png_ptr),
                       info_ptr, 3, swidth, sheight); // invalid unit
    } catch (const PngTestError& e) {
        caught = true;
        // Expect specific message
        if (std::string(e.what()) != "Invalid sCAL unit") {
            std::cerr << "Unexpected error message: " << e.what() << std::endl;
            caught = false;
        }
    } catch (...) {
        std::cerr << "Unexpected exception type" << std::endl;
        caught = false;
    }

    delete info_ptr;
    delete png_ptr;
    return caught;
}

// Test 3: Invalid width string triggers error (NULL)
static bool test_sCAL_s_invalid_width_null() {
    png_struct_def* png_ptr = new png_struct_def();
    png_inforp info_ptr = new png_info_struct();

    static_cast<png_info_struct*>(info_ptr)->scal_unit = 0;
    static_cast<png_info_struct*>(info_ptr)->scal_s_width = NULL;
    static_cast<png_info_struct*>(info_ptr)->scal_s_height = NULL;
    static_cast<png_info_struct*>(info_ptr)->free_me = 0;
    static_cast<png_info_struct*>(info_ptr)->valid = 0;

    const char* swidth = NULL;
    const char* sheight = "2.0";

    bool caught = false;
    try {
        png_set_sCAL_s(reinterpret_cast<png_const_structrp>(png_ptr),
                       info_ptr, 1, swidth, sheight);
    } catch (const PngTestError& e) {
        caught = true;
        if (std::string(e.what()) != "Invalid sCAL width") {
            std::cerr << "Unexpected error message: " << e.what() << std::endl;
            caught = false;
        }
    } catch (...) {
        std::cerr << "Unexpected exception type in invalid width test" << std::endl;
        caught = false;
    }

    delete info_ptr;
    delete png_ptr;
    return caught;
}

// Test 4: Invalid height string triggers error (NULL)
static bool test_sCAL_s_invalid_height_null() {
    png_struct_def* png_ptr = new png_struct_def();
    png_inforp info_ptr = new png_info_struct();

    static_cast<png_info_struct*>(info_ptr)->scal_unit = 0;
    static_cast<png_info_struct*>(info_ptr)->scal_s_width = NULL;
    static_cast<png_info_struct*>(info_ptr)->scal_s_height = NULL;
    static_cast<png_info_struct*>(info_ptr)->free_me = 0;
    static_cast<png_info_struct*>(info_ptr)->valid = 0;

    const char* swidth = "1.0";
    const char* sheight = NULL;

    bool caught = false;
    try {
        png_set_sCAL_s(reinterpret_cast<png_const_structrp>(png_ptr),
                       info_ptr, 1, swidth, sheight);
    } catch (const PngTestError& e) {
        caught = true;
        if (std::string(e.what()) != "Invalid sCAL height") {
            std::cerr << "Unexpected error message: " << e.what() << std::endl;
            caught = false;
        }
    } catch (...) {
        std::cerr << "Unexpected exception type in invalid height test" << std::endl;
        caught = false;
    }

    delete info_ptr;
    delete png_ptr;
    return caught;
}

// Test 5: Width allocation fails (first malloc) -> should warn and return without setting width/height
static bool test_sCAL_s_width_allocation_failure() {
    // Configure first allocation to fail
    g_malloc_call_count = 0;
    g_fail_first_malloc = true;
    g_fail_second_malloc = false;

    png_struct_def* png_ptr = new png_struct_def();
    png_inforp info_ptr = new png_info_struct();

    static_cast<png_info_struct*>(info_ptr)->scal_unit = 0;
    static_cast<png_info_struct*>(info_ptr)->scal_s_width = NULL;
    static_cast<png_info_struct*>(info_ptr)->scal_s_height = NULL;
    static_cast<png_info_struct*>(info_ptr)->free_me = 0;
    static_cast<png_info_struct*>(info_ptr)->valid = 0;

    const char* swidth = "1.0";
    const char* sheight = "2.0";

    bool ok = true;
    try {
        png_set_sCAL_s(reinterpret_cast<png_const_structrp>(png_ptr),
                       info_ptr, 1, swidth, sheight);
    } catch (...) {
        ok = false;
        std::cerr << "Exception in width allocation failure test" << std::endl;
    }

    // Expect: scal_s_width == NULL, scal_s_height == NULL, scal_unit set but allocations failed
    png_info_struct* info = reinterpret_cast<png_info_struct*>(info_ptr);
    bool condition = true;
    if (info->scal_s_width != NULL) { condition = false; }
    if (info->scal_s_height != NULL) { condition = false; }
    if (info->scal_unit != 1) { condition = false; }

    // Cleanup
    delete info_ptr;
    delete png_ptr;
    // Reset allocation flags for other tests
    g_fail_first_malloc = false;
    g_malloc_call_count = 0;

    return ok && condition;
}

// Test 6: Height allocation fails after width succeeded -> both width and height should be NULL
static bool test_sCAL_s_height_allocation_failure() {
    g_malloc_call_count = 0;
    g_fail_first_malloc = false;
    g_fail_second_malloc = true; // fail on second allocation (height)

    png_struct_def* png_ptr = new png_struct_def();
    png_inforp info_ptr = new png_info_struct();

    static_cast<png_info_struct*>(info_ptr)->scal_unit = 0;
    static_cast<png_info_struct*>(info_ptr)->scal_s_width = NULL;
    static_cast<png_info_struct*>(info_ptr)->scal_s_height = NULL;
    static_cast<png_info_struct*>(info_ptr)->free_me = 0;
    static_cast<png_info_struct*>(info_ptr)->valid = 0;

    const char* swidth = "1.0";
    const char* sheight = "2.0";

    bool ok = true;
    try {
        png_set_sCAL_s(reinterpret_cast<png_const_structrp>(png_ptr),
                       info_ptr, 1, swidth, sheight);
    } catch (...) {
        ok = false;
        std::cerr << "Exception in height allocation failure test" << std::endl;
    }

    png_info_struct* info = reinterpret_cast<png_info_struct*>(info_ptr);
    bool condition = true;
    if (info->scal_s_width != NULL) condition = false;
    if (info->scal_s_height != NULL) condition = false;
    if (info->scal_unit != 1) condition = false;

    delete info_ptr;
    delete png_ptr;
    g_fail_second_malloc = false;
    g_malloc_call_count = 0;
    return ok && condition;
}

// Test runner
bool TestSuite::run_all() {
    bool overall = true;

    if (test_sCAL_s_valid_allocation()) {
        succeed("test_sCAL_s_valid_allocation");
        passed++;
    } else {
        fail("test_sCAL_s_valid_allocation");
        overall = false;
    }

    if (test_sCAL_s_invalid_unit_triggers_error()) {
        succeed("test_sCAL_s_invalid_unit_triggers_error");
        passed++;
    } else {
        fail("test_sCAL_s_invalid_unit_triggers_error");
        overall = false;
    }

    if (test_sCAL_s_invalid_width_null()) {
        succeed("test_sCAL_s_invalid_width_null");
        passed++;
    } else {
        fail("test_sCAL_s_invalid_width_null");
        overall = false;
    }

    if (test_sCAL_s_invalid_height_null()) {
        succeed("test_sCAL_s_invalid_height_null");
        passed++;
    } else {
        fail("test_sCAL_s_invalid_height_null");
        overall = false;
    }

    if (test_sCAL_s_width_allocation_failure()) {
        succeed("test_sCAL_s_width_allocation_failure");
        passed++;
    } else {
        fail("test_sCAL_s_width_allocation_failure");
        overall = false;
    }

    if (test_sCAL_s_height_allocation_failure()) {
        succeed("test_sCAL_s_height_allocation_failure");
        passed++;
    } else {
        fail("test_sCAL_s_height_allocation_failure");
        overall = false;
    }

    return overall;
}

// Entrypoint
int main() {
    TestSuite suite;
    bool ok = suite.run_all();
    std::cout << "\nTest results: " << suite.passed << " passed, "
              << suite.failed << " failed." << std::endl;
    if (!ok) {
        std::cerr << "One or more tests failed." << std::endl;
        return 1;
    }
    return 0;
}