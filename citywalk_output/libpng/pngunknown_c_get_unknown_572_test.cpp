// Self-contained test suite for the focal method get_unknown
// This file re-implements a minimal subset of the dependencies
// to exercise the logic of get_unknown without external libraries.
// The goal is to provide high-coverage unit tests in a single TU
// compatible with C++11 and without GTest.

#include <setjmp.h>
#include <vector>
#include <string.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <map>
#include <stdio.h>
#include <cstring>


// Domain knowledge: static members and file-scope helpers are mocked here.

#define UNUSED(x) /* nothing */

// Minimal typedefs to mirror the original C structures used by get_unknown
typedef unsigned int png_uint_32;
typedef unsigned char png_byte;
typedef void* png_infop;
typedef void* png_structp;

// Constants emulating libpng control flags
enum {
   PNG_HANDLE_CHUNK_AS_DEFAULT = 0,
   PNG_HANDLE_CHUNK_NEVER = 1,
   PNG_HANDLE_CHUNK_IF_SAFE = 2,
   PNG_HANDLE_CHUNK_ALWAYS = 3
};

// Forward declare get_unknown to mimic the original API signature
// (we implement it in this TU for tests)
struct display;

// Structure representing an unknown chunk (name only for testing)
struct png_unknown_chunk {
    const char* name;
};
typedef png_unknown_chunk* png_unknown_chunkp;

// Global unknown chunks storage used by the test harness
static png_unknown_chunk g_unknowns[16];
static int g_num_unknown = 0;

// Helper: simulate known chunk info table and flags
static const char* g_known_names[] = { "IHDR", "PLTE", "IDAT", "IEND" };
static const int KNOWN_COUNT = 4;
struct ChunkInfo {
    png_uint_32 flag;
};
static ChunkInfo chunk_info[KNOWN_COUNT] = {
    { 0x01 }, // IHDR
    { 0x02 }, // PLTE
    { 0x04 }, // IDAT
    { 0x08 }  // IEND
};

// Map to control which chunk is ancillary (for IF_SAFE path)
static std::map<std::string, bool> g_ancillary_map;

// Mock: findb returns index of known chunk by name or -1 if unknown
static int findb(const png_byte* name) {
    for (int i = 0; i < KNOWN_COUNT; ++i) {
        if (std::strcmp(reinterpret_cast<const char*>(name), g_known_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

// Mock: ancillaryb returns whether a chunk name is ancillary (non-critical)
static bool ancillaryb(const png_byte* name) {
    auto it = g_ancillary_map.find(reinterpret_cast<const char*>(name));
    if (it != g_ancillary_map.end()) return it->second;
    return false;
}

// Mock: png_get_unknown_chunks fills the unknowns array and returns its count
static int png_get_unknown_chunks(void* /*png_ptr*/, png_infop /*info_ptr*/, png_unknown_chunkp *unknown) {
    *unknown = g_unknowns;
    return g_num_unknown;
}

// The actual focal method under test (re-implemented for the test TU)
png_uint_32 get_unknown(struct display* d, png_infop info_ptr, int after_IDAT) {
{
   // Create corresponding 'unknown' flags
   png_uint_32 flags = 0;
   UNUSED(after_IDAT)
   {
      png_unknown_chunkp unknown;
      int num_unknown = png_get_unknown_chunks(d->png_ptr, info_ptr, &unknown);
      while (--num_unknown >= 0)
      {
         int chunk = findb(reinterpret_cast<const png_byte*>(unknown[num_unknown].name));
         /* Chunks not known to pngunknown must be validated here; since they
          * must also be unknown to libpng the 'display->keep' behavior should
          * have been used.
          */
         if (chunk < 0) switch (d->keep)
         {
            default: /* impossible */
            case PNG_HANDLE_CHUNK_AS_DEFAULT:
            case PNG_HANDLE_CHUNK_NEVER:
               fprintf(stderr, "%s(%s): %s: %s: unknown chunk saved\n",
                  d->file, d->test, d->keep ? "discard" : "default",
                  unknown[num_unknown].name);
               ++(d->error_count);
               break;
            case PNG_HANDLE_CHUNK_IF_SAFE:
               if (!ancillaryb(reinterpret_cast<const png_byte*>(unknown[num_unknown].name)))
               {
                  fprintf(stderr,
                     "%s(%s): if-safe: %s: unknown critical chunk saved\n",
                     d->file, d->test, unknown[num_unknown].name);
                  ++(d->error_count);
                  break;
               }
               /* FALLTHROUGH */ /* (safe) */
            case PNG_HANDLE_CHUNK_ALWAYS:
               break;
         }
         else
            flags |= chunk_info[chunk].flag;
      }
   }
   return flags;
}
}

// Display structure used by get_unknown
struct display {
    const char* file;
    const char* test;
    int keep; // 0 => default, 1 => discard
    int error_count;
    void* png_ptr;
};

// Helpers for tests
static void set_unknowns(const char* names[], int count) {
    g_num_unknown = count;
    for (int i = 0; i < count; ++i) {
        g_unknowns[i].name = names[i];
    }
}

static void clear_ancillary() {
    g_ancillary_map.clear();
}

static void set_ancillary(const char* name, bool value) {
    g_ancillary_map[std::string(name)] = value;
}

// Simple, non-terminating assertion framework
static int g_total_tests = 0;
static int g_failed_tests = 0;
#define EXPECT_EQ(expected, actual, msg) do { \
    ++g_total_tests; \
    if ((expected) != (actual)) { \
        std::cerr << "EXPECT_EQ FAILED: " << (msg) \
                  << " | expected: " << (expected) \
                  << " actual: " << (actual) << std::endl; \
        ++g_failed_tests; \
    } \
} while(0)

#define EXPECT_TRUE(cond, msg) do { \
    ++g_total_tests; \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE FAILED: " << (msg) \
                  << " | condition: " << #cond << std::endl; \
        ++g_failed_tests; \
    } \
} while(0)

static void test_description(const char* s) {
    std::cout << "Running test: " << s << std::endl;
}

// Test 1: Unknown chunk is not known to libpng and keep=default => error_count increments
static void test_unknown_unknown_default_increments_error() {
    test_description("Unknown chunk (not known to libpng) with default handling increments error_count");
    display d;
    d.file = "testfile.c";
    d.test = "test_unknown_default";
    d.keep = PNG_HANDLE_CHUNK_AS_DEFAULT;
    d.error_count = 0;
    d.png_ptr = nullptr;
    // Setup unknowns: one unknown chunk FOO
    const char* names[] = { "FOO" };
    set_unknowns(names, 1);
    // Ensure ancillary map empty
    clear_ancillary();

    png_uint_32 flags = get_unknown(&d, nullptr, 0);

    EXPECT_EQ(0, flags, "Flags should be 0 when unknown chunk and unknown path (default)");
    EXPECT_EQ(1, d.error_count, "Error count should increment for unknown default case");
}

// Test 2: Unknown chunk not known to libpng with IF_SAFE and ancillaryb returns false => error
static void test_unknown_unknown_if_safe_and_ancillary_false_increments_error() {
    test_description("Unknown chunk with IF_SAFE and non-ancillary name should increment error_count");
    display d;
    d.file = "testfile.c";
    d.test = "test_if_safe_ancillary_false";
    d.keep = PNG_HANDLE_CHUNK_IF_SAFE;
    d.error_count = 0;
    d.png_ptr = nullptr;
    const char* names[] = { "BAR" };
    set_unknowns(names, 1);
    clear_ancillary();
    set_ancillary("BAR", false);

    png_uint_32 flags = get_unknown(&d, nullptr, 0);

    EXPECT_EQ(0, flags, "Flags should be 0 when unknown and IF_SAFE with non-ancillary chunk");
    EXPECT_EQ(1, d.error_count, "Error count should increment for IF_SAFE non-ancillary");
}

// Test 3: Unknown chunk IF_SAFE with ancillary true should not increment error_count
static void test_unknown_unknown_if_safe_and_ancillary_true_no_error() {
    test_description("Unknown chunk with IF_SAFE and ancillary chunk should not increment error_count");
    display d;
    d.file = "testfile.c";
    d.test = "test_if_safe_ancillary_true";
    d.keep = PNG_HANDLE_CHUNK_IF_SAFE;
    d.error_count = 0;
    d.png_ptr = nullptr;
    const char* names[] = { "BAR2" };
    set_unknowns(names, 1);
    clear_ancillary();
    set_ancillary("BAR2", true);

    png_uint_32 flags = get_unknown(&d, nullptr, 0);

    EXPECT_EQ(0, flags, "Flags should remain 0 when IF_SAFE with ancillary chunk");
    EXPECT_EQ(0, d.error_count, "Error count should remain 0 in ancillary-safe path");
}

// Test 4: Known chunk (e.g., IDAT) should accumulate its flag
static void test_known_chunk_accumulates_flags() {
    test_description("Known chunk should accumulate its flag from chunk_info");
    display d;
    d.file = "testfile.c";
    d.test = "test_known_idat";
    d.keep = PNG_HANDLE_CHUNK_AS_DEFAULT;
    d.error_count = 0;
    d.png_ptr = nullptr;
    const char* names[] = { "IDAT" };
    set_unknowns(names, 1);

    png_uint_32 flags = get_unknown(&d, nullptr, 0);

    EXPECT_EQ(0x04, flags, "IDAT should contribute flag 0x04");
    EXPECT_EQ(0, d.error_count, "Error count should remain 0 for known chunk");
}

// Test 5: Mixed unknowns: first unknown (FOO) triggers error, second (IDAT) adds known flag
static void test_mixed_unknowns_order_and_flags() {
    test_description("Mixed unknowns: unknown FOO triggers error, followed by known IDAT flag");
    display d;
    d.file = "testfile.c";
    d.test = "test_mixed_unknowns";
    d.keep = PNG_HANDLE_CHUNK_AS_DEFAULT;
    d.error_count = 0;
    d.png_ptr = nullptr;
    const char* names[] = { "IDAT", "FOO" }; // Note: order matters due to --num_unknown semantics
    set_unknowns(names, 2);
    clear_ancillary();

    png_uint_32 flags = get_unknown(&d, nullptr, 0);

    // First processed is names[1] = "FOO" -> unknown => default path -> error
    // Then processed is names[0] = "IDAT" -> known path -> set flag 0x04
    EXPECT_EQ(0x04, flags, "After processing, IDAT flag should be set to 0x04");
    EXPECT_EQ(1, d.error_count, "One unknown should increment error_count in default mode");
}

// Runner
int main() {
    // Initialize test suite
    test_unknown_unknown_default_increments_error();
    test_unknown_unknown_if_safe_and_ancillary_false_increments_error();
    test_unknown_unknown_if_safe_and_ancillary_true_no_error();
    test_known_chunk_accumulates_flags();
    test_mixed_unknowns_order_and_flags();

    std::cout << "Tests run: " << g_total_tests << ", Failures: " << g_failed_tests << std::endl;
    return g_failed_tests == 0 ? 0 : 1;
}