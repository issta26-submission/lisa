// Self-contained C++11 unit test suite for the focal method
// init_standard_palette as implemented in the provided snippet.
// Note: This is a self-contained mock environment designed to exercise
// the logic of init_standard_palette without relying on external libpng.
// It implements minimal stand-ins for the dependent types and functions
// used by the focal method, and verifies key behaviors via direct state
// inspection. This is suitable for environments where GTest is not allowed.

#include <cassert>
#include <signal.h>
#include <string.h>
#include <iomanip>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Domain: self-contained mocks to mimic the real interfaces
// The real project would provide these via libpng headers.
typedef void* png_structp;
typedef void* png_infop;
typedef void* png_store; // opaque in real code; kept as void* for test

// Minimal color structure matching usage in init_standard_palette
struct png_color {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

// The project uses a store_palette_entry with fields red/green/blue/alpha
// We'll declare it here to align with make_standard_palette's contract.
// We do not rely on the exact storage in the real project; we only emulate it.
struct store_palette_entry {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
};

// Forward declaration of the focal helper used by init_standard_palette
// In the real project, make_standard_palette(ps, npalette, do_tRNS) returns
// a pointer to an array of store_palette_entry. We emulate that here.
static store_palette_entry fake_palette_storage[256];
store_palette_entry* make_standard_palette(png_store *ps, int npalette, int do_tRNS)
{
    // Produce deterministic palette entries for testing purposes.
    // For i in [0, npalette)
    //   red   = i
    //   green = i + 1
    //   blue  = i + 2
    //   alpha = 0 or 255 depending on i (to exercise tRNS path)
    for (int i = 0; i < npalette; ++i) {
        fake_palette_storage[i].red   = static_cast<unsigned char>(i & 0xFF);
        fake_palette_storage[i].green = static_cast<unsigned char>((i + 1) & 0xFF);
        fake_palette_storage[i].blue  = static_cast<unsigned char>((i + 2) & 0xFF);
        // Alternate alpha to ensure some entries are non-opaque
        fake_palette_storage[i].alpha = (i % 2 == 0) ? 255 : 200;
    }
    // If for some reason npalette == 0, still return non-null to mimic API.
    return fake_palette_storage;
}

// Captured PLTE writeback (test spy for png_set_PLTE)
static bool g_plte_written = false;
static int g_plte_npalette = -1;
static png_color g_plte_palette[256];

// Captured tRNS writeback (test spy for png_set_tRNS)
static bool g_trns_written = false;
static int g_trns_num = 0;

// Mock implementations of the libpng write hooks used by the focal method.
// In a real test environment these would be external symbols provided by the
// test harness; here we implement lightweight spies to observe behavior.
void png_set_PLTE(png_structp pp, png_infop pi, png_color palette[], int npalette)
{
    g_plte_written = true;
    g_plte_npalette = npalette;
    // Copy up to 256 palette entries
    for (int i = 0; i < (npalette < 256 ? npalette : 256); ++i)
    {
        g_plte_palette[i] = palette[i];
    }
}
void png_set_tRNS(png_structp pp, png_infop pi, png_byte tRNS[], int num, void* color)
{
    g_trns_written = true;
    g_trns_num = (num > 255) ? 255 : num; // clamp for safety in test
}

// Re-declare the key function under test (as per the provided snippet signature)
typedef unsigned char png_byte;
struct png_color rgba_to_color(unsigned char r, unsigned char g, unsigned char b) {
    png_color c;
    c.red = r; c.green = g; c.blue = b;
    return c;
}

store_palette_entry* make_standard_palette(png_store* ps, int npalette, int do_tRNS);
void init_standard_palette(png_store *ps, png_structp pp, png_infop pi, int npalette, int do_tRNS);

// Implementation of the focal function, adapted to use the mocks above.
// This mirrors the exact logic in the provided <FOCAL_METHOD> block.
store_palette_entry* make_standard_palette(png_store *ps, int npalette, int do_tRNS)
{
    return make_standard_palette(ps, npalette, do_tRNS); // forward to our local impl
}

// The actual focal function under test (reproduced for the self-contained test)
void init_standard_palette(png_store *ps, png_structp pp, png_infop pi, int npalette, int do_tRNS)
{
    {
        store_palette_entry *ppal = make_standard_palette(ps, npalette, do_tRNS);
        {
            int i;
            png_color palette[256];
            /* Set all entries to detect overread errors. */
            for (i=0; i<npalette; ++i)
            {
                palette[i].red = ppal[i].red;
                palette[i].green = ppal[i].green;
                palette[i].blue = ppal[i].blue;
            }
            /* Just in case fill in the rest with detectable values: */
            for (; i<256; ++i)
                palette[i].red = palette[i].green = palette[i].blue = 42;
            png_set_PLTE(pp, pi, palette, npalette);
        }
        if (do_tRNS)
        {
            int i, j;
            png_byte tRNS[256];
            /* Set all the entries, but skip trailing opaque entries */
            for (i=j=0; i<npalette; ++i)
                if ((tRNS[i] = ppal[i].alpha) < 255)
                    j = i+1;
            /* Fill in the remainder with a detectable value: */
            for (; i<256; ++i)
                tRNS[i] = 24;
#ifdef PNG_WRITE_tRNS_SUPPORTED
            if (j > 0)
                png_set_tRNS(pp, pi, tRNS, j, 0/*color*/);
#endif
        }
    }
}

// Simple test harness
#define ASSERT_TRUE(cond) do { if (!(cond)) { std::cerr << "ASSERT_TRUE failed: " << #cond << std::endl; ++g_failures; } } while (0)
#define ASSERT_EQ(a,b) do { if ((a) != (b)) { std::cerr << "ASSERT_EQ failed: " << #a << " (" << (a) << ") != " << #b << " (" << (b) << ")" << std::endl; ++g_failures; } } while (0)
static int g_failures = 0;

void reset_state()
{
    g_plte_written = false;
    g_plte_npalette = -1;
    std::memset(g_plte_palette, 0, sizeof(g_plte_palette));
    g_trns_written = false;
    g_trns_num = 0;
}

// Domain Knowledge-Driven Tests

// Test 1: Basic PLTE population and overread protection
// - npalette = 5
// - do_tRNS = 0
// Expectation:
// - PLTE was written with npalette = 5
// - palette[0..4] matches the values produced by make_standard_palette
// - palette[5..255] all equal 42 (to detect overreads)
void test_init_standard_palette_basic_plte()
{
    reset_state();
    png_store *ps = nullptr;
    png_structp pp = nullptr;
    png_infop pi = nullptr;
    int npalette = 5;
    int do_tRNS = 0;

    init_standard_palette(ps, pp, pi, npalette, do_tRNS);

    // PLTE write should be invoked
    ASSERT_TRUE(g_plte_written);
    ASSERT_EQ(g_plte_npalette, npalette);

    // Validate first npalette entries reflect make_standard_palette() output
    for (int i = 0; i < npalette; ++i)
    {
        unsigned char expected_red   = static_cast<unsigned char>(i);
        unsigned char expected_green = static_cast<unsigned char>((i + 1) & 0xFF);
        unsigned char expected_blue  = static_cast<unsigned char>((i + 2) & 0xFF);
        ASSERT_EQ(g_plte_palette[i].red,   expected_red);
        ASSERT_EQ(g_plte_palette[i].green, expected_green);
        ASSERT_EQ(g_plte_palette[i].blue,  expected_blue);
    }

    // Validate remaining entries are filled with 42
    for (int i = npalette; i < 256; ++i)
    {
        ASSERT_TRUE(g_plte_palette[i].red   == 42);
        ASSERT_TRUE(g_plte_palette[i].green == 42);
        ASSERT_TRUE(g_plte_palette[i].blue  == 42);
    }

    // No tRNS should be written in this test since do_tRNS == 0
    ASSERT_TRUE(!g_trns_written);
}

// Test 2: tRNS path is exercised when do_tRNS is true
// - npalette = 3
// - do_tRNS = 1
// Expectation:
// - PLTE is written with npalette = 3
// - tRNS is written if any alpha < 255 in the palette
//   (Our make_standard_palette assigns alpha 255 for even i and 200 for odd i,
//    so at least one entry will be < 255 -> tRNS should be written)
void test_init_standard_palette_with_tRNS()
{
    reset_state();
    png_store *ps = nullptr;
    png_structp pp = nullptr;
    png_infop pi = nullptr;
    int npalette = 3;
    int do_tRNS = 1;

    init_standard_palette(ps, pp, pi, npalette, do_tRNS);

    // PLTE write should be invoked
    ASSERT_TRUE(g_plte_written);
    ASSERT_EQ(g_plte_npalette, npalette);

    // tRNS should be written if any non-opaque entry exists (our mock ensures
    // that we would hit the code path and the macro is defined in the test
    // environment to allow this to run)
#ifdef PNG_WRITE_tRNS_SUPPORTED
    ASSERT_TRUE(g_trns_written);
    ASSERT_TRUE(g_trns_num > 0);
#else
    // If tRNS is not compiled in, we cannot assert this; just ensure code path
    // does not crash and that the PLTE path still executed.
    // We still assert PLTE correctness above.
#endif

    // Validate first npalette entries
    for (int i = 0; i < npalette; ++i)
    {
        unsigned char expected_red   = static_cast<unsigned char>(i);
        unsigned char expected_green = static_cast<unsigned char>((i + 1) & 0xFF);
        unsigned char expected_blue  = static_cast<unsigned char>((i + 2) & 0xFF);
        ASSERT_EQ(g_plte_palette[i].red,   expected_red);
        ASSERT_EQ(g_plte_palette[i].green, expected_green);
        ASSERT_EQ(g_plte_palette[i].blue,  expected_blue);
    }
}

// Test 3: Zero palette should still behave gracefully
// - npalette = 0
// - do_tRNS = 0
// Expectation:
// - PLTE is written with npalette = 0
void test_init_standard_palette_zero()
{
    reset_state();
    png_store *ps = nullptr;
    png_structp pp = nullptr;
    png_infop pi = nullptr;
    int npalette = 0;
    int do_tRNS = 0;

    init_standard_palette(ps, pp, pi, npalette, do_tRNS);

    // PLTE write should be invoked with zero entries
    ASSERT_TRUE(g_plte_written);
    ASSERT_EQ(g_plte_npalette, npalette);

    // Entries 0..255 should all be 42 (the "detect overread" filler)
    for (int i = 0; i < 256; ++i)
    {
        ASSERT_TRUE(g_plte_palette[i].red == 42 &&
                    g_plte_palette[i].green == 42 &&
                    g_plte_palette[i].blue == 42);
    }

    // No tRNS write in this test
    ASSERT_TRUE(!g_trns_written);
}

// Runner
int main()
{
    std::cout << "Running init_standard_palette unit tests (self-contained mock)\n";

    test_init_standard_palette_basic_plte();
    test_init_standard_palette_with_tRNS();
    test_init_standard_palette_zero();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED (" << 3 << " tests).\n";
        return 0;
    } else {
        std::cout << g_failures << " TEST(S) FAILED.\n";
        return 1;
    }
}