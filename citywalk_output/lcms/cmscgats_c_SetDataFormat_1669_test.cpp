// cmscgats_test_SetDataFormat.cpp
// A lightweight, non-GTest C++11 test harness for the focal method SetDataFormat
// This test exercises the SetDataFormat function in cmscgats.c and its dependent
// components (GetTable, AllocateDataFormat, AllocString, GetDataFormat, etc.).
// It uses non-terminating assertions (EXPECT_* macros) to maximize code coverage
// and continues execution after failures.

#include <lcms2_internal.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>


// The tests rely on the project's CMS/LCMS internal API.
// The exact types and function signatures are provided by the project's headers.
// Include the internal header that declares cmsBool, cmsIT8, TABLE, GetTable, etc.
// If your build uses different headers, adjust accordingly.
extern "C" {
    // Core declarations required by SetDataFormat and its dependencies.
    // The following declarations are assumed to be available from the project.
    // If your environment uses slightly different prototypes, adapt as needed.

    // Typedefs and constants (as used by the project)
    typedef int cmsBool;
    typedef void* cmsContext;
    typedef struct TABLE TABLE;
    typedef struct cmsIT8 cmsIT8;

    // Boolean literals (if not defined by the included headers)
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

    // Core functions used by SetDataFormat's implementation (as declared in the project)
    TABLE* GetTable(cmsIT8* it8);
    cmsBool AllocateDataFormat(cmsIT8* it8);
    char* AllocString(cmsIT8* it8, const char* str);
    const char* GetDataFormat(cmsIT8* it8, int n);
    void SynError(cmsIT8* it8, const char* Txt, ...);

    // The function under test
    cmsBool SetDataFormat(cmsIT8* it8, int n, const char *label);

    // Context and allocation helpers (likely used to prepare test objects)
    cmsContext cmsCreateContext(void* UserData, void* ClientData);
    cmsIT8* cmsIT8Alloc(cmsContext Context);
}

// Lightweight test harness
static int g_totalTests = 0;
static int g_failedTests = 0;

// Non-terminating assertion macros
#define EXPECT_TRUE(cond) do { \
    ++g_totalTests; \
    if (!(cond)) { \
        fprintf(stderr, "TEST FAILED: %s:%d: EXPECT_TRUE(%s) is false\n", __FILE__, __LINE__, #cond); \
        ++g_failedTests; \
    } \
} while (0)

#define EXPECT_FALSE(cond) do { \
    ++g_totalTests; \
    if (cond) { \
        fprintf(stderr, "TEST FAILED: %s:%d: EXPECT_FALSE(%s) is true\n", __FILE__, __LINE__, #cond); \
        ++g_failedTests; \
    } \
} while (0)

#define EXPECT_EQ_INT(a, b) do { \
    ++g_totalTests; \
    if ((a) != (b)) { \
        fprintf(stderr, "TEST FAILED: %s:%d: EXPECT_EQ_INT(%d, %d)\n", __FILE__, __LINE__, (int)(a), (int)(b)); \
        ++g_failedTests; \
    } \
} while (0)

#define EXPECT_STR_EQ(a, b) do { \
    ++g_totalTests; \
    if ((a) == NULL && (b) == NULL) { \
        /* both NULL, pass */ \
    } else if ((a) == NULL || (b) == NULL) { \
        fprintf(stderr, "TEST FAILED: %s:%d: EXPECT_STR_EQ(NULL vs non-NULL) \"%s\" vs \"%s\"\n", __FILE__, __LINE__, (a? (a) : "NULL"), (b? (b) : "NULL")); \
        ++g_failedTests; \
    } else if (strcmp((a), (b)) != 0) { \
        fprintf(stderr, "TEST FAILED: %s:%d: EXPECT_STR_EQ(\"%s\", \"%s\")\n", __FILE__, __LINE__, (a), (b)); \
        ++g_failedTests; \
    } \
} while (0)

// Helper: create a test CMS IT8 context/object
static cmsIT8* CreateTestIt8(int nSamples, bool dataFormatNull)
{
    // Create a CMS context (parameters are not used by tests; pass NULL-like values)
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    // Allocate a cmsIT8 instance
    cmsIT8* it8 = cmsIT8Alloc(ctx);
    if (it8 == NULL) return NULL;

    // Obtain the associated table and configure basics
    TABLE* t = GetTable(it8);
    if (t == NULL) return it8; // leave as-is; tests may fail later if NULL

    t->nSamples = nSamples;
    if (dataFormatNull) {
        t->DataFormat = NULL;
    } else {
        // Pre-allocate DataFormat array with non-NULL placeholders
        t->DataFormat = (char**)malloc(sizeof(char*) * (nSamples > 0 ? nSamples : 1));
        if (t->DataFormat != NULL) {
            for (int i = 0; i < nSamples; ++i) {
                t->DataFormat[i] = AllocString(it8, "INIT");
            }
        }
    }
    return it8;
}

// Test 1: Normal path when DataFormat is NULL and Allocation succeeds
static void test_SetDataFormat_NormalPathAllocAndSet()
{
    // Prepare an IT8 object with a table having nSamples >= 3 and DataFormat initially NULL
    cmsIT8* it8 = CreateTestIt8(4, true);
    if (it8 == NULL) {
        fprintf(stderr, "Test 1: Failed to allocate cmsIT8 for test\n");
        ++g_failedTests;
        ++g_totalTests;
        return;
    }

    // Ensure initial state: DataFormat is NULL
    TABLE* t = GetTable(it8);
    EXPECT_TRUE(t != NULL);
    EXPECT_TRUE(t->DataFormat == NULL);

    // Action: SetDataFormat should allocate and set the string for index 2
    cmsBool ok = SetDataFormat(it8, 2, "Label-Alpha");
    EXPECT_TRUE(ok == TRUE);

    // Verify: DataFormat[2] should be "Label-Alpha"
    const char* v = GetDataFormat(it8, 2);
    EXPECT_STR_EQ(v, "Label-Alpha");

    // Cleanup
    if (t->DataFormat) {
        for (int i = 0; i < t->nSamples; ++i) {
            free(t->DataFormat[i]);
        }
        free(t->DataFormat);
        t->DataFormat = NULL;
    }
}

// Test 2: Bound check: n >= t->nSamples should return FALSE
static void test_SetDataFormat_BoundsCheckFalse()
{
    // Prepare IT8 with nSamples = 3
    cmsIT8* it8 = CreateTestIt8(3, true);
    if (it8 == NULL) {
        fprintf(stderr, "Test 2: Failed to allocate cmsIT8 for test\n");
        ++g_failedTests;
        ++g_totalTests;
        return;
    }

    // Action: Request an index out of bounds (3) since valid indices are 0..2
    cmsBool ok = SetDataFormat(it8, 3, "OutOfBounds");
    EXPECT_FALSE(ok == TRUE);

    // No change expected; DataFormat should remain NULL (since it8 had DataFormat NULL initially)
    TABLE* t = GetTable(it8);
    if (t) {
        EXPECT_TRUE(t->DataFormat == NULL);
    }

    // Cleanup
    if (t && t->DataFormat) {
        for (int i = 0; i < t->nSamples; ++i) {
            free(t->DataFormat[i]);
        }
        free(t->DataFormat);
        t->DataFormat = NULL;
    }
}

// Test 3: DataFormat already allocated (non-NULL) - verify assignment works
static void test_SetDataFormat_ExistingDataFormatAssignment()
{
    // Prepare IT8 with nSamples = 2 and DataFormat pre-allocated (non-NULL)
    cmsIT8* it8 = CreateTestIt8(2, false);
    if (it8 == NULL) {
        fprintf(stderr, "Test 3: Failed to allocate cmsIT8 for test\n");
        ++g_failedTests;
        ++g_totalTests;
        return;
    }

    // Precondition: DataFormat array exists (non-NULL). We'll set a placeholder for index 1
    TABLE* t = GetTable(it8);
    if (t->DataFormat == NULL) {
        t->DataFormat = (char**)malloc(sizeof(char*) * t->nSamples);
        for (int i = 0; i < t->nSamples; ++i) {
            t->DataFormat[i] = NULL;
        }
    }

    // Ensure index 1 is currently NULL to observe the new allocation
    EXPECT_TRUE(t->DataFormat[1] == NULL);

    // Action: SetDataFormat should allocate a new string for index 1
    cmsBool ok = SetDataFormat(it8, 1, "Label-Second");
    EXPECT_TRUE(ok == TRUE);

    // Verify: DataFormat[1] should be "Label-Second"
    const char* v = GetDataFormat(it8, 1);
    EXPECT_STR_EQ(v, "Label-Second");

    // Cleanup
    if (t->DataFormat) {
        for (int i = 0; i < t->nSamples; ++i) {
            if (t->DataFormat[i]) {
                free(t->DataFormat[i]);
            }
        }
        free(t->DataFormat);
        t->DataFormat = NULL;
    }
}

// Test 4: Null label triggers AllocString failure path (AllocString returns NULL)
// We simulate by passing NULL as the label when DataFormat is already allocated.
// Expect SetDataFormat to return FALSE.
static void test_SetDataFormat_NullLabelFailurePath()
{
    // Prepare IT8 with nSamples = 1 and pre-allocated DataFormat
    cmsIT8* it8 = CreateTestIt8(1, false);
    if (it8 == NULL) {
        fprintf(stderr, "Test 4: Failed to allocate cmsIT8 for test\n");
        ++g_failedTests;
        ++g_totalTests;
        return;
    }

    TABLE* t = GetTable(it8);
    // Ensure DataFormat array exists and is non-NULL
    if (t->DataFormat == NULL) {
        t->DataFormat = (char**)malloc(sizeof(char*) * t->nSamples);
        t->DataFormat[0] = NULL;
    }

    // Action: Pass NULL label to AllocString; expected to fail (return NULL)
    cmsBool ok = SetDataFormat(it8, 0, NULL);
    EXPECT_FALSE(ok == TRUE);

    // Cleanup
    if (t->DataFormat) {
        for (int i = 0; i < t->nSamples; ++i) {
            if (t->DataFormat[i]) free(t->DataFormat[i]);
        }
        free(t->DataFormat);
        t->DataFormat = NULL;
    }
}

// Driver: run all tests
int main()
{
    printf("Running SetDataFormat test suite (no GTest) - C++11\n");

    test_SetDataFormat_NormalPathAllocAndSet();
    test_SetDataFormat_BoundsCheckFalse();
    test_SetDataFormat_ExistingDataFormatAssignment();
    test_SetDataFormat_NullLabelFailurePath();

    printf("Tests run: %d, Failures: %d\n", g_totalTests, g_failedTests);
    if (g_failedTests > 0) {
        printf("Some tests failed. Review the log above for details.\n");
    } else {
        printf("All tests passed.\n");
    }

    return (g_failedTests == 0) ? 0 : 1;
}