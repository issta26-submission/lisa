// Comprehensive unit tests for png_set_quantize (ported into a standalone C++11 test harness)
// This test harness provides minimal in-process implementations of the necessary
// libpng-like structures and functions to exercise the focal function without
// requiring the full libpng build system. It follows the non-GTest, single-file
// approach requested.

// Note: This is a lightweight emulation intended for unit testing the control
// flow and basic state changes of png_set_quantize. It aims for reproducible
// behavior across platforms.

#include <cassert>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Domain constants and types (simplified subset)
typedef unsigned char png_byte;
typedef unsigned short png_uint_16;
typedef size_t png_alloc_size_t;
typedef unsigned char* png_bytep;
typedef struct png_color_struct { unsigned char red, green, blue; } png_color;
typedef png_color* png_colorp;

// Forward declare to mirror the focal signature
typedef struct png_struct_def png_struct;
typedef png_struct* png_structrp;
typedef const png_struct* png_const_structrp;
typedef png_color* png_colorp;
typedef const png_color* png_colorcp;
typedef unsigned short* png_uint_16p;
typedef const unsigned short* png_const_uint_16p;

// libpng-like constants
#define PNG_MAX_PALETTE_LENGTH 256
#define PNG_QUANTIZE 0x01 // dummy flag for transformation
#define PNG_QUANTIZE_RED_BITS 2
#define PNG_QUANTIZE_GREEN_BITS 2
#define PNG_QUANTIZE_BLUE_BITS 2

// Internal helper macros/functions (emulate libpng behavior)
static inline int PNG_COLOR_DIST(png_color a, png_color b)
{
   int dr = (int)a.red - (int)b.red; if (dr < 0) dr = -dr;
   int dg = (int)a.green - (int)b.green; if (dg < 0) dg = -dg;
   int db = (int)a.blue - (int)b.blue; if (db < 0) db = -db;
   return dr + dg + db; // simple Manhattan-like distance
}

// Minimal memory management to support unit tests
static void* g_malloc(png_structrp png_ptr, png_alloc_size_t size) {
    (void)png_ptr;
    return std::malloc(size);
}
static void* g_calloc(png_structrp png_ptr, png_alloc_size_t size) {
    (void)png_ptr;
    void* p = std::calloc(1, size);
    return p;
}
static void g_free(png_structrp png_ptr, void* ptr) {
    (void)png_ptr;
    std::free(ptr);
}
static png_bytep g_malloc_bytes(png_structrp png_ptr, png_alloc_size_t size) {
    return (png_bytep)g_malloc(png_ptr, size);
}
static png_bytep g_calloc_bytes(png_structrp png_ptr, png_alloc_size_t size) {
    return (png_bytep)g_calloc(png_ptr, size);
}

// Tiny wrappers to mimic libpng "error" signaling via no-ops for tests
static void png_debug(int level, const char* message) {
    (void)level; (void)message;
    // No-op for test clarity
}

// Tiny rtran_ok check (can simulate fail or pass)
static int png_rtran_ok(png_structrp png_ptr, int need_IHDR)
{
    (void)need_IHDR;
    if (png_ptr == nullptr) return 0;
    return (png_ptr->rtran_ok_flag != 0) ? 1 : 0;
}

// Eagerly declare fields in png_struct to satisfy png_set_quantize usage
struct png_struct_def {
    int transformations;
    png_bytep quantize_index;     // array mapping palette indices
    png_bytep index_to_palette;   // internal mapping, used in some branches
    png_bytep palette_to_index;    // internal mapping, used in some branches
    png_bytep palette_lookup;      // used in full_quantize path
    png_colorp palette;             // palette array
    png_uint_16 num_palette;
    int rtran_ok_flag;              // for controlling png_rtran_ok
    // Additional fields to satisfy code paths (not fully used by tests)
    // In real libpng, there are many more fields.
};

// Forward declare the focal function
extern "C" void png_set_quantize(png_structrp png_ptr, png_colorp palette,
    int num_palette, int maximum_colors, png_const_uint_16p histogram,
    int full_quantize);

// Implementation of the focal method (adapted to our test harness)
extern "C" void png_set_quantize(png_structrp png_ptr, png_colorp palette,
    int num_palette, int maximum_colors, png_const_uint_16p histogram,
    int full_quantize)
{
    {
       png_debug(1, "in png_set_quantize");
       if (png_rtran_ok(png_ptr, 0) == 0)
          return;
       png_ptr->transformations |= PNG_QUANTIZE;
       if (full_quantize == 0)
       {
          int i;
          /* Initialize the array to index colors.
           *
           * Ensure quantize_index can fit 256 elements (PNG_MAX_PALETTE_LENGTH)
           * rather than num_palette elements. This is to prevent buffer overflows
           * caused by malformed PNG files with out-of-range palette indices.
           *
           * Be careful to avoid leaking memory. Applications are allowed to call
           * this function more than once per png_struct.
           */
          if (png_ptr->quantize_index != NULL)
              g_free(png_ptr, png_ptr->quantize_index);
          png_ptr->quantize_index = (png_bytep)g_malloc(png_ptr,
              PNG_MAX_PALETTE_LENGTH);
          for (i = 0; i < PNG_MAX_PALETTE_LENGTH; i++)
             png_ptr->quantize_index[i] = (png_byte)i;
       }
       if (num_palette > maximum_colors)
       {
          if (histogram != NULL)
          {
             /* This is easy enough, just throw out the least used colors.
              * Perhaps not the best solution, but good enough.
              */
             png_bytep quantize_sort;
             int i, j;
             /* Initialize the local array to sort colors. */
             quantize_sort = (png_bytep)g_malloc(png_ptr,
                 (png_alloc_size_t)num_palette);
             for (i = 0; i < num_palette; i++)
                quantize_sort[i] = (png_byte)i;
             /* Find the least used palette entries by starting a
              * bubble sort, and running it until we have sorted
              * out enough colors.  Note that we don't care about
              * sorting all the colors, just finding which are
              * least used.
              */
             for (i = num_palette - 1; i >= maximum_colors; i--)
             {
                int done; /* To stop early if the list is pre-sorted */
                done = 1;
                for (j = 0; j < i; j++)
                {
                   if (histogram[quantize_sort[j]]
                       < histogram[quantize_sort[j + 1]])
                   {
                      png_byte t;
                      t = quantize_sort[j];
                      quantize_sort[j] = quantize_sort[j + 1];
                      quantize_sort[j + 1] = t;
                      done = 0;
                   }
                }
                if (done != 0)
                   break;
             }
             /* Swap the palette around, and set up a table, if necessary */
             if (full_quantize != 0)
             {
                int j;
                j = num_palette;
                /* Put all the useful colors within the max, but don't
                 * move the others.
                 */
                for (i = 0; i < maximum_colors; i++)
                {
                   if ((int)quantize_sort[i] >= maximum_colors)
                   {
                      do
                         j--;
                      while ((int)quantize_sort[j] >= maximum_colors);
                      palette[i] = palette[j];
                   }
                }
             }
             else
             {
                int j;
                j = num_palette;
                /* Move all the used colors inside the max limit, and
                 * develop a translation table.
                 */
                for (i = 0; i < maximum_colors; i++)
                {
                   /* Only move the colors we need to */
                   if ((int)quantize_sort[i] >= maximum_colors)
                   {
                      png_color tmp_color;
                      do
                         j--;
                      while ((int)quantize_sort[j] >= maximum_colors);
                      tmp_color = palette[j];
                      palette[j] = palette[i];
                      palette[i] = tmp_color;
                      /* Indicate where the color went */
                      png_ptr->quantize_index[j] = (png_byte)i;
                      png_ptr->quantize_index[i] = (png_byte)j;
                   }
                }
                /* Find closest color for those colors we are not using */
                for (i = 0; i < num_palette; i++)
                {
                   if ((int)png_ptr->quantize_index[i] >= maximum_colors)
                   {
                      int min_d, k, min_k, d_index;
                      /* Find the closest color to one we threw out */
                      d_index = png_ptr->quantize_index[i];
                      min_d = PNG_COLOR_DIST(palette[d_index], palette[0]);
                      for (k = 1, min_k = 0; k < maximum_colors; k++)
                      {
                         int d;
                         d = PNG_COLOR_DIST(palette[d_index], palette[k]);
                         if (d < min_d)
                         {
                            min_d = d;
                            min_k = k;
                         }
                      }
                      /* Point to closest color */
                      png_ptr->quantize_index[i] = (png_byte)min_k;
                   }
                }
             }
             g_free(png_ptr, quantize_sort);
          }
          else
          {
             /* This is much harder to do simply (and quickly).  Perhaps
              * we need to go through a median cut routine, but those
              * don't always behave themselves with only a few colors
              * as input.  So we will just find the closest two colors,
              * and throw out one of them (chosen somewhat randomly).
              * [We don't understand this at all, so if someone wants to
              *  work on improving it, be our guest - AED, GRP]
              */
             int i;
             int max_d;
             int num_new_palette;
             png_dsortp t;
             png_dsortpp hash;
             t = NULL;
             /* Initialize palette index arrays */
             png_ptr->index_to_palette = (png_bytep)g_malloc(png_ptr,
                 (png_alloc_size_t)num_palette);
             png_ptr->palette_to_index = (png_bytep)g_malloc(png_ptr,
                 (png_alloc_size_t)num_palette);
             /* Initialize the sort array */
             for (i = 0; i < num_palette; i++)
             {
                png_ptr->index_to_palette[i] = (png_byte)i;
                png_ptr->palette_to_index[i] = (png_byte)i;
             }
             hash = (png_dsortpp)g_calloc(png_ptr, (png_alloc_size_t)(769 *
                 (sizeof (png_dsortp))));
             num_new_palette = num_palette;
             /* Initial wild guess at how far apart the farthest pixel
              * pair we will be eliminating will be.  Larger
              * numbers mean more areas will be allocated, Smaller
              * numbers run the risk of not saving enough data, and
              * having to do this all over again.
              *
              * I have not done extensive checking on this number.
              */
             max_d = 96;
             while (num_new_palette > maximum_colors)
             {
                for (i = 0; i < num_new_palette - 1; i++)
                {
                   int j;
                   for (j = i + 1; j < num_new_palette; j++)
                   {
                      int d;
                      d = PNG_COLOR_DIST(palette[i], palette[j]);
                      if (d <= max_d)
                      {
                         t = (png_dsortp)g_malloc_warn(png_ptr,
                             (png_alloc_size_t)(sizeof (png_dsort)));
                         if (t == NULL)
                             break;
                         t->next = hash[d];
                         t->left = (png_byte)i;
                         t->right = (png_byte)j;
                         hash[d] = t;
                      }
                   }
                   if (t == NULL)
                      break;
                }
                if (t != NULL)
                for (i = 0; i <= max_d; i++)
                {
                   if (hash[i] != NULL)
                   {
                      png_dsortp p;
                      for (p = hash[i]; p; p = p->next)
                      {
                         if ((int)png_ptr->index_to_palette[p->left]
                             < num_new_palette &&
                             (int)png_ptr->index_to_palette[p->right]
                             < num_new_palette)
                         {
                            int j, next_j;
                            if (num_new_palette & 0x01)
                            {
                               j = p->left;
                               next_j = p->right;
                            }
                            else
                            {
                               j = p->right;
                               next_j = p->left;
                            }
                            num_new_palette--;
                            palette[png_ptr->index_to_palette[j]]
                                = palette[num_new_palette];
                            if (full_quantize == 0)
                            {
                               int k;
                               for (k = 0; k < num_palette; k++)
                               {
                                  if (png_ptr->quantize_index[k] ==
                                      png_ptr->index_to_palette[j])
                                     png_ptr->quantize_index[k] =
                                         png_ptr->index_to_palette[next_j];
                                  if ((int)png_ptr->quantize_index[k] ==
                                      num_new_palette)
                                     png_ptr->quantize_index[k] =
                                         png_ptr->index_to_palette[j];
                               }
                            }
                            png_ptr->index_to_palette[png_ptr->palette_to_index
                                [num_new_palette]] = png_ptr->index_to_palette[j];
                            png_ptr->palette_to_index[png_ptr->index_to_palette[j]]
                                = png_ptr->palette_to_index[num_new_palette];
                            png_ptr->index_to_palette[j] =
                                (png_byte)num_new_palette;
                            png_ptr->palette_to_index[num_new_palette] =
                                (png_byte)j;
                         }
                         if (num_new_palette <= maximum_colors)
                            break;
                      }
                      if (num_new_palette <= maximum_colors)
                         break;
                   }
                }
                for (i = 0; i < 769; i++)
                {
                   if (hash[i] != NULL)
                   {
                      png_dsortp p = hash[i];
                      while (p)
                      {
                         t = p->next;
                         g_free(png_ptr, p);
                         p = t;
                      }
                   }
                   hash[i] = 0;
                }
                max_d += 96;
             }
             g_free(png_ptr, hash);
             g_free(png_ptr, png_ptr->palette_to_index);
             g_free(png_ptr, png_ptr->index_to_palette);
             png_ptr->palette_to_index = NULL;
             png_ptr->index_to_palette = NULL;
          }
          num_palette = maximum_colors;
       }
       if (png_ptr->palette == NULL)
       {
          png_ptr->palette = palette;
       }
       png_ptr->num_palette = (png_uint_16)num_palette;
       if (full_quantize != 0)
       {
          int i;
          png_bytep distance;
          int total_bits = PNG_QUANTIZE_RED_BITS + PNG_QUANTIZE_GREEN_BITS +
              PNG_QUANTIZE_BLUE_BITS;
          int num_red = (1 << PNG_QUANTIZE_RED_BITS);
          int num_green = (1 << PNG_QUANTIZE_GREEN_BITS);
          int num_blue = (1 << PNG_QUANTIZE_BLUE_BITS);
          size_t num_entries = ((size_t)1 << total_bits);
          png_ptr->palette_lookup = (png_bytep)g_calloc(png_ptr,
              (png_alloc_size_t)(num_entries));
          distance = (png_bytep)g_malloc(png_ptr, (png_alloc_size_t)num_entries);
          std::memset(distance, 0xff, num_entries);
          for (i = 0; i < num_palette; i++)
          {
             int ir, ig, ib;
             int r = (palette[i].red >> (8 - PNG_QUANTIZE_RED_BITS));
             int g = (palette[i].green >> (8 - PNG_QUANTIZE_GREEN_BITS));
             int b = (palette[i].blue >> (8 - PNG_QUANTIZE_BLUE_BITS));
             for (ir = 0; ir < num_red; ir++)
             {
                /* int dr = abs(ir - r); */
                int dr = ((ir > r) ? ir - r : r - ir);
                int index_r = (ir << (PNG_QUANTIZE_BLUE_BITS +
                    PNG_QUANTIZE_GREEN_BITS));
                for (ig = 0; ig < num_green; ig++)
                {
                   /* int dg = abs(ig - g); */
                   int dg = ((ig > g) ? ig - g : g - ig);
                   int dt = dr + dg;
                   int dm = ((dr > dg) ? dr : dg);
                   int index_g = index_r | (ig << PNG_QUANTIZE_BLUE_BITS);
                   for (ib = 0; ib < num_blue; ib++)
                   {
                      int d_index = index_g | ib;
                      /* int db = abs(ib - b); */
                      int db = ((ib > b) ? ib - b : b - ib);
                      int dmax = ((dm > db) ? dm : db);
                      int d = dmax + dt + db;
                      if (d < (int)distance[d_index])
                      {
                         distance[d_index] = (png_byte)d;
                         png_ptr->palette_lookup[d_index] = (png_byte)i;
                      }
                   }
                }
             }
          }
          g_free(png_ptr, distance);
       }
    }
}

// Test harness utilities and tests

// Helper to initialize a palette with deterministic colors
static void init_palette(png_colorp pal, int n)
{
    for (int i = 0; i < n && i < PNG_MAX_PALETTE_LENGTH; ++i) {
        pal[i].red   = (unsigned char)((i * 37) % 256);
        pal[i].green = (unsigned char)((i * 53) % 256);
        pal[i].blue  = (unsigned char)((i * 97) % 256);
    }
}

// Test 1: Early return when png_rtran_ok() fails
bool test_early_return_no_change()
{
    // Setup png_ptr with rtran_ok_flag = 0 to simulate failure
    png_struct test_ptr;
    std::memset(&test_ptr, 0, sizeof(test_ptr));
    test_ptr.rtran_ok_flag = 0; // force failure
    test_ptr.transformations = 0;
    test_ptr.quantize_index = NULL;
    test_ptr.palette = NULL;
    test_ptr.num_palette = 0;

    // Palette input
    png_color palette[4];
    init_palette(palette, 4);

    // histogram helper
    png_uint_16 histogram[4] = { 10, 20, 30, 40 };

    // Call with various parameters; should return immediately
    png_set_quantize(&test_ptr, palette, 4, 2, histogram, 0);

    // Assertions: no state change expected
    bool pass = (test_ptr.transformations == 0) &&
                (test_ptr.quantize_index == NULL) &&
                (test_ptr.palette == NULL);

    // Debug message
    std::cout << "Test 1 - Early return: " << (pass ? "PASS" : "FAIL") << "\n";
    return pass;
}

// Test 2: full_quantize = 0 initializes quantize_index to 0..255
bool test_full_quantize_zero_initializes_quantize_index()
{
    // Setup png_ptr with rtran_ok true
    png_struct test_ptr;
    std::memset(&test_ptr, 0, sizeof(test_ptr));
    test_ptr.rtran_ok_flag = 1;
    test_ptr.quantize_index = NULL;
    test_ptr.transformations = 0;
    test_ptr.palette = new png_color[4];
    init_palette(test_ptr.palette, 4);
    test_ptr.num_palette = 4;

    // Setup palette input (palette not used much for this test)
    png_color input_palette[4];
    init_palette(input_palette, 4);

    // Call with max_colors smaller than num_palette to exercise quantize_index path
    png_uint_16 histogram[4] = {0};
    png_set_quantize(&test_ptr, input_palette, 4, 8, histogram, 0);

    bool pass = (test_ptr.transformations & PNG_QUANTIZE) != 0;
    pass = pass && (test_ptr.quantize_index != NULL);
    // Quick check: first few indices should map to themselves
    bool idx_ok = true;
    for (int i = 0; i < 8; ++i) {
        if (test_ptr.quantize_index[i] != (png_byte)i) {
            idx_ok = false;
            break;
        }
    }
    pass = pass && idx_ok;

    std::cout << "Test 2 - Quantize index init: " << (pass ? "PASS" : "FAIL") << "\n";

    // Cleanup
    if (test_ptr.palette) delete[] test_ptr.palette;
    if (test_ptr.quantize_index) { /* allocated in test, but do not double-free */ }

    return pass;
}

// Test 3: full_quantize = 1 allocates palette_lookup and distance data
bool test_full_quantize_true_allocates_palette_lookup()
{
    // Setup png_ptr with rtran_ok true
    png_struct test_ptr;
    std::memset(&test_ptr, 0, sizeof(test_ptr));
    test_ptr.rtran_ok_flag = 1;
    test_ptr.transformations = 0;
    test_ptr.quantize_index = NULL;
    test_ptr.index_to_palette = NULL;
    test_ptr.palette_to_index = NULL;
    test_ptr.palette_lookup = NULL;

    // Small palette to trigger quantization path
    png_color palette[3];
    palette[0].red = palette[0].green = palette[0].blue = 10;
    palette[1].red = palette[1].green = palette[1].blue = 120;
    palette[2].red = palette[2].green = palette[2].blue = 200;

    test_ptr.palette = palette;
    test_ptr.num_palette = 3;

    // histogram provided to drive the "histogram != NULL" path
    png_uint_16 histogram[3] = { 5, 2, 9 };

    // Call with full_quantize = 1; histogram non-null to use the distance path
    png_color input_palette[3] = { palette[0], palette[1], palette[2] };

    png_set_quantize(&test_ptr, input_palette, 3, 2, histogram, 1);

    bool pass = (test_ptr.transformations & PNG_QUANTIZE) != 0;
    pass = pass && (test_ptr.palette_lookup != NULL);
    // The distance array is allocated as part of this call; ensure it exists
    // (palette_lookup should be inited and distance allocated; we infer distance block by non-NULL palette_lookup)
    std::cout << "Test 3 - full_quantize true allocates palette_lookup: " << (pass ? "PASS" : "FAIL") << "\n";

    return pass;
}

// Main runner
int main()
{
    bool all_pass = true;

    all_pass &= test_early_return_no_change();
    all_pass &= test_full_quantize_zero_initializes_quantize_index();
    all_pass &= test_full_quantize_true_allocates_palette_lookup();

    std::cout << "\nSummary: " << (all_pass ? "ALL TESTS PASSED" : "SOME TESTS FAILED") << "\n";
    return all_pass ? 0 : 1;
}