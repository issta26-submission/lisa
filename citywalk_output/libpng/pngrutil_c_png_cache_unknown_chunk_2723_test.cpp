// Note: This is a self-contained C++11 test harness that re-implements a minimal
// subset of the PNG library structures and the focal function
// png_cache_unknown_chunk to enable exhaustively testing its behavior
// without depending on the actual libpng sources.
// The tests cover true/false branches, memory allocation paths, and cleanup
// behavior described in the provided focal method.

/*
Tests aim:
- Step through all branches of png_cache_unknown_chunk:
  - Free existing unknown_chunk.data if non-NULL
  - Allocate unknown_chunk.data when length <= limit and length > 0
  - Handle length == 0 gracefully (no allocation)
  - On length > limit or allocation failure, return 0 with benign error
  - On success, read CRC data into allocated memory and return 1
- Validate that:
  - Memory is freed when appropriate (test 1 & test 4)
  - chunk name is copied from chunk_name
  - location is set from mode
  - data contents are written by crc_read when length > 0
  - benign error path is exercised when allocation fails or length > limit
*/

// Include standard headers
#include <cstring>
#include <pngpriv.h>
#include <cstdlib>
#include <cstdio>


// Domain-specific: minimal PNG-like types and helpers (self-contained)

/* Type definitions to mimic libpng style in a compact way */
typedef unsigned char png_bytep;
typedef const unsigned char* png_const_bytep;
typedef unsigned int png_uint_32;
typedef size_t png_alloc_size_t;
typedef unsigned char png_byte;
typedef void* png_voidp;
typedef struct png_struct png_struct;
typedef png_struct* png_structrp;
typedef const png_struct* png_const_structrp;

/* Forward declaration of the focal function to be tested (implemented below) */
png_uint_32 png_cache_unknown_chunk(png_structrp png_ptr, png_uint_32 length);

// Minimal structure layout matching usage in the focal function
struct png_struct {
    // Simulated "unknown_chunk" storage
    struct {
        void* data;
        char name[5];
        size_t size;
        png_byte location;
    } unknown_chunk;

    // chunk_name used to fill unknown_chunk.name
    char chunk_name[5];

    // mode used for location
    unsigned int mode;

    // memory limit for chunks
    size_t chunk_max;
};

// Helper: mimic libpng macro to copy 4-char chunk name into a null-terminated string
#define PNG_CSTRING_FROM_CHUNK(dest, src) { \
    memcpy((dest), (src), 4); \
    (dest)[4] = '\0'; \
}

// Function prototypes for the minimal test harness
static size_t g_free_calls = 0;
static int g_malloc_fail_next = 0;

// Custom "mock" implementations of libpng helpers used by the focal method

static void png_free(png_structrp /*png_ptr*/, void* ptr) {
    if (ptr) {
        free(ptr);
        ++g_free_calls;
    }
}

// Simulate a warning-limited allocator (malloc with possible failure)
static void* png_malloc_warn(png_structrp /*png_ptr*/, png_alloc_size_t size) {
    if (g_malloc_fail_next > 0) {
        --g_malloc_fail_next;
        return NULL;
    }
    if (size == 0) return NULL;
    return malloc((size_t)size);
}

// Simulate casting helper
#define png_voidcast(type, expr) ((type)(expr))

// Simulate CRC read/write behavior for tests
static void png_crc_read(png_structrp /*png_ptr*/, png_bytep buf, png_uint_32 length) {
    // Fill with a deterministic pattern for verification
    for (png_uint_32 i = 0; i < length; ++i) {
        buf[i] = static_cast<unsigned char>(i + 1); // 1,2,3,...
    }
}
static void png_crc_finish(png_structrp /*png_ptr*/, png_uint_32 /*skip*/) {
    // No-op for test
}
static void png_chunk_benign_error(png_structrp /*png_ptr*/, const char* /*msg*/) {
    // No-op for test
}

// Accessor: mimic png_chunk_max as used by the focal function
static png_alloc_size_t png_chunk_max(png_structrp png_ptr) {
    return static_cast<png_alloc_size_t>(png_ptr->chunk_max);
}

// Minimal test assertion framework (non-terminating)
static int g_test_failed = 0;
static void test_assert(bool cond, const char* msg) {
    if (!cond) {
        fprintf(stderr, "TEST FAIL: %s\n", msg);
        ++g_test_failed;
    }
}
#define ASSERT_TRUE(x, msg) test_assert((x), (msg))

// Implement the focal method under test (reproduced and adapted for test)
png_uint_32 png_cache_unknown_chunk(png_structrp png_ptr, png_uint_32 length)
{
{
   const png_alloc_size_t limit = png_chunk_max(png_ptr);
   if (png_ptr->unknown_chunk.data != NULL)
   {
      png_free(png_ptr, png_ptr->unknown_chunk.data);
      png_ptr->unknown_chunk.data = NULL;
   }
   if (length <= limit)
   {
      PNG_CSTRING_FROM_CHUNK(png_ptr->unknown_chunk.name, png_ptr->chunk_name);
      /* The following is safe because of the PNG_SIZE_MAX init above */
      png_ptr->unknown_chunk.size = (size_t)length/*SAFE*/;
      /* 'mode' is a flag array, only the bottom four bits matter here */
      png_ptr->unknown_chunk.location = (png_byte)png_ptr->mode/*SAFE*/;
      if (length == 0)
         png_ptr->unknown_chunk.data = NULL;
      else
      {
         /* Do a 'warn' here - it is handled below. */
         png_ptr->unknown_chunk.data = png_voidcast(png_bytep,
             png_malloc_warn(png_ptr, length));
      }
   }
   if (png_ptr->unknown_chunk.data == NULL && length > 0)
   {
      /* This is benign because we clean up correctly */
      png_crc_finish(png_ptr, length);
      png_chunk_benign_error(png_ptr, "unknown chunk exceeds memory limits");
      return 0;
   }
   else
   {
      if (length > 0)
         png_crc_read(png_ptr, png_ptr->unknown_chunk.data, length);
      png_crc_finish(png_ptr, 0);
      return 1;
   }
}
}

// ---------------- Test Suite ----------------

namespace test_png_cache_unknown_chunk {

// Helper to initialize a fresh png_struct for tests
static void init_png_struct(png_struct* p) {
    if (!p) return;
    memset(p, 0, sizeof(png_struct));
    // Provide a sane default chunk name (4 chars)
    memcpy(p->chunk_name, "ABCD", 4);
    p->chunk_name[4] = '\0';
    p->mode = 0;
    p->chunk_max = 10; // default limit
}

// Test 1: Free existing data and handle length == 0 (no allocation), expect 1 and data == NULL
static void test_free_and_zero_length() {
    init_png_struct(reinterpret_cast<png_struct*>(nullptr)); // no-op, placeholder for clarity
    png_struct* ptr = new png_struct;
    init_png_struct(ptr);

    // Pre-existing allocated block to ensure free() is called
    void* old = malloc(16);
    ptr->unknown_chunk.data = old;
    g_free_calls = 0;
    g_malloc_fail_next = 0;

    // Execute
    png_uint_32 ret = png_cache_unknown_chunk(ptr, 0);

    // Verify: freed old block, data NULL, return 1
    ASSERT_TRUE(ret == 1, "png_cache_unknown_chunk should return 1 for length 0");
    ASSERT_TRUE(ptr->unknown_chunk.data == NULL, "unknown_chunk.data should be NULL after length 0");
    ASSERT_TRUE(g_free_calls >= 1, "png_free should be called to free existing data");
    // Confirm chunk_name is preserved/copied to unknown_chunk.name
    char expect[5] = "ABCD";
    ASSERT_TRUE(strncmp(ptr->unknown_chunk.name, expect, 5) == 0, "unknown_chunk.name should be copied from chunk_name");
    delete ptr;
}

// Test 2: Allocate and fill data when length > 0 and length <= limit; verify contents
static void test_allocate_and_crc_read() {
    init_png_struct(reinterpret_cast<png_struct*>(nullptr));
    png_struct* ptr = new png_struct;
    init_png_struct(ptr);
    // Ensure no previous data
    ptr->unknown_chunk.data = NULL;
    g_free_calls = 0;
    g_malloc_fail_next = 0;

    // Set length within limit
    const png_uint_32 length = 4;
    // Keep mode non-zero to test location copying
    ptr->mode = 0x05;
    // chunk_name already "ABCD"
    png_uint_32 ret = png_cache_unknown_chunk(ptr, length);

    // Verify: success return
    ASSERT_TRUE(ret == 1, "png_cache_unknown_chunk should return 1 on successful allocation/read");
    // Verify memory allocated
    ASSERT_TRUE(ptr->unknown_chunk.data != NULL, "unknown_chunk.data should be allocated when length > 0 and within limit");
    // Verify data contents were written by crc_read
    unsigned char* data = reinterpret_cast<unsigned char*>(ptr->unknown_chunk.data);
    for (png_uint_32 i = 0; i < length; ++i) {
        unsigned char expected = static_cast<unsigned char>(i + 1);
        ASSERT_TRUE(data[i] == expected, "unknown_chunk.data should be filled by crc_read with pattern");
    }
    // Verify chunk name copied and location set
    char expect[5] = "ABCD";
    ASSERT_TRUE(strncmp(ptr->unknown_chunk.name, expect, 5) == 0, "unknown_chunk.name should be copied from chunk_name");
    ASSERT_TRUE(ptr->unknown_chunk.size == (size_t)length, "unknown_chunk.size should match length");
    ASSERT_TRUE(ptr->unknown_chunk.location == (png_byte)ptr->mode, "unknown_chunk.location should reflect mode");
    delete ptr;
}

// Test 3: Exceed memory limit -> should finish with benign error and return 0
static void test_exceed_limit_returns_zero() {
    init_png_struct(reinterpret_cast<png_struct*>(nullptr));
    png_struct* ptr = new png_struct;
    init_png_struct(ptr);
    ptr->chunk_max = 10; // limit
    // No pre-allocated data
    ptr->unknown_chunk.data = NULL;
    g_malloc_fail_next = 0;

    // length exceeds limit
    const png_uint_32 length = 20;
    png_uint_32 ret = png_cache_unknown_chunk(ptr, length);

    // Verify: returns 0 and no allocation
    ASSERT_TRUE(ret == 0, "png_cache_unknown_chunk should return 0 when length > limit and no memory available");
    ASSERT_TRUE(ptr->unknown_chunk.data == NULL, "unknown_chunk.data should remain NULL when exceed limit");
    delete ptr;
}

// Test 4: Free old data and reallocate new data when length <= limit
static void test_free_old_and_realloc() {
    init_png_struct(reinterpret_cast<png_struct*>(nullptr));
    png_struct* ptr = new png_struct;
    init_png_struct(ptr);

    // Pre-existing data to be freed
    void* old = malloc(8);
    ptr->unknown_chunk.data = old;
    g_free_calls = 0;
    g_malloc_fail_next = 0;

    // New allocation within limit
    const png_uint_32 length = 6;
    ptr->mode = 0x0A;

    png_uint_32 ret = png_cache_unknown_chunk(ptr, length);

    // Verify: freed old, allocated new, content filled, return 1
    ASSERT_TRUE(ret == 1, "png_cache_unknown_chunk should return 1 after reallocation within limit");
    ASSERT_TRUE(ptr->unknown_chunk.data != NULL, "unknown_chunk.data should be allocated");
    ASSERT_TRUE(g_free_calls >= 1, "Old data should be freed before new allocation");
    unsigned char* data = reinterpret_cast<unsigned char*>(ptr->unknown_chunk.data);
    for (png_uint_32 i = 0; i < length; ++i) {
        unsigned char expected = static_cast<unsigned char>(i + 1);
        ASSERT_TRUE(data[i] == expected, "CRC read should fill new memory with pattern");
    }
    delete ptr;
}

// Test 5: Allocation failure (simulate malloc_warn returning NULL)
static void test_allocation_failure() {
    init_png_struct(reinterpret_cast<png_struct*>(nullptr));
    png_struct* ptr = new png_struct;
    init_png_struct(ptr);
    // Force malloc to fail on next allocation
    g_malloc_fail_next = 1;
    const png_uint_32 length = 4;

    png_uint_32 ret = png_cache_unknown_chunk(ptr, length);

    // Verify: returns 0 and no data allocated
    ASSERT_TRUE(ret == 0, "png_cache_unknown_chunk should return 0 when memory allocation fails");
    ASSERT_TRUE(ptr->unknown_chunk.data == NULL, "unknown_chunk.data should be NULL when allocation fails");
    delete ptr;
}

// Runner to execute all tests
static void run_all_tests() {
    printf("Running test suite for png_cache_unknown_chunk (self-contained test harness)\n");
    test_free_and_zero_length();
    test_allocate_and_crc_read();
    test_exceed_limit_returns_zero();
    test_free_old_and_realloc();
    test_allocation_failure();

    if (g_test_failed == 0) {
        printf("ALL TESTS PASSED\n");
    } else {
        printf("SOME TESTS FAILED: %d\n", g_test_failed);
    }
}

// Entry point
} // namespace test_png_cache_unknown_chunk

int main() {
    // Run tests
    test_png_cache_unknown_chunk::run_all_tests();
    return (test_png_cache_unknown_chunk::g_test_failed == 0) ? 0 : 1;
}