// Unit tests for png_free_data implementation (C-style API adapted for C++11 tests)
// This test suite is designed to exercise various branches of the png_free_data function
// without relying on external libraries such as GTest. It uses a lightweight in-file
// mock of necessary structures and functions to verify behavior.

// Note: This file contains both the focal method (png_free_data) adapted for the test
// environment and the test cases. The tests compile under C++11.

// We provide explanatory comments for each test case as requested.

#include <vector>
#include <iostream>
#include <pngpriv.h>
#include <cstdlib>
#include <cstring>


// -------------------------
// Minimal mock of libpng types and helpers (C-like API in C++ file)
// -------------------------

// Basic typedefs and constants to mirror the subset used by png_free_data
typedef unsigned int png_uint_32;
typedef unsigned char png_byte;
typedef void* png_voidp;
typedef const void* png_const_voidp;
typedef struct png_struct_tag png_struct;
typedef png_struct* png_structrp;
typedef const png_struct* png_const_structrp;

typedef struct png_info_struct png_info_struct;
typedef png_info_struct* png_inforp;

typedef char* png_charp;
typedef const char* png_const_charp;
typedef unsigned int png_uint_32_t;

// Text item structure used by png_free_data
struct png_text {
    png_charp key;
};

// Simple text array wrapper
struct png_info_struct {
    // Text related
    png_text* text;
    int num_text;
    int max_text;
    // Free mask and validity flags
    unsigned int free_me;
    unsigned int valid;

    // tRNS data
    png_byte* trans_alpha;
    int num_trans;

    // sCAL
    png_byte* scal_s_width;
    png_byte* scal_s_height;

    // pCAL
    png_charp pcal_purpose;
    png_charp pcal_units;
    png_voidp* pcal_params;
    int pcal_nparams;

    // iCCP
    png_charp iccp_name;
    png_voidp iccp_profile;

    // sPLT
    struct {
        png_charp name;
        png_voidp entries;
    }* splt_palettes;
    int splt_palettes_num;

    // Unknown chunks
    struct {
        png_byte* data;
    }* unknown_chunks;
    int unknown_chunks_num;

    // eXIf
    png_voidp exif;

    // hIST
    png_voidp hist;

    // PLTE
    png_voidp palette;
    int num_palette;

    // Image data attached to info (for PNG_FREE_ROWS)
    size_t height;
    png_voidp* row_pointers;
};

// Pointers assert-like helper for test clarity
#define PNG_INFO_tRNS 0x01
#define PNG_INFO_sCAL 0x02
#define PNG_INFO_pCAL 0x04
#define PNG_INFO_iCCP 0x08
#define PNG_INFO_sPLT 0x10
#define PNG_INFO_eXIf 0x20
#define PNG_INFO_hIST 0x40
#define PNG_INFO_PLTE 0x80
#define PNG_INFO_IDAT 0x100

// Free mask bits (subset)
#define PNG_FREE_TEXT   0x01
#define PNG_FREE_TRNS   0x02
#define PNG_FREE_SCAL   0x04
#define PNG_FREE_PCAL   0x08
#define PNG_FREE_ICCP   0x10
#define PNG_FREE_SPLT   0x20
#define PNG_FREE_UNKN   0x40
#define PNG_FREE_EXIF   0x80
#define PNG_FREE_HIST   0x100
#define PNG_FREE_PLTE   0x200
#define PNG_FREE_ROWS   0x400
#define PNG_FREE_MUL    0x800

// Feature flags (simulated)
#define PNG_TEXT_SUPPORTED 1
#define PNG_tRNS_SUPPORTED 1
#define PNG_sCAL_SUPPORTED 1
#define PNG_pCAL_SUPPORTED 1
#define PNG_iCCP_SUPPORTED 1
#define PNG_sPLT_SUPPORTED 1
#define PNG_STORE_UNKNOWN_CHUNKS_SUPPORTED 1
#define PNG_eXIf_SUPPORTED 1
#define PNG_hIST_SUPPORTED 1
#define PNG_INFO_IMAGE_SUPPORTED 1

// Helper: a simple, non-terminating assertion
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define ASSERT(condition, msg) do { \
    if (condition) { \
        ++g_tests_passed; \
    } else { \
        ++g_tests_failed; \
        std::cerr << "Test failed: " << (msg) << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
    } \
} while (0)

#define ASSERT_EQ(a, b, msg) do { \
    if ((a) == (b)) { \
        ++g_tests_passed; \
    } else { \
        ++g_tests_failed; \
        std::cerr << "Test failed: " << (msg) \
                  << " (expected " << (b) << ", got " << (a) << ")" \
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
    } \
} while (0)

// Memory accounting helper: allocate and track
static void* xmalloc(size_t s) { void* p = std::malloc(s); std::memset(p, 0, s); return p; }
static void xfree(void* p) { std::free(p); }

// Minimal png_free implementation (dummy)
static void png_free(png_structrp /*png_ptr*/, void* ptr)
{
    if (ptr != nullptr) {
        xfree(ptr);
    }
}

// Minimal png_debug stub
static void png_debug(int /*level*/, const char* /*message*/) { /* no-op in tests */ }

// Forward declaration of focal function (adapted for test)
static void png_free_data(png_const_structrp png_ptr, png_inforp info_ptr,
                          png_uint_32 mask, int num);

// -------------------------
// Focal method: adapted version of png_free_data
// -------------------------

static void png_free_data(png_const_structrp /*png_ptr*/, png_inforp info_ptr,
                          png_uint_32 mask, int num)
{
    if (info_ptr == nullptr)
        return;

    // PNG_TEXT_SUPPORTED
#if PNG_TEXT_SUPPORTED
    if (info_ptr->text != nullptr &&
        ((mask & PNG_FREE_TEXT) & info_ptr->free_me) != 0)
    {
        if (num != -1)
        {
            png_free(nullptr, info_ptr->text[num].key);
            info_ptr->text[num].key = nullptr;
        }
        else
        {
            int i;
            for (i = 0; i < info_ptr->num_text; i++)
                png_free(nullptr, info_ptr->text[i].key);
            png_free(nullptr, info_ptr->text);
            info_ptr->text = nullptr;
            info_ptr->num_text = 0;
            info_ptr->max_text = 0;
        }
    }
#endif

// PNG_tRNS_SUPPORTED
#if PNG_tRNS_SUPPORTED
    if (((mask & PNG_FREE_TRNS) & info_ptr->free_me) != 0)
    {
        info_ptr->valid &= ~PNG_INFO_tRNS;
        png_free(nullptr, info_ptr->trans_alpha);
        info_ptr->trans_alpha = nullptr;
        info_ptr->num_trans = 0;
    }
#endif

// PNG_sCAL_SUPPORTED
#if PNG_sCAL_SUPPORTED
    if (((mask & PNG_FREE_SCAL) & info_ptr->free_me) != 0)
    {
        png_free(nullptr, info_ptr->scal_s_width);
        png_free(nullptr, info_ptr->scal_s_height);
        info_ptr->scal_s_width = nullptr;
        info_ptr->scal_s_height = nullptr;
        info_ptr->valid &= ~PNG_INFO_sCAL;
    }
#endif

// PNG_pCAL_SUPPORTED
#if PNG_pCAL_SUPPORTED
    if (((mask & PNG_FREE_PCAL) & info_ptr->free_me) != 0)
    {
        png_free(nullptr, info_ptr->pcal_purpose);
        png_free(nullptr, info_ptr->pcal_units);
        info_ptr->pcal_purpose = nullptr;
        info_ptr->pcal_units = nullptr;
        if (info_ptr->pcal_params != nullptr)
        {
            int i;
            for (i = 0; i < info_ptr->pcal_nparams; i++)
                png_free(nullptr, info_ptr->pcal_params[i]);
            png_free(nullptr, info_ptr->pcal_params);
            info_ptr->pcal_params = nullptr;
        }
        info_ptr->valid &= ~PNG_INFO_pCAL;
    }
#endif

// PNG_iCCP_SUPPORTED
#if PNG_iCCP_SUPPORTED
    if (((mask & PNG_FREE_ICCP) & info_ptr->free_me) != 0)
    {
        png_free(nullptr, info_ptr->iccp_name);
        png_free(nullptr, info_ptr->iccp_profile);
        info_ptr->iccp_name = nullptr;
        info_ptr->iccp_profile = nullptr;
        info_ptr->valid &= ~PNG_INFO_iCCP;
    }
#endif

// PNG_sPLT_SUPPORTED
#if PNG_sPLT_SUPPORTED
    if (info_ptr->splt_palettes != nullptr &&
        ((mask & PNG_FREE_SPLT) & info_ptr->free_me) != 0)
    {
        if (num != -1)
        {
            png_free(nullptr, info_ptr->splt_palettes[num].name);
            png_free(nullptr, info_ptr->splt_palettes[num].entries);
            info_ptr->splt_palettes[num].name = nullptr;
            info_ptr->splt_palettes[num].entries = nullptr;
        }
        else
        {
            int i;
            for (i = 0; i < info_ptr->splt_palettes_num; i++)
            {
                png_free(nullptr, info_ptr->splt_palettes[i].name);
                png_free(nullptr, info_ptr->splt_palettes[i].entries);
            }
            png_free(nullptr, info_ptr->splt_palettes);
            info_ptr->splt_palettes = nullptr;
            info_ptr->splt_palettes_num = 0;
            info_ptr->valid &= ~PNG_INFO_sPLT;
        }
    }
#endif

// PNG_STORE_UNKNOWN_CHUNKS_SUPPORTED
#if PNG_STORE_UNKNOWN_CHUNKS_SUPPORTED
    if (info_ptr->unknown_chunks != nullptr &&
        ((mask & PNG_FREE_UNKN) & info_ptr->free_me) != 0)
    {
        if (num != -1)
        {
            png_free(nullptr, info_ptr->unknown_chunks[num].data);
            info_ptr->unknown_chunks[num].data = nullptr;
        }
        else
        {
            int i;
            for (i = 0; i < info_ptr->unknown_chunks_num; i++)
                png_free(nullptr, info_ptr->unknown_chunks[i].data);
            png_free(nullptr, info_ptr->unknown_chunks);
            info_ptr->unknown_chunks = nullptr;
            info_ptr->unknown_chunks_num = 0;
        }
    }
#endif

// PNG_eXIf_SUPPORTED
#if PNG_eXIf_SUPPORTED
    if (((mask & PNG_FREE_EXIF) & info_ptr->free_me) != 0)
    {
        if (info_ptr->exif)
        {
            png_free(nullptr, info_ptr->exif);
            info_ptr->exif = nullptr;
        }
        info_ptr->valid &= ~PNG_INFO_eXIf;
    }
#endif

// PNG_hIST_SUPPORTED
#if PNG_hIST_SUPPORTED
    if (((mask & PNG_FREE_HIST) & info_ptr->free_me) != 0)
    {
        png_free(nullptr, info_ptr->hist);
        info_ptr->hist = nullptr;
        info_ptr->valid &= ~PNG_INFO_hIST;
    }
#endif

// Free PLTE
    if (((mask & PNG_FREE_PLTE) & info_ptr->free_me) != 0)
    {
        png_free(nullptr, info_ptr->palette);
        info_ptr->palette = nullptr;
        info_ptr->valid &= ~PNG_INFO_PLTE;
        info_ptr->num_palette = 0;
    }

// PNG_INFO_IMAGE_SUPPORTED (free rows)
#if PNG_INFO_IMAGE_SUPPORTED
    if (((mask & PNG_FREE_ROWS) & info_ptr->free_me) != 0)
    {
        if (info_ptr->row_pointers != nullptr)
        {
            png_uint_32 row;
            for (row = 0; row < info_ptr->height; row++)
                png_free(nullptr, info_ptr->row_pointers[row]);
            png_free(nullptr, info_ptr->row_pointers);
            info_ptr->row_pointers = nullptr;
        }
        info_ptr->valid &= ~PNG_INFO_IDAT;
    }
#endif

// Clear multiplication flag if non-multi-free
    if (num != -1)
        mask &= ~PNG_FREE_MUL;

// Update free_me to reflect freed flags
    info_ptr->free_me &= ~mask;
}

// -------------------------
// Test helpers: Test case implementations
// -------------------------

// Helper: initialize a fresh png_info_struct
static void reset_info(png_inforp info)
{
    // Zero all fields to a clean slate
    if (!info) return;
    std::memset(info, 0, sizeof(*info));
}

// Test 1: Free a single text item (num != -1)
static void test_free_text_single()
{
    // Setup info
    png_info_struct info;
    reset_info(reinterpret_cast<png_inforp>(&info));
    // Prepare a single text item
    info.text = static_cast<png_text*>(xmalloc(sizeof(png_text)));
    info.text[0].key = static_cast<char*>(xmalloc(16));
    std::strcpy(info.text[0].key, "SampleKey");
    info.num_text = 1;
    info.max_text = 1;
    info.free_me = PNG_FREE_TEXT; // allow text free
    info.valid = 0;

    // png_ptr is unused in free implementation; pass nullptr
    png_free_data(nullptr, reinterpret_cast<png_inforp>(&info), PNG_FREE_TEXT, 0);

    // Validation: the first key should be freed and set to NULL
    ASSERT(info.text[0].key == nullptr, "text[0].key should be NULL after free (single item).");
    // Note: In the implementation, it does not free the text array itself when num != -1,
    // but the key pointer should be NULL.
}

// Test 2: Free all text items (num == -1)
static void test_free_text_all()
{
    png_info_struct info;
    reset_info(reinterpret_cast<png_inforp>(&info));

    // Allocate two text items
    info.text = static_cast<png_text*>(xmalloc(sizeof(png_text) * 2));
    info.num_text = 2;
    info.max_text = 2;
    info.text[0].key = static_cast<char*>(xmalloc(10));
    info.text[1].key = static_cast<char*>(xmalloc(12));
    std::strcpy(info.text[0].key, "A");
    std::strcpy(info.text[1].key, "B");
    info.free_me = PNG_FREE_TEXT;
    info.valid = 0;

    png_free_data(nullptr, reinterpret_cast<png_inforp>(&info), PNG_FREE_TEXT, -1);

    // After freeing all, text pointer should be NULL and counters zero
    ASSERT(info.text == nullptr, "text array should be NULL after free-all.");
    ASSERT_EQ(info.num_text, 0, "num_text should be 0 after free-all.");
    ASSERT_EQ(info.max_text, 0, "max_text should be 0 after free-all.");
}

// Test 3: Free tRNS entry (invalidated color info)
static void test_free_trns()
{
    png_info_struct info;
    reset_info(reinterpret_cast<png_inforp>(&info));

    info.free_me = PNG_FREE_TRNS;
    info.valid = PNG_INFO_tRNS;
    info.trans_alpha = static_cast<png_byte*>(xmalloc(5));
    info.num_trans = 1;

    png_free_data(nullptr, reinterpret_cast<png_inforp>(&info), PNG_FREE_TRNS, -1);

    // Validate tRNS data is freed and flags cleared
    ASSERT(info.trans_alpha == nullptr, "trans_alpha should be NULL after free TRNS.");
    ASSERT((info.num_trans == 0), "num_trans should be 0 after free TRNS.");
    // Ensure valid flag no longer has tRNS bit
    ASSERT((info.valid & PNG_INFO_tRNS) == 0, "valid should clear tRNS bit after free TRNS.");
}

// Test 4: Free sPLT entry (single palette)
static void test_free_splt_single()
{
    png_info_struct info;
    reset_info(reinterpret_cast<png_inforp>(&info));

    // Allocate one SPLT palette
    info.splt_palettes = static_cast<decltype(info.splt_palettes)>(xmalloc(sizeof(info.splt_palettes[0])));
    info.splt_palettes_num = 1;
    info.splt_palettes[0].name = static_cast<char*>(xmalloc(20));
    info.splt_palettes[0].entries = static_cast<void*>(xmalloc(40));
    std::strcpy(info.splt_palettes[0].name, "Palette1");
    info.free_me = PNG_FREE_SPLT;
    info.valid = PNG_INFO_sPLT;

    png_free_data(nullptr, reinterpret_cast<png_inforp>(&info), PNG_FREE_SPLT, 0);

    // Validate that the first palette's name and entries are NULLed
    ASSERT(info.splt_palettes != nullptr, "splt_palettes should remain allocated as array after single-free.");
    ASSERT(info.splt_palettes[0].name == nullptr, "name of first SPLT should be NULL after free.");
    ASSERT(info.splt_palettes[0].entries == nullptr, "entries of first SPLT should be NULL after free.");
}

// Test 5: Free unknown chunks (all)
static void test_free_unknown_chunks_all()
{
    png_info_struct info;
    reset_info(reinterpret_cast<png_inforp>(&info));

    // Allocate unknown_chunks array with two items
    info.unknown_chunks = static_cast<decltype(info.unknown_chunks)>(xmalloc(sizeof(info.unknown_chunks[0])));
    info.unknown_chunks_num = 2; // for loop, we mock as 2 and free accordingly
    info.unknown_chunks[0].data = static_cast<png_byte*>(xmalloc(8));
    info.unknown_chunks[1].data = static_cast<png_byte*>(xmalloc(12));
    info.free_me = PNG_FREE_UNKN;
    info.valid = 0;

    png_free_data(nullptr, reinterpret_cast<png_inforp>(&info), PNG_FREE_UNKN, -1);

    // Expect the array to be freed and pointer NULL
    ASSERT(info.unknown_chunks == nullptr, "unknown_chunks should be NULL after free-all.");
    ASSERT(info.unknown_chunks_num == 0, "unknown_chunks_num should be 0 after free-all.");
}

// Test 6: Free PLTE entry
static void test_free_plte()
{
    png_info_struct info;
    reset_info(reinterpret_cast<png_inforp>(&info));

    info.palette = xmalloc(16);
    info.num_palette = 1;
    info.free_me = PNG_FREE_PLTE;
    info.valid = PNG_INFO_PLTE;

    png_free_data(nullptr, reinterpret_cast<png_inforp>(&info), PNG_FREE_PLTE, -1);

    ASSERT(info.palette == nullptr, "palette should be NULL after free PLTE.");
    ASSERT((info.valid & PNG_INFO_PLTE) == 0, "PNG_INFO_PLTE flag should be cleared after free PLTE.");
    ASSERT(info.num_palette == 0, "num_palette should be 0 after free PLTE.");
}

// Test 7: Free image rows
static void test_free_rows()
{
    png_info_struct info;
    reset_info(reinterpret_cast<png_inforp>(&info));

    // Allocate two row pointers
    info.height = 2;
    info.row_pointers = static_cast<png_voidp*>(xmalloc(sizeof(png_voidp) * info.height));
    info.row_pointers[0] = xmalloc(4);
    info.row_pointers[1] = xmalloc(4);
    info.free_me = PNG_FREE_ROWS;
    info.valid = PNG_INFO_IDAT;

    png_free_data(nullptr, reinterpret_cast<png_inforp>(&info), PNG_FREE_ROWS, -1);

    // Validate that rows are freed and pointer array is NULL
    ASSERT(info.row_pointers == nullptr, "row_pointers should be NULL after free-rows.");
    ASSERT((info.valid & PNG_INFO_IDAT) == 0, "PNG_INFO_IDAT flag should be cleared after free-rows.");
}

// Test 8: NULL inputs should be safely ignored (no crash)
static void test_null_input()
{
    // Should not crash; nothing to verify, just ensure no exceptions
    png_inforp fake = nullptr;
    png_free_data(nullptr, fake, PNG_FREE_TEXT, 0);
    ASSERT(true, "NULL input should safely return from png_free_data.");
}

// -------------------------
// Test runner
// -------------------------

static void run_all_tests()
{
    std::cout << "Running png_free_data unit tests (mocked environment)..." << std::endl;

    test_free_text_single();
    test_free_text_all();
    test_free_trns();
    test_free_splt_single();
    test_free_unknown_chunks_all();
    test_free_plte();
    test_free_rows();
    test_null_input();

    std::cout << "Tests completed. Passed: " << g_tests_passed
              << ", Failed: " << g_tests_failed << std::endl;
}

// -------------------------
// Main
// -------------------------

int main()
{
    run_all_tests();
    return (g_tests_failed == 0) ? 0 : 1;
}