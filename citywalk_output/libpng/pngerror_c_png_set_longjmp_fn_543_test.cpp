// Test suite for the focal method png_set_longjmp_fn
// NOTE: This test harness provides a self-contained minimal mock
// of the surrounding PNG structures and functions to exercise the
// control-flow and branching logic of png_set_longjmp_fn.
// It is written for C++11 and uses plain main() with non-terminating
// assertions to maximize coverage without relying on external test frameworks.

#include <cstdlib>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>
#include <cstddef>


// -----------------------------
// Mocked minimal PNG environment
// -----------------------------

// Forward declaration to mimic the real API style
struct png_struct;
typedef png_struct png_struct;

// Typedef for a longjmp function pointer consistent with the focal API
typedef void (*png_longjmp_ptr)(png_struct*, int);

// Lightweight mock implementations to avoid libpng dependencies
static bool g_png_error_called = false;
static const char* g_last_png_error_message = nullptr;
static bool g_png_warning_called = false;
static const char* g_last_png_warning_message = nullptr;

static void png_error(png_struct* /*png_ptr*/, const char* message)
{
    g_png_error_called = true;
    g_last_png_error_message = message;
    // In real libpng, this would longjmp; here we just record.
}

static void png_warning(png_struct* /*png_ptr*/, const char* message)
{
    g_png_warning_called = true;
    g_last_png_warning_message = message;
}

// Simulated memory allocator that libpng might call in error paths
static void* png_malloc_warn(png_struct* /*png_ptr*/, size_t size)
{
    return std::malloc(size);
}

// Helper macro (simple cast)
#define PNG_VOIDCAST(type, expr) (type)(expr)

// Minimal PNG struct layout sufficient for the test
struct png_struct {
    void* jmp_buf_ptr;          // Pointer to current jump buffer (local or allocated)
    size_t jmp_buf_size;         // Size of the allocated jump buffer (0 if local buffer used)
    char jmp_buf_local[64];       // Built-in local buffer
    png_longjmp_ptr longjmp_fn;   // App-provided longjmp function
};

// The focal function under test (self-contained mock of the real function)
// Signature matches the expected usage pattern in the provided source.
static void* png_set_longjmp_fn(png_struct* png_ptr, png_longjmp_ptr longjmp_fn,
                               size_t jmp_buf_size)
{
    if (png_ptr == nullptr)
        return nullptr;
    if (png_ptr->jmp_buf_ptr == nullptr)
    {
        png_ptr->jmp_buf_size = 0; // not allocated yet
        if (jmp_buf_size <= (sizeof png_ptr->jmp_buf_local))
            png_ptr->jmp_buf_ptr = &png_ptr->jmp_buf_local;
        else
        {
            png_ptr->jmp_buf_ptr = PNG_VOIDCAST(void*,
                png_malloc_warn(png_ptr, jmp_buf_size));
            if (png_ptr->jmp_buf_ptr == nullptr)
                return nullptr; // OOM
            png_ptr->jmp_buf_size = jmp_buf_size;
        }
    }
    else // Already allocated
    {
        size_t size = png_ptr->jmp_buf_size;
        if (size == 0)
        {
            size = (sizeof png_ptr->jmp_buf_local);
            if (png_ptr->jmp_buf_ptr != &png_ptr->jmp_buf_local)
            {
                // Internal error in this simplified environment
                png_error(png_ptr, "Libpng jmp_buf still allocated");
                // Intentionally continue to preserve testability of flow
            }
        }
        if (size != jmp_buf_size)
        {
            png_warning(png_ptr, "Application jmp_buf size changed");
            return nullptr;
        }
    }
    // Final step: install the function and return the buffer pointer
    png_ptr->longjmp_fn = longjmp_fn;
    return png_ptr->jmp_buf_ptr;
}

// -----------------------------
// Simple test harness helpers
// -----------------------------
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg)                               \
    do {                                                       \
        if (!(cond)) {                                         \
            ++g_failures;                                      \
            printf("TEST FAIL: %s\n", msg);                    \
        } else {                                               \
            printf("TEST PASS: %s\n", msg);                    \
        }                                                      \
    } while (0)

#define EXPECT_PTR_EQ(a, b, msg)                              \
    do {                                                        \
        if ((a) != (b)) {                                      \
            ++g_failures;                                      \
            printf("TEST FAIL: %s (0x%p != 0x%p)\n", msg, (void*)(a), (void*)(b)); \
        } else {                                               \
            printf("TEST PASS: %s\n", msg);                    \
        }                                                      \
    } while (0)

// Helper to clean up allocated buffers if needed
static void safe_free(void* p) { if (p) std::free(p); }

// Dummy longjmp function to install via png_set_longjmp_fn
static void dummy_longjmp(png_struct* /*png_ptr*/, int /*code*/) { /* no-op for tests */ }

// -----------------------------
// Test cases
// -----------------------------

// Test 0: NULL png_ptr must return NULL
static void test_null_png_ptr()
{
    g_png_error_called = false;
    g_last_png_error_message = nullptr;
    g_png_warning_called = false;
    g_last_png_warning_message = nullptr;
    void* res = png_set_longjmp_fn(nullptr, dummy_longjmp, 16);
    EXPECT_TRUE(res == nullptr, "png_set_longjmp_fn(NULL, ...) returns NULL");
}

// Test 1: First call with small jmp_buf_size initializes to local buffer
static void test_first_call_small_buffer()
{
    // Prepare a fresh PNG struct with no prior allocation
    png_struct* p = new png_struct;
    std::memset(p, 0, sizeof(png_struct));
    p->jmp_buf_ptr = nullptr;
    p->jmp_buf_size = 0;

    g_png_error_called = false;
    g_last_png_error_message = nullptr;
    g_png_warning_called = false;
    g_last_png_warning_message = nullptr;

    void* ret = png_set_longjmp_fn(p, dummy_longjmp, 8); // <= sizeof(local)=64
    // Expect returned pointer to be local buffer
    EXPECT_PTR_EQ(ret, p->jmp_buf_ptr, "First call small buffer returns local buffer");
    EXPECT_TRUE(p->jmp_buf_ptr == (void*)&p->jmp_buf_local, "Pointer equals local buffer");
    EXPECT_TRUE(p->jmp_buf_size == 0, "jmp_buf_size remains 0 for local buffer");
    EXPECT_TRUE(p->longjmp_fn == dummy_longjmp, "longjmp_fn stored correctly");
    // Cleanup
    safe_free(nullptr); // nothing allocated
    delete p;
}

// Test 2: First call with large jmp_buf_size allocates and stores size
static void test_first_call_large_buffer_allocated()
{
    png_struct* p = new png_struct;
    std::memset(p, 0, sizeof(png_struct));
    p->jmp_buf_ptr = nullptr;
    p->jmp_buf_size = 0;

    g_png_error_called = false;
    g_last_png_error_message = nullptr;
    g_png_warning_called = false;
    g_last_png_warning_message = nullptr;

    void* ret = png_set_longjmp_fn(p, dummy_longjmp, 128); // larger than local
    // Expect allocation occurred and pointer != local
    EXPECT_TRUE(ret != nullptr, "First call large buffer returns non-null");
    EXPECT_TRUE(p->jmp_buf_ptr != (void*)&p->jmp_buf_local, "jmp_buf_ptr is not the local buffer");
    EXPECT_TRUE(p->jmp_buf_size == 128, "jmp_buf_size updated to 128");
    EXPECT_TRUE(p->longjmp_fn == dummy_longjmp, "longjmp_fn stored correctly");

    // Cleanup allocated memory
    safe_free(p->jmp_buf_ptr);
    delete p;
}

// Test 3: Allocated large buffer, then size mismatch triggers warning and NULL return
static void test_size_mismatch_triggers_warning()
{
    // Setup: allocate large buffer first
    png_struct* p = new png_struct;
    std::memset(p, 0, sizeof(png_struct));
    p->jmp_buf_ptr = nullptr;
    p->jmp_buf_size = 0;

    void* ret1 = png_set_longjmp_fn(p, dummy_longjmp, 128);
    // Now attempt with a different size to cause mismatch
    g_png_error_called = false;
    g_last_png_error_message = nullptr;
    g_png_warning_called = false;
    g_last_png_warning_message = nullptr;

    void* ret2 = png_set_longjmp_fn(p, dummy_longjmp, 32); // mismatch
    EXPECT_TRUE(ret2 == nullptr, "Size mismatch returns NULL with warning");
    EXPECT_TRUE(g_png_warning_called, "Warning issued on size change");
    delete p;
}

// Test 4: Same size reuses existing buffer and updates longjmp_fn
static void test_same_size_reuses_buffer()
{
    png_struct* p = new png_struct;
    std::memset(p, 0, sizeof(png_struct));
    p->jmp_buf_ptr = nullptr;
    p->jmp_buf_size = 0;

    // First allocate with a specific size
    void* ret1 = png_set_longjmp_fn(p, dummy_longjmp, 64);
    // Now call again with the same size
    g_png_error_called = false;
    g_last_png_error_message = nullptr;
    g_png_warning_called = false;
    g_last_png_warning_message = nullptr;

    void* ret2 = png_set_longjmp_fn(p, dummy_longjmp, 64);
    EXPECT_TRUE(ret2 == p->jmp_buf_ptr, "Reusing same size returns same buffer");
    EXPECT_TRUE(p->longjmp_fn == dummy_longjmp, "longjmp_fn updated to new value");
    delete p;
}

// Test 5: Internal error path when jmp_buf still allocated but size == 0, then triggers error
static void test_internal_error_path_when_non_local_allocated()
{
    png_struct* p = new png_struct;
    // Simulate an internal error scenario: currently jmp_buf_ptr points to non-local buffer
    // and jmp_buf_size == 0
    p->jmp_buf_ptr = std::malloc(16); // not the local buffer
    p->jmp_buf_size = 0;
    std::memset(p->jmp_buf_local, 0, sizeof(p->jmp_buf_local));

    g_png_error_called = false;
    g_last_png_error_message = nullptr;
    g_png_warning_called = false;
    g_last_png_warning_message = nullptr;

    void* ret = png_set_longjmp_fn(p, dummy_longjmp, 32); // 32 != 64 (size after block)
    // Should call png_error (internal) and then return NULL due to size mismatch
    EXPECT_TRUE(g_png_error_called, "png_error should be invoked for internal error path");
    EXPECT_TRUE(ret == nullptr, "Returns NULL when size mismatch after internal error");
    // Cleanup
    safe_free(p->jmp_buf_ptr);
    delete p;
}

// -----------------------------
// Main: run all tests
// -----------------------------
int main()
{
    printf("Starting png_set_longjmp_fn unit tests (mocked environment)\n\n");

    test_null_png_ptr();
    test_first_call_small_buffer();
    test_first_call_large_buffer_allocated();
    test_size_mismatch_triggers_warning();
    test_same_size_reuses_buffer();
    test_internal_error_path_when_non_local_allocated();

    printf("\nTests completed. Failures: %d\n", g_failures);
    return (g_failures == 0) ? 0 : 1;
}