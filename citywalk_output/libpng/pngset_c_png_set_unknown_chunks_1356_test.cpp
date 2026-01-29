// Self-contained C++11 test harness for the focal method
// png_set_unknown_chunks, re-implemented here to be testable without
// requiring the full libpng project. This test focuses on validating
// the allocation, copying, and bookkeeping behavior described in the
// provided FOCAL_METHOD (png_set_unknown_chunks).
//
// Notes:
// - This is a lightweight, self-contained mock tailored to exercise
//   core logic of the unknown chunk handling as described.
// - We avoid GTest; instead we provide lightweight EXPECT_* macros.
// - We compile this as a single translation unit with -std=c++11.

#include <cerrno>
#include <cstdlib>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>


// -------------------- Minimal Mocked API & Types --------------------

// Forward declare C-style structs to mirror libpng-like types.
struct png_struct;
struct png_info;
struct png_unknown_chunk;

// Typedefs to resemble libpng naming used by the focal function
typedef const struct png_struct* png_const_structrp;
typedef struct png_struct*       png_structrp;
typedef struct png_info*           png_inforp;
typedef const struct png_unknown_chunk* png_const_unknown_chunkp;
typedef struct png_unknown_chunk*       png_unknown_chunkp;
typedef unsigned char*             png_bytep;
typedef void*                      png_voidp;
typedef const unsigned char*       png_const_bytep;

// Helper: cast macro used in the focal code
#define png_voidcast(type, val) ((type)(val))

// Basic bit masks/constants
#define PNG_FREE_UNKN 0x01
#define PNG_IS_READ_STRUCT 0x01
#define PNG_CHUNK_WRITE_ERROR 1

// Compile-time guards to avoid invoking error-path code in tests
#define PNG_READ_UNKNOWN_CHUNKS_SUPPORTED 1
#define PNG_WRITE_UNKNOWN_CHUNKS_SUPPORTED 1
#define PNG_READ_SUPPORTED 1
#define PNG_WRITE_SUPPORTED 1

// Bridge functions (provided locally for testing)
static bool g_fail_alloc = false;           // Simulate memory allocation failure
static bool g_chunk_report_called = false;   // Track if error reporter invoked

// Simple memory management wrappers to simulate libpng helpers
static void* png_malloc_base(png_structrp /*png_ptr*/, size_t size)
{
    if (g_fail_alloc) return NULL;
    return std::malloc(size);
}

static void png_free(png_structrp /*png_ptr*/, void* ptr)
{
    if (ptr) std::free(ptr);
}

// Realloc helper that mimics libpng's realloc_array usage
static void* png_realloc_array(png_structrp /*png_ptr*/,
                               void* old_ptr,
                               unsigned int /*old_num*/,
                               unsigned int new_num,
                               size_t item_size)
{
    if (g_fail_alloc) return NULL;
    size_t total = new_num * item_size;
    // realloc(NULL, size) behaves like malloc(size)
    void* newp = std::realloc(old_ptr, total);
    return newp;
}

// Simple chunk reporting stub
static void png_chunk_report(png_structrp /*png_ptr*/, const char* /*message*/, int /*error_code*/)
{
    g_chunk_report_called = true;
}

// Location check stub (identity mapping for test)
static int check_location(png_const_structrp /*png_ptr*/, int location)
{
    return location;
}

// Minimal struct definitions to back the focal function
struct png_struct {
    unsigned int mode; // keep a minimal field to satisfy checks if needed
};

struct png_unknown_chunk {
    char name[5]; // 4 chars + NUL
    int location;
    unsigned char* data;
    size_t size;
};

struct png_info {
    png_unknown_chunk* unknown_chunks;
    unsigned int unknown_chunks_num;
    unsigned int free_me;
};

// Typedefs to mirror the signature of the focal function
typedef const struct png_struct* png_const_structrp;
typedef struct png_struct* png_structrp;
typedef struct png_info* png_inforp;
typedef const struct png_unknown_chunk* png_const_unknown_chunkp;
typedef struct png_unknown_chunk* png_unknown_chunkp;

// The focal method: png_set_unknown_chunks
void png_set_unknown_chunks(png_const_structrp png_ptr,
    png_inforp info_ptr, png_const_unknown_chunkp unknowns, int num_unknowns)
{
{
   png_unknown_chunkp np;
   if (png_ptr == NULL || info_ptr == NULL || num_unknowns <= 0 ||
       unknowns == NULL)
      return;
#  if !defined(PNG_READ_UNKNOWN_CHUNKS_SUPPORTED) && \
      defined(PNG_READ_SUPPORTED)
      if ((png_ptr->mode & PNG_IS_READ_STRUCT) != 0)
      {
         png_chunk_report(png_ptr, "no unknown chunk support on read",
          PNG_CHUNK_WRITE_ERROR);
         return;
      }
#  endif
#  if !defined(PNG_WRITE_UNKNOWN_CHUNKS_SUPPORTED) && \
      defined(PNG_WRITE_SUPPORTED)
      if ((png_ptr->mode & PNG_IS_READ_STRUCT) == 0)
      {
         png_chunk_report(png_ptr, "no unknown chunk support on write",
         PNG_CHUNK_WRITE_ERROR);
         return;
      }
#  endif
   np = png_voidcast(png_unknown_chunkp, png_realloc_array(png_ptr,
       info_ptr->unknown_chunks, info_ptr->unknown_chunks_num, num_unknowns,
       sizeof *np));
   if (np == NULL)
   {
      png_chunk_report(png_ptr, "too many unknown chunks",
          PNG_CHUNK_WRITE_ERROR);
      return;
   }
   png_free(png_ptr, info_ptr->unknown_chunks);
   info_ptr->unknown_chunks = np; /* safe because it is initialized */
   info_ptr->free_me |= PNG_FREE_UNKN;
   np += info_ptr->unknown_chunks_num;
   /* Increment unknown_chunks_num each time round the loop to protect the
    * just-allocated chunk data.
    */
   for (; num_unknowns > 0; --num_unknowns, ++unknowns)
   {
      memcpy(np->name, unknowns->name, (sizeof np->name));
      np->name[(sizeof np->name)-1] = '\0';
      np->location = check_location(png_ptr, unknowns->location);
      if (unknowns->size == 0)
      {
         np->data = NULL;
         np->size = 0;
      }
      else
      {
         np->data = png_voidcast(png_bytep,
             png_malloc_base(png_ptr, unknowns->size));
         if (np->data == NULL)
         {
            png_chunk_report(png_ptr, "unknown chunk: out of memory",
                PNG_CHUNK_WRITE_ERROR);
            /* But just skip storing the unknown chunk */
            continue;
         }
         memcpy(np->data, unknowns->data, unknowns->size);
         np->size = unknowns->size;
      }
      /* These increments are skipped on out-of-memory for the data - the
       * unknown chunk entry gets overwritten if the png_chunk_report returns.
       * This is correct in the read case (the chunk is just dropped.)
       */
      ++np;
      ++(info_ptr->unknown_chunks_num);
   }
}
}

// -------------------- Lightweight Unit Test Framework --------------------

static int g_total_tests = 0;
static int g_failed_tests = 0;

// Simple assertion helpers (non-terminating)
#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        fprintf(stderr, "EXPECT_TRUE failed: %s at %s:%d\n", #cond, __FILE__, __LINE__); \
        ++g_failed_tests; \
    } \
} while (0)

#define EXPECT_EQ_INT(a, b) do { \
    if ((a) != (b)) { \
        fprintf(stderr, "EXPECT_EQ_INT failed: %d != %d (%s vs %s) at %s:%d\n", (int)(a), (int)(b), #a, #b, __FILE__, __LINE__); \
        ++g_failed_tests; \
    } \
} while (0)

#define EXPECT_STRN_EQ(a, b, n) do { \
    if (std::memcmp((a), (b), (n)) != 0) { \
        fprintf(stderr, "EXPECT_STRN_EQ failed: mismatch in first %d bytes at %s:%d\n", (int)(n), __FILE__, __LINE__); \
        ++g_failed_tests; \
    } \
} while (0)

#define EXPECT_PTR_EQ(a, b) do { \
    if ((a) != (b)) { \
        fprintf(stderr, "EXPECT_PTR_EQ failed: %p != %p at %s:%d\n", (void*)(a), (void*)(b), __FILE__, __LINE__); \
        ++g_failed_tests; \
    } \
} while (0)

// Helper to reset global state between tests
static void reset_globals_for_test()
{
    g_fail_alloc = false;
    g_chunk_report_called = false;
}

// Helper to cleanup allocated information chunks
static void cleanup_info(png_inforp info_ptr)
{
    if (info_ptr == NULL) return;
    if (info_ptr->unknown_chunks != NULL) {
        for (unsigned int i = 0; i < info_ptr->unknown_chunks_num; ++i) {
            if (info_ptr->unknown_chunks[i].data != NULL) {
                std::free(info_ptr->unknown_chunks[i].data);
            }
        }
        std::free(info_ptr->unknown_chunks);
        info_ptr->unknown_chunks = NULL;
    }
    info_ptr->unknown_chunks_num = 0;
    info_ptr->free_me = 0;
}

// Test 1: Basic allocation and copying of two unknown chunks
static void test_basic_allocation()
{
    // Description: ensure two unknown chunks are allocated, copied, and visible in info_ptr.
    reset_globals_for_test();

    // Prepare fake handles
    png_struct png_ptr_instance = {0};
    png_structrp png_ptr = &png_ptr_instance;

    png_info info;
    info.unknown_chunks = NULL;
    info.unknown_chunks_num = 0;
    info.free_me = 0;

    // Prepare two unknown chunks to insert
    unsigned char d0[4] = {0x11, 0x22, 0x33, 0x44};
    unsigned char d1[3] = {0xAA, 0xBB, 0xCC};

    png_unknown_chunk unk0;
    std::memset(unk0.name, 0, sizeof(unk0.name));
    std::memcpy(unk0.name, "ABCD", 4);
    unk0.location = 2;
    unk0.size = 4;
    unk0.data = d0;

    png_unknown_chunk unk1;
    std::memset(unk1.name, 0, sizeof(unk1.name));
    std::memcpy(unk1.name, "EFGH", 4);
    unk1.location = 5;
    unk1.size = 3;
    unk1.data = d1;

    png_unknown_chunk unknowns_arr[2] = {unk0, unk1};

    // Call function
    png_set_unknown_chunks(png_ptr, &info, unknowns_arr, 2);

    // Assertions
    EXPECT_EQ_INT(info.unknown_chunks_num, 2);
    EXPECT_TRUE(info.unknown_chunks != NULL);

    // Validate first copied entry
    EXPECT_STRN_EQ(info.unknown_chunks[0].name, "ABCD", 4);
    EXPECT_EQ_INT(info.unknown_chunks[0].location, 2);
    EXPECT_TRUE(info.unknown_chunks[0].data != NULL);
    EXPECT_EQ_INT(info.unknown_chunks[0].size, 4);
    EXPECT_STRN_EQ(info.unknown_chunks[0].data, d0, 4);

    // Validate second copied entry
    EXPECT_STRN_EQ(info.unknown_chunks[1].name, "EFGH", 4);
    EXPECT_EQ_INT(info.unknown_chunks[1].location, 5);
    EXPECT_TRUE(info.unknown_chunks[1].data != NULL);
    EXPECT_EQ_INT(info.unknown_chunks[1].size, 3);
    EXPECT_STRN_EQ(info.unknown_chunks[1].data, d1, 3);

    // Cleanup
    cleanup_info(&info);
}

// Test 2: Unknown chunk with zero-size data should have NULL data pointer
static void test_zero_size_unknown()
{
    reset_globals_for_test();

    png_struct png_ptr_instance = {0};
    png_structrp png_ptr = &png_ptr_instance;

    png_info info;
    info.unknown_chunks = NULL;
    info.unknown_chunks_num = 0;
    info.free_me = 0;

    png_unknown_chunk unk0;
    std::memset(unk0.name, 0, sizeof(unk0.name));
    std::memcpy(unk0.name, "ZERO", 4);
    unk0.location = 0;
    unk0.size = 0;
    unk0.data = NULL; // no data

    png_unknown_chunk unknowns_arr[1] = {unk0};

    png_set_unknown_chunks(png_ptr, &info, unknowns_arr, 1);

    // Assertions
    EXPECT_EQ_INT(info.unknown_chunks_num, 1);
    EXPECT_TRUE(info.unknown_chunks != NULL);
    EXPECT_STRN_EQ(info.unknown_chunks[0].name, "ZERO", 4);
    EXPECT_EQ_INT(info.unknown_chunks[0].size, 0);
    EXPECT_TRUE(info.unknown_chunks[0].data == NULL);

    // Cleanup
    cleanup_info(&info);
}

// Test 3: Multiple calls accumulate unknown chunks
static void test_multiple_calls_accumulate()
{
    reset_globals_for_test();

    png_struct png_ptr_instance = {0};
    png_structrp png_ptr = &png_ptr_instance;

    png_info info;
    info.unknown_chunks = NULL;
    info.unknown_chunks_num = 0;
    info.free_me = 0;

    // First batch: 1 chunk
    unsigned char a[2] = {1,2};
    png_unknown_chunk k0;
    std::memset(k0.name, 0, sizeof(k0.name));
    std::memcpy(k0.name, "AA  ", 3); // intentionally short; test padding
    k0.location = 1;
    k0.size = 2;
    k0.data = a;

    png_unknown_chunk arr1[1] = {k0};
    png_set_unknown_chunks(png_ptr, &info, arr1, 1);

    // Second batch: 1 more chunk
    unsigned char b[1] = {0xFF};
    png_unknown_chunk k1;
    std::memset(k1.name, 0, sizeof(k1.name));
    std::memcpy(k1.name, "BB  ", 3);
    k1.location = 3;
    k1.size = 1;
    k1.data = b;

    png_unknown_chunk arr2[1] = {k1};
    png_set_unknown_chunks(png_ptr, &info, arr2, 1);

    // Assertions: total now 2
    EXPECT_EQ_INT(info.unknown_chunks_num, 2);
    EXPECT_TRUE(info.unknown_chunks != NULL);
    // First entry check
    EXPECT_STRN_EQ(info.unknown_chunks[0].name, "AA  ", 3);
    // Second entry check
    EXPECT_STRN_EQ(info.unknown_chunks[1].name, "BB  ", 3);

    // Cleanup
    cleanup_info(&info);
}

// Test 4: Simulated out-of-memory during allocation path should not modify info
static void test_allocation_failure_path()
{
    reset_globals_for_test();

    png_struct png_ptr_instance = {0};
    png_structrp png_ptr = &png_ptr_instance;

    png_info info;
    info.unknown_chunks = NULL;
    info.unknown_chunks_num = 0;
    info.free_me = 0;

    // Prepare one unknown chunk
    unsigned char d0[2] = {0x01, 0x02};
    png_unknown_chunk unk0;
    std::memset(unk0.name, 0, sizeof(unk0.name));
    std::memcpy(unk0.name, "F1  ", 3);
    unk0.location = 0;
    unk0.size = 2;
    unk0.data = d0;

    png_unknown_chunk unknowns_arr[1] = {unk0};

    // Simulate allocation failure
    g_fail_alloc = true;

    png_set_unknown_chunks(png_ptr, &info, unknowns_arr, 1);

    // Should not modify info_ptr on failure
    EXPECT_EQ_INT(info.unknown_chunks_num, 0);
    EXPECT_TRUE(info.unknown_chunks == NULL);
    EXPECT_TRUE(g_chunk_report_called); // chunk_report called on failure

    // Cleanup (no allocations performed, nothing to free)
}

// -------------------- Test Runner --------------------

int main()
{
    // Run each test and print a brief header
    fprintf(stdout, "Running tests for png_set_unknown_chunks (self-contained mock)\n");

    g_total_tests = 0;
    g_failed_tests = 0;

    // Test 1
    fprintf(stdout, "Test 1: Basic allocation and copying of two unknown chunks\n");
    ++g_total_tests;
    test_basic_allocation();

    // Test 2
    fprintf(stdout, "Test 2: Zero-size unknown chunk handling\n");
    ++g_total_tests;
    test_zero_size_unknown();

    // Test 3
    fprintf(stdout, "Test 3: Accumulating unknown chunks across multiple calls\n");
    ++g_total_tests;
    test_multiple_calls_accumulate();

    // Test 4
    fprintf(stdout, "Test 4: Allocation failure path handling\n");
    ++g_total_tests;
    test_allocation_failure_path();

    // Summary
    if (g_failed_tests == 0) {
        fprintf(stdout, "ALL TESTS PASSED (%d/%d)\n", g_total_tests - g_failed_tests, g_total_tests);
        return 0;
    } else {
        fprintf(stderr, "SOME TESTS FAILED: %d out of %d\n", g_failed_tests, g_total_tests);
        return 1;
    }
}