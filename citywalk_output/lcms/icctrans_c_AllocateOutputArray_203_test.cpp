/*
  Unit test suite for AllocateOutputArray (from icctrans.c)
  - No GTest; a lightweight test harness with non-terminating assertions.
  - Uses a minimal mock of the MATLAB Mex API (mxArray and related functions)
    to enable standalone compilation without the actual MATLAB runtime.
  - Tests focus on:
    - Expansion of the last dimension when OutputChannels != InputChannels
    - No-change path when OutputChannels == InputChannels
    - Correct handling across several class IDs (UINT8, DOUBLE, INT16, etc.)
  - Assumptions:
    - The real AllocateOutputArray is provided by icctrans.c and linked at build time.
    - The mock mex API mirrors the subset used by AllocateOutputArray.
*/

#include <cstring>
#include <mex.h>
#include <vector>
#include <stdarg.h>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <lcms2.h>
#include <sstream>
#include <string.h>


//////////////////////
// Mock Mex API (header-like definitions and implementations)
//////////////////////

// Lightweight representation of MATLAB's mxArray
struct mxArray {
    int numDimensions;   // number of dimensions
    int* dims;           // dimension sizes (length == numDimensions)
    int classID;           // data type class (simulate mxINT8_CLASS, etc.)
    void* data;            // pointer to raw data
};

// Class IDs (subset for tests)
enum {
    mxINT8_CLASS    = 0,
    mxUINT8_CLASS   = 1,
    mxINT16_CLASS   = 2,
    mxUINT16_CLASS  = 3,
    mxDOUBLE_CLASS  = 4
};

// Forward declare the function under test (from icctrans.c)
extern "C" mxArray* AllocateOutputArray(const mxArray* In, int OutputChannels);

// Helpers: determine element size for class
static size_t elementSize(int classID) {
    switch (classID) {
        case mxINT8_CLASS:   return sizeof(char);
        case mxUINT8_CLASS:  return sizeof(unsigned char);
        case mxINT16_CLASS:  return sizeof(short);
        case mxUINT16_CLASS: return sizeof(unsigned short);
        case mxDOUBLE_CLASS: return sizeof(double);
        default:             return sizeof(double);
    }
}

// mxMalloc / mxFree
static void* mxMalloc(size_t size) { return malloc(size); }
static void mxFree(void* p) { free(p); }

// Get number of elements from dimensions
static size_t totalElements(const mxArray* a) {
    if (!a || a->numDimensions <= 0 || !a->dims) return 0;
    size_t prod = 1;
    for (int i = 0; i < a->numDimensions; ++i) prod *= (size_t)a->dims[i];
    return prod;
}

// Create a deep copy of an mxArray (mock)
static mxArray* mxDuplicateArray(const mxArray* In) {
    if (!In) return nullptr;
    mxArray* Out = new mxArray;
    Out->numDimensions = In->numDimensions;
    if (In->dims && In->numDimensions > 0) {
        Out->dims = new int[In->numDimensions];
        std::memcpy(Out->dims, In->dims, In->numDimensions * sizeof(int));
    } else {
        Out->dims = nullptr;
    }
    Out->classID = In->classID;
    size_t nelems = totalElements(In);
    size_t esz = elementSize(In->classID);
    if (nelems > 0) {
        Out->data = mxMalloc(nelems * esz);
        std::memcpy(Out->data, In->data, nelems * esz);
    } else {
        Out->data = nullptr;
    }
    return Out;
}

// Query helpers
static int mxGetNumberOfDimensions(const mxArray* a) {
    return a ? a->numDimensions : 0;
}
static const int* mxGetDimensions(const mxArray* a) {
    return a ? a->dims : nullptr;
}
static int mxGetClassID(const mxArray* a) {
    return a ? a->classID : -1;
}

// Mutators
static void mxSetDimensions(mxArray* a, const int* dims, int nDimensions) {
    if (!a) return;
    if (a->dims) delete[] a->dims;
    a->dims = new int[nDimensions];
    std::memcpy(a->dims, dims, nDimensions * sizeof(int));
    a->numDimensions = nDimensions;
}
static void* mxGetPr(mxArray* a) {
    return a ? a->data : nullptr;
}
static void mxSetPr(mxArray* a, void* pr) {
    if (a) a->data = pr;
}
static void* mxRealloc(void* ptr, size_t size) {
    if (size == 0) {
        free(ptr);
        return nullptr;
    }
    void* np = realloc(ptr, size);
    return np ? np : ptr;
}

// A tiny factory to create input arrays for tests
static mxArray* createTestArray(const std::vector<int>& dims, int classID) {
    mxArray* a = new mxArray;
    a->numDimensions = (int)dims.size();
    a->dims = new int[dims.size()];
    for (size_t i = 0; i < dims.size(); ++i) a->dims[i] = dims[i];
    a->classID = classID;
    size_t nelems = 1;
    for (int d : dims) nelems *= (size_t)d;
    a->data = nullptr;
    if (nelems == 0) { a->data = nullptr; return a; }
    size_t esz = elementSize(classID);
    a->data = mxMalloc(nelems * esz);

    // Fill with deterministic data per class
    if (classID == mxUINT8_CLASS) {
        unsigned char* p = (unsigned char*)a->data;
        for (size_t i = 0; i < nelems; ++i) p[i] = (unsigned char)(i & 0xFF);
    } else if (classID == mxINT8_CLASS) {
        char* p = (char*)a->data;
        for (size_t i = 0; i < nelems; ++i) p[i] = (char)((i + 1) & 0xFF);
    } else if (classID == mxINT16_CLASS) {
        short* p = (short*)a->data;
        for (size_t i = 0; i < nelems; ++i) p[i] = (short)(i * 2);
    } else if (classID == mxUINT16_CLASS) {
        unsigned short* p = (unsigned short*)a->data;
        for (size_t i = 0; i < nelems; ++i) p[i] = (unsigned short)(i * 3);
    } else { // mxDOUBLE_CLASS
        double* p = (double*)a->data;
        for (size_t i = 0; i < nelems; ++i) p[i] = (double)i + 0.5;
    }
    return a;
}

// Compare helpers
template <typename T>
static bool arrayEquals(const T* a, const T* b, size_t n) {
    for (size_t i = 0; i < n; ++i) if (a[i] != b[i]) return false;
    return true;
}

static void freeMxArray(mxArray* a) {
    if (!a) return;
    if (a->dims) { delete[] a->dims; a->dims = nullptr; }
    if (a->data) { mxFree(a->data); a->data = nullptr; }
    delete a;
}

// We must implement the actual allocation function via the real icctrans.c.
// We declare it here to link with the provided implementation when building.
extern "C" mxArray* AllocateOutputArray(const mxArray* In, int OutputChannels);

// Compatibility wrappers: mimic the upstream API usage in icctrans.c
static int getTotalElements(const mxArray* a) { return (int)totalElements(a); }

// Simple pretty-printer for test failure messages
static std::string toString(const std::string& s) { return s; }

// Logging for non-terminating assertions
static void logFailure(const std::string& msg) {
    std::cerr << "TEST_FAILURE: " << msg << std::endl;
}

// Simple test assertion macros (non-terminating)
#define EXPECT_TRUE(cond) \
    do { if(!(cond)) { std::ostringstream os; os << "EXPECTED_TRUE failed: " << #cond; logFailure(os.str()); } } while(0)

#define EXPECT_EQ(a, b) \
    do { if(!((a) == (b))) { std::ostringstream os; os << "EXPECTED_EQ failed: " << #a << "=" << (a) << " vs " << #b << "=" << (b); logFailure(os.str()); } } while(0)

#define EXPECT_NEQ(a, b) \
    do { if((a) == (b)) { std::ostringstream os; os << "EXPECTED_NEQ failed: " << #a << " == " << #b; logFailure(os.str()); } } while(0)

#define EXPECT_MEM_EQ(a, b, n) \
    do { if(!arrayEquals(reinterpret_cast<const unsigned char*>(a), reinterpret_cast<const unsigned char*>(b), (n))) { std::ostringstream os; os << "EXPECTED_MEM_EQ failed at " << #a << " vs " << #b; logFailure(os.str()); } } while(0)

//////////////////////
// Test harness
//////////////////////

// Helper to cast and read data values from mxArray for verification (read-only)
static double readAsDouble(const mxArray* a, size_t idx) {
    if (!a || !a->data) return 0.0;
    if (a->classID == mxDOUBLE_CLASS) {
        double* p = (double*)a->data;
        return p[idx];
    } else if (a->classID == mxINT16_CLASS) {
        short* p = (short*)a->data;
        return static_cast<double>(p[idx]);
    } else if (a->classID == mxUINT16_CLASS) {
        unsigned short* p = (unsigned short*)a->data;
        return static_cast<double>(p[idx]);
    } else if (a->classID == mxUINT8_CLASS) {
        unsigned char* p = (unsigned char*)a->data;
        return static_cast<double>(p[idx]);
    } else if (a->classID == mxINT8_CLASS) {
        char* p = (char*)a->data;
        return static_cast<double>(p[idx]);
    }
    return 0.0;
}

// Test 1: Expansion for UINT8 class
static void test_expand_uint8() {
    // In: dims {2,3}, class UINT8
    std::vector<int> dims = {2, 3};
    mxArray* In = createTestArray(dims, mxUINT8_CLASS);
    // Save original In data for later comparison
    size_t nelemsIn = totalElements(In);
    unsigned char* original = (unsigned char*)malloc(nelemsIn * sizeof(unsigned char));
    std::memcpy(original, In->data, nelemsIn * sizeof(unsigned char));

    int NewChannels = 5;
    mxArray* Out = AllocateOutputArray(In, NewChannels);

    // Assertions on Out
    EXPECT_TRUE(Out != nullptr);
    if (Out) {
        EXPECT_EQ(Out->numDimensions, In->numDimensions);
        const int* od = Out->dims;
        // Expect last dimension to be Updated to 5
        EXPECT_EQ(od[0], In->dims[0]);
        EXPECT_EQ(od[1], NewChannels);
        // Verify total elements
        int totalOut = od[0] * od[1];
        EXPECT_EQ(totalOut, 2 * 5);

        // First part of data should match In (since Out is a deep copy then resized)
        unsigned char* outData = (unsigned char*)Out->data;
        for (size_t i = 0; i < nelemsIn; ++i) {
            EXPECT_EQ(outData[i], ((unsigned char*)In->data)[i]);
        }

        // In should remain unchanged
        for (size_t i = 0; i < nelemsIn; ++i) {
            EXPECT_EQ(((unsigned char*)In->data)[i], original[i]);
        }

        // Cleanup
        free(outData); // allocated by mxRealloc in real code; in test we free directly
        // Note: mxFree/actual deallocation is handled in freeMxArray below; here we mimic
    }

    // Free resources
    free(original);
    freeMxArray(In);
    freeMxArray(Out);
}

// Test 2: No-change path when OutputChannels equals InputChannels (DOUBLE)
static void test_no_change_double() {
    // In: dims {2,4}, class DOUBLE
    std::vector<int> dims = {2, 4};
    mxArray* In = createTestArray(dims, mxDOUBLE_CLASS);
    // Copy In for later comparison
    mxArray* InCopy = mxDuplicateArray(In);

    int NewChannels = 4; // same as InputChannels
    mxArray* Out = AllocateOutputArray(In, NewChannels);

    // Assertions
    EXPECT_TRUE(Out != nullptr);
    if (Out) {
        // Dimensions should remain the same
        EXPECT_EQ(Out->numDimensions, In->numDimensions);
        const int* od = Out->dims;
        EXPECT_EQ(od[0], In->dims[0]);
        EXPECT_EQ(od[1], In->dims[1]);

        // Data should be identical
        size_t nelems = totalElements(In);
        const double* inData = (const double*)InCopy->data;
        const double* outData = (const double*)Out->data;
        for (size_t i = 0; i < nelems; ++i) {
            EXPECT_EQ(outData[i], inData[i]);
        }

        // In should be unchanged
        for (size_t i = 0; i < nelems; ++i) {
            EXPECT_EQ(((const double*)In->data)[i], inData[i]);
        }

        // Cleanup
        // nothing
    }

    freeMxArray(In);
    freeMxArray(InCopy);
    freeMxArray(Out);
}

// Test 3: Expansion for INT16 class
static void test_expand_int16() {
    // In: dims {3,2}, class INT16
    std::vector<int> dims = {3, 2};
    mxArray* In = createTestArray(dims, mxINT16_CLASS);
    size_t nelemsIn = totalElements(In);

    int NewChannels = 3;
    mxArray* Out = AllocateOutputArray(In, NewChannels);

    EXPECT_TRUE(Out != nullptr);
    if (Out) {
        const int* od = Out->dims;
        EXPECT_EQ(od[0], In->dims[0]);
        EXPECT_EQ(od[1], NewChannels);
        size_t totalOut = od[0] * od[1];
        EXPECT_EQ(totalOut, (int)(3 * 3));

        // First portion should match In
        const short* inData = (const short*)In->data;
        const short* outData = (const short*)Out->data;
        for (size_t i = 0; i < nelemsIn; ++i) {
            EXPECT_EQ(outData[i], inData[i]);
        }
    }

    freeMxArray(In);
    freeMxArray(Out);
}

// Test 4: Ensure multiple test coverage by running all tests
static void runAllTests() {
    test_expand_uint8();
    test_no_change_double();
    test_expand_int16();
}

int main() {
    std::cout << "Starting AllocateOutputArray unit tests (with mocked Mex API)..." << std::endl;
    runAllTests();
    std::cout << "Unit tests completed. If no test failures were printed above, the tests likely passed (non-terminating assertions)." << std::endl;
    return 0;
}