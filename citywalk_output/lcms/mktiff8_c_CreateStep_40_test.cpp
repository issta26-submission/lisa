// File: lcms2.h
#ifndef LCMS2_H
#define LCMS2_H

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned short cmsUInt16Number;

typedef struct cmsToneCurve {
    int n;
    cmsUInt16Number* table;
} cmsToneCurve;

cmsToneCurve* cmsBuildTabulatedToneCurve16(int /*ignore*/, int n, const cmsUInt16Number* Table);

double DecodeAbTIFF(double ab);

#ifdef __cplusplus
}
#endif

#endif // LCMS2_H



// File: mock_lcms2.cpp
#include <cstring>
#include <cstdio>
#include <math.h>
#include <cstdlib>
#include <stdlib.h>
#include <lcms2.h>
#include <cmath>


extern "C" int g_force_nullcalloc = 0; // 0 -> normal allocation, non-zero -> simulate calloc failure

// Override calloc to simulate allocation failure when requested by tests
extern "C" void* calloc(size_t nmemb, size_t size) {
    if (g_force_nullcalloc) {
        return NULL;
    }
    return std::calloc(nmemb, size);
}

// Mock implementation: build a tabulated tone curve by copying the provided table
extern "C" cmsToneCurve* cmsBuildTabulatedToneCurve16(int /*ignore*/, int n, const cmsUInt16Number* Table) {
    if (Table == NULL || n <= 0) return NULL;
    cmsToneCurve* curve = (cmsToneCurve*)std::malloc(sizeof(cmsToneCurve));
    if (curve == NULL) return NULL;
    curve->n = n;
    curve->table = (cmsUInt16Number*)std::malloc(n * sizeof(cmsUInt16Number));
    if (curve->table == NULL) {
        std::free(curve);
        return NULL;
    }
    std::memcpy(curve->table, Table, n * sizeof(cmsUInt16Number));
    return curve;
}

// Identity decode function (as used in the focal method)
extern "C" double DecodeAbTIFF(double ab) {
    return ab;
}



// File: test_CreateStep.cpp

// Declaration of the focal method under test
extern "C" cmsToneCurve* CreateStep(void);

// Expose DecodeAbTIFF for test computation of expected values
extern "C" double DecodeAbTIFF(double);

// Accessor for test to toggle calloc behavior (provided by mock_lcms2.cpp)
extern "C" int g_force_nullcalloc;

// Simple non-terminating test assertion macros
static int g_failures = 0;

#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        fprintf(stderr, "EXPECT_TRUE failed: %s at %s:%d\n", #cond, __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_NOT_NULL(ptr) do { \
    if ((ptr) == nullptr) { \
        fprintf(stderr, "EXPECT_NOT_NULL failed: pointer is NULL at %s:%d\n", __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_EQ(a, b) do { \
    if ((a) != (b)) { \
        fprintf(stderr, "EXPECT_EQ failed: %ld != %ld at %s:%d\n", (long)(a), (long)(b), __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while (0)

// Helper to compute expected value for a given index following CreateStep's logic
static unsigned short expected_value_for_index(int i) {
    double a = (double) i * 255.0 / 4095.0;
    a = DecodeAbTIFF(a);
    unsigned short val = (unsigned short) floor(a * 257.0 + 0.5);
    return val;
}

static int run_tests() {
    int failures_before = g_failures;

    // Test 1: Normal path (memory allocation succeeds)
    g_force_nullcalloc = 0;
    cmsToneCurve* gamma = CreateStep();
    EXPECT_NOT_NULL(gamma);
    if (gamma != nullptr) {
        EXPECT_EQ(gamma->n, 4096);
        if (gamma->table != nullptr) {
            // Validate a few representative entries against the same formula used in CreateStep
            unsigned short v0 = gamma->table[0];
            unsigned short vLast = gamma->table[4095];
            EXPECT_EQ(v0, (unsigned short)0);
            EXPECT_EQ(vLast, (unsigned short)65535);

            int test_indices[] = {1024, 2048, 3072};
            for (int idx = 0; idx < 3; ++idx) {
                int i = test_indices[idx];
                unsigned short got = gamma->table[i];
                unsigned short exp = expected_value_for_index(i);
                EXPECT_EQ((long)got, (long)exp);
            }
        } else {
            fprintf(stderr, "Gamma.table is NULL in normal path test\n");
            ++g_failures;
        }
        // Free allocated memory from mock
        if (gamma->table) free(gamma->table);
        free(gamma);
    }

    // Test 2: Null path (simulate calloc failure)
    g_force_nullcalloc = 1;
    cmsToneCurve* gamma_null = CreateStep();
    EXPECT_TRUE(gamma_null == nullptr);

    // Reset for potential subsequent tests
    g_force_nullcalloc = 0;

    // Test 3: Another run to ensure functionality remains after a previous NULL-path
    cmsToneCurve* gamma_again = CreateStep();
    EXPECT_NOT_NULL(gamma_again);
    if (gamma_again != nullptr) {
        // Basic sanity on returned object
        EXPECT_EQ(gamma_again->n, 4096);
        if (gamma_again->table) {
            unsigned short v0 = gamma_again->table[0];
            unsigned short vLast = gamma_again->table[4095];
            EXPECT_EQ(v0, (unsigned short)0);
            EXPECT_EQ(vLast, (unsigned short)65535);
        } else {
            fprintf(stderr, "Gamma_again->table is NULL in repeated normal path test\n");
            ++g_failures;
        }
        if (gamma_again->table) free(gamma_again->table);
        free(gamma_again);
    }

    return g_failures - failures_before;
}

int main(int argc, char** argv) {
    int failures = run_tests();
    if (failures == 0) {
        printf("All tests passed\n");
    } else {
        printf("Total failures: %d\n", failures);
    }
    return failures;
}