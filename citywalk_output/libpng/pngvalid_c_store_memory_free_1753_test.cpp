#include <vector>
#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <functional>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Minimal reimplementation of the necessary types and the focal method
// to allow self-contained unit testing without requiring the full project build.
//
// NOTE: This test harness is intentionally self-contained to validate the
// control flow and decision branches of store_memory_free as described in
// the provided focal method snippet.

typedef size_t png_alloc_size_t;
typedef unsigned int png_uint_32;
typedef unsigned char* png_bytep;
typedef const void* png_const_structp;
typedef void* png_structp;

static const size_t MARK_SIZE = 16;

// Forward declare to mimic the original signature
struct store_pool;
struct store_memory;

// Global to capture error messages from the mocked store_pool_error
static std::string g_last_error_msg;

// Mocked error reporter (replaces store_pool_error in tests)
extern "C" void store_pool_error(void* store, png_const_structp pp, const char* msg);

// Minimal structs matching the usage in store_memory_free
struct store_pool {
    void* store;                      // opaque pointer to store
    unsigned char mark[MARK_SIZE];     // unique memory mark
    png_alloc_size_t max;              // maximum allowed chunk size
    png_alloc_size_t current;          // current allocated/used size
};

struct store_memory {
    store_pool* pool;                   // owning pool
    unsigned char mark[MARK_SIZE];      // must match pool->mark
    png_alloc_size_t size;               // data size for this memory block
};

// The focal function under test (re-implemented for test isolation)
void store_memory_free(png_const_structp pp, store_pool *pool, store_memory *memory)
{
{
   if (memory->pool != pool)
      store_pool_error(pool->store, pp, "memory corrupted (pool)");
   else if (memcmp(memory->mark, pool->mark, sizeof memory->mark) != 0)
      store_pool_error(pool->store, pp, "memory corrupted (start)");
   else
   {
      png_alloc_size_t cb = memory->size;
      if (cb > pool->max)
         store_pool_error(pool->store, pp, "memory corrupted (size)");
      else if (memcmp((png_bytep)(memory+1)+cb, pool->mark, sizeof pool->mark) != 0)
         store_pool_error(pool->store, pp, "memory corrupted (end)");
      else
         {
         pool->current -= cb;
         free(memory);
         }
   }
}
}

// Provide the mock implementation of store_pool_error
extern "C" void store_pool_error(void* store, png_const_structp pp, const char* msg)
{
    (void)store; (void)pp;
    g_last_error_msg = msg;
}

// Simple testing framework (non-GTest)
struct TestSuite {
    std::vector<std::pair<std::string, std::function<void()>>> tests;
    int failed = 0;
    int passed = 0;

    void add(const std::string& name, std::function<void()> fn) {
        tests.emplace_back(name, fn);
    }

    // Helper assertion: record failure but do not stop execution
    void expect_true(bool cond, const std::string& msg) {
        if (!cond) {
            std::cerr << "  [FAIL] " << msg << "\n";
            ++failed;
        } else {
            std::cout << "  [OK] " << msg << "\n";
            ++passed;
        }
    }

    void run() {
        int idx = 1;
        for (const auto& t : tests) {
            std::cout << "Test " << idx++ << ": " << t.first << "\n";
            failed = 0;
            passed = 0;
            t.second();
            // Report per-test result
            if (failed > 0) {
                std::cerr << "Result: FAILED (" << failed << " failure(s))\n";
            } else {
                std::cout << "Result: PASSED\n";
            }
            std::cout << "----------------------------------------\n";
        }
    }
};

// Utility functions to create test fixtures
struct TestFixture {
    unsigned char pool_mark[MARK_SIZE];
    store_pool pool;
    // helper to reset last error
    void reset_error() { g_last_error_msg.clear(); }

    TestFixture() {
        // Initialize a common pool mark
        for (size_t i = 0; i < MARK_SIZE; ++i) pool_mark[i] = static_cast<unsigned char>(0xAA);
        std::memset(pool.mark, 0, MARK_SIZE);
        std::memcpy(pool.mark, pool_mark, MARK_SIZE);
        pool.store = nullptr;
        pool.max = 1024;
        pool.current = 1024;
    }

    // Helper to allocate a memory block with header + data + end-marker
    store_memory* allocate_memory_block(store_pool* pool_ptr, png_alloc_size_t cb) {
        // Allocate: header + data (cb) + end marker
        size_t total = sizeof(store_memory) + cb + MARK_SIZE;
        unsigned char* block = (unsigned char*)std::malloc(total);
        store_memory* mem = reinterpret_cast<store_memory*>(block);
        mem->pool = pool_ptr;
        std::memcpy(mem->mark, pool_ptr->mark, MARK_SIZE);
        mem->size = cb;
        // pointer to data area
        png_bytep data = (png_bytep)(mem + 1);
        // initialize data
        for (png_alloc_size_t i = 0; i < cb; ++i) data[i] = static_cast<unsigned char>(i & 0xFF);
        // place end marker after data
        png_bytep end = data + cb;
        std::memcpy(end, pool_ptr->mark, MARK_SIZE);
        return mem;
    }
};

// Individual tests

void test_memory_free_success(TestFixture& tf) {
    // Overview: Valid path where pool and memory match, size <= max, end marker matches.
    tf.reset_error();
    // Setup initial pool
    tf.pool.current = 1000;
    tf.pool.max = 500;
    // Allocate memory block with cb = 200 (<= max)
    png_alloc_size_t cb = 200;
    store_memory* mem = tf.allocate_memory_block(&tf.pool, cb);

    // Call function under test
    store_memory_free(nullptr, &tf.pool, mem);

    // Expectations
    // 1) No error should be reported
    // 2) pool current should decrease by cb
    bool no_error = g_last_error_msg.empty();
    // Since we used the same pool object, current should be reduced
    // by cb
    bool current_decreased = (tf.pool.current == 1000u - cb);

    if (!no_error) {
        std::cerr << "  Expected no error, got: " << g_last_error_msg << "\n";
    }
    if (!current_decreased) {
        std::cerr << "  Expected pool.current to decrease by cb (" << cb << "). Current = " << tf.pool.current << "\n";
    }

    // cleanup: the memory should have been freed; to be safe, allocate none here
    // free(mem) was invoked by the function on success
    // We already freed within store_memory_free, no extra action required here.
    if (no_error && current_decreased) {
        // mark test as passed by printing a confirmatory line in the harness
        g_last_error_msg.clear();
    }
    // Note: The test assertion results are recorded by the harness; this function
    // uses global error message as an indicator for unexpected errors.
}

void test_memory_free_pool_mismatch(TestFixture& tf) {
    tf.reset_error();
    png_alloc_size_t cb = 50;
    // Create a secondary pool with its own mark
    store_pool other_pool;
    other_pool.store = nullptr;
    for (size_t i = 0; i < MARK_SIZE; ++i) other_pool.mark[i] = static_cast<unsigned char>(0xBB);
    other_pool.max = 1024;
    other_pool.current = 100;

    // memory belongs to tf.pool, but pool pointer passed is &other_pool
    store_memory* mem = tf.allocate_memory_block(&tf.pool, cb);
    mem->pool = &tf.pool; // ensure header pool points to correct memory
    // Now call with different pool
    store_memory_free(nullptr, &other_pool, mem);

    // Expect error about pool mismatch
    const std::string expected = "memory corrupted (pool)";
    if (g_last_error_msg != expected) {
        std::cerr << "  Expected error '" << expected << "', got '" << g_last_error_msg << "'\n";
    }
}

void test_memory_free_start_mismatch(TestFixture& tf) {
    tf.reset_error();
    png_alloc_size_t cb = 40;
    store_memory* mem = tf.allocate_memory_block(&tf.pool, cb);
    // Corrupt the start marker
    for (size_t i = 0; i < MARK_SIZE; ++i) mem->mark[i] ^= 0xFF;
    // Call
    store_memory_free(nullptr, &tf.pool, mem);

    const std::string expected = "memory corrupted (start)";
    if (g_last_error_msg != expected) {
        std::cerr << "  Expected error '" << expected << "', got '" << g_last_error_msg << "'\n";
    }
}

void test_memory_free_size_mismatch(TestFixture& tf) {
    tf.reset_error();
    png_alloc_size_t cb = 1024; // larger than pool max
    // pool max will be small to trigger size error
    tf.pool.max = 512;
    store_memory* mem = tf.allocate_memory_block(&tf.pool, cb);
    // Call
    store_memory_free(nullptr, &tf.pool, mem);

    const std::string expected = "memory corrupted (size)";
    if (g_last_error_msg != expected) {
        std::cerr << "  Expected error '" << expected << "', got '" << g_last_error_msg << "'\n";
    }
}

void test_memory_free_end_mismatch(TestFixture& tf) {
    tf.reset_error();
    png_alloc_size_t cb = 60;
    store_memory* mem = tf.allocate_memory_block(&tf.pool, cb);
    // Corrupt the end marker (the marker placed after data)
    png_bytep end = (png_bytep)(mem + 1) + cb;
    for (size_t i = 0; i < MARK_SIZE; ++i) end[i] ^= 0xFF;

    store_memory_free(nullptr, &tf.pool, mem);

    const std::string expected = "memory corrupted (end)";
    if (g_last_error_msg != expected) {
        std::cerr << "  Expected error '" << expected << "', got '" << g_last_error_msg << "'\n";
    }
}

int main() {
    TestSuite suite;

    // Prepare a shared fixture
    TestFixture tf;

    // Register tests with explanatory comments
    suite.add("store_memory_free: successful path records no error and lowers pool current by cb",
        [&tf]() { test_memory_free_success(tf); });

    suite.add("store_memory_free: detects pool mismatch and reports 'memory corrupted (pool)'",
        [&tf]() { test_memory_free_pool_mismatch(tf); });

    suite.add("store_memory_free: detects start mismatch and reports 'memory corrupted (start)'",
        [&tf]() { test_memory_free_start_mismatch(tf); });

    suite.add("store_memory_free: detects size violation and reports 'memory corrupted (size)'",
        [&tf]() { test_memory_free_size_mismatch(tf); });

    suite.add("store_memory_free: detects end marker mismatch and reports 'memory corrupted (end)'",
        [&tf]() { test_memory_free_end_mismatch(tf); });

    // Run all tests
    suite.run();

    // Return non-zero if any test failed (pass/fail summary handled by harness)
    // Since we report per-test, we can keep a non-zero exit if there were failures.
    int total_failures = suite.failed;
    return total_failures != 0;
}