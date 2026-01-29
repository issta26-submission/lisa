// High-quality C++ unit tests for the focal C function: deflateCopy
// This test suite is designed to be compiled with a C/C++ project that provides
// the deflate.h header and the C implementation (deflate.c) as in the prompt.
// The tests avoid Google Test and use a lightweight, non-terminating assertion style.

#include <iostream>
#include <deflate.h>
#include <cstdlib>
#include <cstring>


// Bring in the C declarations (ensure C linkage for C APIs)
extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) \
    do { if (!(cond)) { std::cerr << "EXPECT_TRUE FAILED: " << (msg) << "\n"; ++g_failures; } } while (0)
#define EXPECT_NOT_NULL(ptr, msg) \
    do { if ((ptr) == nullptr) { std::cerr << "EXPECT_NOT_NULL FAILED: " << (msg) << "\n"; ++g_failures; } } while (0)
#define EXPECT_EQ(a, b, msg) \
    do { if (!((a) == (b))) { std::cerr << "EXPECT_EQ FAILED: " << (msg) << " (" << (a) << " != " << (b) << ")\n"; ++g_failures; } } while (0)


// Helper to safely clean up the allocated deflate_state and buffers
static void cleanup_deflate_copy_test(z_stream *src, z_stream *dst)
{
    // Best-effort cleanup; ignore if already freed by deflateEnd
    if (src) {
        // src->state was ss (deflate_state*) allocated by test, freeing its members
        deflate_state *ss = (deflate_state *) src->state;
        if (ss) {
            if (ss->window) free(ss->window);
            if (ss->prev) free(ss->prev);
            if (ss->head) free(ss->head);
            if (ss->pending_buf) free(ss->pending_buf);
            // ss itself allocated later; free it
            free(ss);
        }
        // If the src struct owns any other allocations, they would be freed here if known
        // src itself is a stack/object in tests, so no need to free
    }
    if (dst) {
        // dest->state is ds; deflateEnd will free it
        deflateEnd(dst);
    }
}


// Test 1: Basic, successful deflateCopy path
// This constructs a minimal valid environment for the focal function and asserts
// that the function returns Z_OK and that several important post-conditions hold.
// Notes:
// - We rely on the repository's actual types (z_stream, deflate_state, etc.).
// - We manually allocate and populate the source's internal deflate_state (ss) to a
//   minimal but coherent state so that the copy proceeds through the major code path.
static bool test_deflateCopy_basic_copy()
{
    // Prepare source z_stream with a valid internal state
    z_stream src;
    z_stream dst;
    std::memset(&src, 0, sizeof(src));
    std::memset(&dst, 0, sizeof(dst));

    // Allocate and initialize a minimal deflate_state (ss) for source
    deflate_state *ss = (deflate_state *) malloc(sizeof(deflate_state));
    if (!ss) {
        std::cerr << "Test setup failed: could not allocate ss\n";
        return false;
    }
    // Minimal sizes
    ss->w_size = 4;       // small window size
    ss->hash_size = 4;    // small hash table
    ss->lit_bufsize = 8;  // small lit buffer

    // Allocate buffers that ss expects to own
    ss->window = (Bytef *) malloc(ss->w_size * 2 * sizeof(Byte));
    ss->prev   = (Posf *)  malloc(ss->w_size * sizeof(Pos));
    ss->head   = (Posf *)  malloc(ss->hash_size * sizeof(Pos));
    ss->pending_buf = (uchf *) malloc(ss->lit_bufsize * LIT_BUFS);

    if (!ss->window || !ss->prev || !ss->head || !ss->pending_buf) {
        std::cerr << "Test setup failed: could not allocate ss buffers\n";
        cleanup_deflate_copy_test(&src, &dst);
        return false;
    }

    // Initialize with recognizable data
    for (size_t i = 0; i < ss->w_size * 2; ++i) ss->window[i] = (Bytef)(i + 1);
    for (size_t i = 0; i < ss->w_size; ++i) ss->prev[i] = (Pos)(i + 10);
    for (size_t i = 0; i < ss->hash_size; ++i) ss->head[i] = (Pos)(i + 20);
    for (size_t i = 0; i < ss->lit_bufsize * LIT_BUFS; ++i) ss->pending_buf[i] = (uchf)(i + 30);

    // Point source's state to our ss
    src.state = ss;

    // Ensure the source z_stream is otherwise well-formed
    // (We don't rely on other fields here.)

    // Call the function under test
    int ret = deflateCopy(&dst, &src);

    // Assertions
    EXPECT_EQ(ret, Z_OK, "deflateCopy should return Z_OK on successful copy");

    // dst should now own a deflate_state (ds)
    deflate_state *ds = (deflate_state *) dst.state;
    EXPECT_NOT_NULL(ds, "dest->state should be non-null after successful deflateCopy");
    if (ds == nullptr) {
        cleanup_deflate_copy_test(&src, &dst);
        return false;
    }

    // The ds should reference the destination stream
    EXPECT_TRUE(ds->strm == &dst, "ds->strm should point back to destination z_stream");

    // Validate allocations inside ds were created
    EXPECT_NOT_NULL(ds->window, "ds->window should be allocated");
    EXPECT_NOT_NULL(ds->prev, "ds->prev should be allocated");
    EXPECT_NOT_NULL(ds->head, "ds->head should be allocated");
    EXPECT_NOT_NULL(ds->pending_buf, "ds->pending_buf should be allocated");

    // pending_out should point into pending_buf at the same offset as source's pending_out
    unsigned offset = (unsigned)(ss->pending_out - ss->pending_buf);
    EXPECT_TRUE(ds->pending_out == ds->pending_buf + offset,
                "ds->pending_out should point to an offset matching source's pending_out");

    // The copied dynamic trees pointers should be wired to the ds's own trees
    // These fields are implementation details but checking their basic linkage is useful
    // to ensure the assignment at the end of deflateCopy occurred.
    // We cast to internal fields to compare; if the exact types differ, this will still be a no-op
    // in many lib configurations, but we attempt to verify the intended linkage.
    if (ds->dyn_ltree && ds->l_desc.dyn_tree) {
        EXPECT_TRUE(ds->l_desc.dyn_tree == ds->dyn_ltree, "l_desc.dyn_tree should reference ds->dyn_ltree");
    }
    if (ds->dyn_dtree && ds->d_desc.dyn_tree) {
        EXPECT_TRUE(ds->d_desc.dyn_tree == ds->dyn_dtree, "d_desc.dyn_tree should reference ds->dyn_dtree");
    }
    if (ds->bl_tree && ds->bl_desc.dyn_tree) {
        EXPECT_TRUE(ds->bl_desc.dyn_tree == ds->bl_tree, "bl_desc.dyn_tree should reference ds->bl_tree");
    }

    // Cleanup
    // End the deflate stream to free allocated memory via deflateEnd
    deflateEnd(&dst);

    // Free source buffers and ss
    if (ss->window) free(ss->window);
    if (ss->prev) free(ss->prev);
    if (ss->head) free(ss->head);
    if (ss->pending_buf) free(ss->pending_buf);
    free(ss);

    // For safety, zero out
    std::memset(&src, 0, sizeof(src));
    std::memset(&dst, 0, sizeof(dst));

    return g_failures == 0;
}


// Test 2: Source state is NULL should trigger an error from deflateCopy
// This verifies that the function properly handles a null internal state pointer
// inside the source z_stream (i.e., source->state == NULL).
static bool test_deflateCopy_null_source_state()
{
    z_stream src;
    z_stream dst;
    std::memset(&src, 0, sizeof(src));
    std::memset(&dst, 0, sizeof(dst));

    // Source has no state; deflateStateCheck(source) should detect invalid state
    src.state = NULL;

    int ret = deflateCopy(&dst, &src);
    // Expect a stream error due to invalid source state
    EXPECT_TRUE(ret == Z_STREAM_ERROR, "deflateCopy should return Z_STREAM_ERROR when source->state is NULL");
    return g_failures == 0;
}


// Main runner
int main()
{
    std::cout << "Starting deflateCopy unit tests (non-GTest)\n";

    bool ok1 = test_deflateCopy_basic_copy();
    std::cout << "Test 1 (basic copy path): " << (ok1 ? "PASSED" : "FAILED") << "\n";

    // Reset failures counter for isolated reporting
    int prev_failures = g_failures;
    bool ok2 = test_deflateCopy_null_source_state();
    std::cout << "Test 2 (null source state): " << (ok2 ? "PASSED" : "FAILED") << "\n";

    // Aggregate results
    if (g_failures == prev_failures) {
        // No new failures in test2 (second test) if applicable
        // But we must report overall
    }

    std::cout << "Total failures: " << g_failures << "\n";

    // Return code: non-zero if any test failed
    return (g_failures == 0) ? 0 : 1;
}