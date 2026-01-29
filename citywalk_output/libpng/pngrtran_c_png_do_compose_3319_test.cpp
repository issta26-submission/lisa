// Minimal C++11 test harness for the focal function: png_do_compose
// This test suite provides light-weight unit tests without any external test framework.
// It exercises a subset of the png_do_compose behavior for key color types and bit depths.
// The tests cover true/false branches of critical predicates by controlling input row data
// and the png_ptr state. Static/global test feedback is printed to stdout/stderr.

#include <cstdint>
#include <vector>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Lightweight redefinition of necessary PNG-like types and constants
typedef unsigned char png_byte;
typedef unsigned int  png_uint_32;
typedef unsigned short png_uint_16;
typedef png_byte*       png_bytep;
typedef const png_byte* png_const_bytep;

// Color type constants (subset)
enum {
    PNG_COLOR_TYPE_GRAY       = 0,
    PNG_COLOR_TYPE_RGB        = 2,
    PNG_COLOR_TYPE_GRAY_ALPHA = 4,
    PNG_COLOR_TYPE_RGB_ALPHA  = 6
};

// Flag constants (subset)
#define PNG_FLAG_OPTIMIZE_ALPHA 0x01

// Simple 16-bit color container to mimic libpng's internal structure
typedef struct {
    unsigned int gray;
    unsigned int red;
    unsigned int green;
    unsigned int blue;
} png_color_16;

// Forward declarations for the focal function and its helpers
struct png_row_info;
typedef png_row_info* png_row_infop;

struct png_row_info {
    png_uint_32 width;
    int color_type;
    int bit_depth;
};

// Fully defined png_struct minimal subset used by the focal method
struct png_struct {
    // Gamma-related members (only used when PNG_READ_GAMMA_SUPPORTED is defined)
    const png_bytep gamma_table;
    const png_bytep gamma_from_1;
    const png_bytep gamma_to_1;
    const png_uint_16** gamma_16_table;
    const png_uint_16** gamma_16_from_1;
    const png_uint_16** gamma_16_to_1;
    int gamma_shift;
    int flags;

    png_color_16 trans_color;
    png_color_16 background;
    png_color_16 background_1;
};

// Typedef to mimic libpng's "pointer to png_struct"
typedef png_struct* png_structrp;

// Forward declaration of a tiny diagnostic function used by the focal method
static void png_debug(int, const char*) { /* no-op in tests */ }

// Prototyped focal function copied/adapted to be self-contained for tests
void png_do_compose(png_row_infop row_info, png_bytep row, png_structrp png_ptr)
{
{
#ifdef PNG_READ_GAMMA_SUPPORTED
   png_const_bytep gamma_table = png_ptr->gamma_table;
   png_const_bytep gamma_from_1 = png_ptr->gamma_from_1;
   png_const_bytep gamma_to_1 = png_ptr->gamma_to_1;
   png_const_uint_16pp gamma_16 = png_ptr->gamma_16_table;
   png_const_uint_16pp gamma_16_from_1 = png_ptr->gamma_16_from_1;
   png_const_uint_16pp gamma_16_to_1 = png_ptr->gamma_16_to_1;
   int gamma_shift = png_ptr->gamma_shift;
   int optimize = (png_ptr->flags & PNG_FLAG_OPTIMIZE_ALPHA) != 0;
#endif
   png_bytep sp;
   png_uint_32 i;
   png_uint_32 row_width = row_info->width;
   int shift;
   png_debug(1, "in png_do_compose");
   switch (row_info->color_type)
   {
      case PNG_COLOR_TYPE_GRAY:
      {
         switch (row_info->bit_depth)
         {
            case 1:
            {
               sp = row;
               shift = 7;
               for (i = 0; i < row_width; i++)
               {
                  if ((png_uint_16)((*sp >> shift) & 0x01)
                     == png_ptr->trans_color.gray)
                  {
                     unsigned int tmp = *sp & (0x7f7f >> (7 - shift));
                     tmp |=
                         (unsigned int)(png_ptr->background.gray << shift);
                     *sp = (png_byte)(tmp & 0xff);
                  }
                  if (shift == 0)
                  {
                     shift = 7;
                     sp++;
                  }
                  else
                     shift--;
               }
               break;
            }
            case 2:
            {
#ifdef PNG_READ_GAMMA_SUPPORTED
               if (gamma_table != NULL)
               {
                  sp = row;
                  shift = 6;
                  for (i = 0; i < row_width; i++)
                  {
                     if ((png_uint_16)((*sp >> shift) & 0x03)
                         == png_ptr->trans_color.gray)
                     {
                        unsigned int tmp = *sp & (0x3f3f >> (6 - shift));
                        tmp |=
                           (unsigned int)png_ptr->background.gray << shift;
                        *sp = (png_byte)(tmp & 0xff);
                     }
                     else
                     {
                        unsigned int p = (*sp >> shift) & 0x03;
                        unsigned int g = (gamma_table [p | (p << 2) |
                            (p << 4) | (p << 6)] >> 6) & 0x03;
                        unsigned int tmp = *sp & (0x3f3f >> (6 - shift));
                        tmp |= (unsigned int)(g << shift);
                        *sp = (png_byte)(tmp & 0xff);
                     }
                     if (shift == 0)
                     {
                        shift = 6;
                        sp++;
                     }
                     else
                        shift -= 2;
                  }
               }
               else
#endif
               {
                  sp = row;
                  shift = 6;
                  for (i = 0; i < row_width; i++)
                  {
                     if ((png_uint_16)((*sp >> shift) & 0x03)
                         == png_ptr->trans_color.gray)
                     {
                        unsigned int tmp = *sp & (0x3f3f >> (6 - shift));
                        tmp |=
                            (unsigned int)png_ptr->background.gray << shift;
                        *sp = (png_byte)(tmp & 0xff);
                     }
                     if (shift == 0)
                     {
                        shift = 6;
                        sp++;
                     }
                     else
                        shift -= 2;
                  }
               }
               break;
            }
            case 4:
            {
#ifdef PNG_READ_GAMMA_SUPPORTED
               if (gamma_table != NULL)
               {
                  sp = row;
                  shift = 4;
                  for (i = 0; i < row_width; i++)
                  {
                     if ((png_uint_16)((*sp >> shift) & 0x0f)
                         == png_ptr->trans_color.gray)
                     {
                        unsigned int tmp = *sp & (0x0f0f >> (4 - shift));
                        tmp |=
                           (unsigned int)(png_ptr->background.gray << shift);
                        *sp = (png_byte)(tmp & 0xff);
                     }
                     else
                     {
                        unsigned int p = (*sp >> shift) & 0x0f;
                        unsigned int g = (gamma_table[p | (p << 4)] >> 4) &
                           0x0f;
                        unsigned int tmp = *sp & (0x0f0f >> (4 - shift));
                        tmp |= (unsigned int)(g << shift);
                        *sp = (png_byte)(tmp & 0xff);
                     }
                     if (shift == 0)
                     {
                        shift = 4;
                        sp++;
                     }
                     else
                        shift -= 4;
                  }
               }
               else
#endif
               {
                  sp = row;
                  shift = 4;
                  for (i = 0; i < row_width; i++)
                  {
                     if ((png_uint_16)((*sp >> shift) & 0x0f)
                         == png_ptr->trans_color.gray)
                     {
                        unsigned int tmp = *sp & (0x0f0f >> (4 - shift));
                        tmp |=
                           (unsigned int)(png_ptr->background.gray << shift);
                        *sp = (png_byte)(tmp & 0xff);
                     }
                     if (shift == 0)
                     {
                        shift = 4;
                        sp++;
                     }
                     else
                        shift -= 4;
                  }
               }
               break;
            }
            case 8:
            {
#ifdef PNG_READ_GAMMA_SUPPORTED
               if (gamma_table != NULL)
               {
                  sp = row;
                  for (i = 0; i < row_width; i++, sp++)
                  {
                     if (*sp == png_ptr->trans_color.gray)
                        *sp = (png_byte)png_ptr->background.gray;
                     else
                        *sp = gamma_table[*sp];
                  }
               }
               else
#endif
               {
                  sp = row;
                  for (i = 0; i < row_width; i++, sp++)
                  {
                     if (*sp == png_ptr->trans_color.gray)
                        *sp = (png_byte)png_ptr->background.gray;
                  }
               }
               break;
            }
            case 16:
            {
#ifdef PNG_READ_GAMMA_SUPPORTED
               if (gamma_16 != NULL)
               {
                  sp = row;
                  for (i = 0; i < row_width; i++, sp += 2)
                  {
                     png_uint_16 v;
                     v = (png_uint_16)(((*sp) << 8) + *(sp + 1));
                     if (v == png_ptr->trans_color.gray)
                     {
                        /* Background is already in screen gamma */
                        *sp = (png_byte)((png_ptr->background.gray >> 8)
                             & 0xff);
                        *(sp + 1) = (png_byte)(png_ptr->background.gray
                             & 0xff);
                     }
                     else
                     {
                        v = gamma_16[*(sp + 1) >> gamma_shift][*sp];
                        *sp = (png_byte)((v >> 8) & 0xff);
                        *(sp + 1) = (png_byte)(v & 0xff);
                     }
                  }
               }
               else
#endif
               {
                  sp = row;
                  for (i = 0; i < row_width; i++, sp += 2)
                  {
                     png_uint_16 v;
                     v = (png_uint_16)(((*sp) << 8) + *(sp + 1));
                     if (v == png_ptr->trans_color.gray)
                     {
                        *sp = (png_byte)((png_ptr->background.gray >> 8)
                             & 0xff);
                        *(sp + 1) = (png_byte)(png_ptr->background.gray
                             & 0xff);
                     }
                  }
               }
               break;
            }
            default:
               break;
         }
         break;
      }
      case PNG_COLOR_TYPE_RGB:
      {
         if (row_info->bit_depth == 8)
         {
#ifdef PNG_READ_GAMMA_SUPPORTED
            if (gamma_table != NULL)
            {
               sp = row;
               for (i = 0; i < row_width; i++, sp += 3)
               {
                  if (*sp == png_ptr->trans_color.red &&
                      *(sp + 1) == png_ptr->trans_color.green &&
                      *(sp + 2) == png_ptr->trans_color.blue)
                  {
                     *sp = (png_byte)png_ptr->background.red;
                     *(sp + 1) = (png_byte)png_ptr->background.green;
                     *(sp + 2) = (png_byte)png_ptr->background.blue;
                  }
                  else
                  {
                     *sp = gamma_table[*sp];
                     *(sp + 1) = gamma_table[*(sp + 1)];
                     *(sp + 2) = gamma_table[*(sp + 2)];
                  }
               }
            }
            else
#endif
            {
               sp = row;
               for (i = 0; i < row_width; i++, sp += 3)
               {
                  if (*sp == png_ptr->trans_color.red &&
                      *(sp + 1) == png_ptr->trans_color.green &&
                      *(sp + 2) == png_ptr->trans_color.blue)
                  {
                     *sp = (png_byte)png_ptr->background.red;
                     *(sp + 1) = (png_byte)png_ptr->background.green;
                     *(sp + 2) = (png_byte)png_ptr->background.blue;
                  }
               }
            }
         }
         else /* if (row_info->bit_depth == 16) */
         {
#ifdef PNG_READ_GAMMA_SUPPORTED
            if (gamma_16 != NULL)
            {
               sp = row;
               for (i = 0; i < row_width; i++, sp += 6)
               {
                  png_uint_16 r = (png_uint_16)(((*sp) << 8) + *(sp + 1));
                  png_uint_16 g = (png_uint_16)(((*(sp + 2)) << 8)
                      + *(sp + 3));
                  png_uint_16 b = (png_uint_16)(((*(sp + 4)) << 8)
                      + *(sp + 5));
                  if (r == png_ptr->trans_color.red &&
                      g == png_ptr->trans_color.green &&
                      b == png_ptr->trans_color.blue)
                  {
                     /* Background is already in screen gamma */
                     *sp = (png_byte)((png_ptr->background.red >> 8) & 0xff);
                     *(sp + 1) = (png_byte)(png_ptr->background.red
                             & 0xff);
                     *(sp + 2) = (png_byte)((png_ptr->background.green >> 8)
                             & 0xff);
                     *(sp + 3) = (png_byte)(png_ptr->background.green
                             & 0xff);
                     *(sp + 4) = (png_byte)((png_ptr->background.blue >> 8)
                             & 0xff);
                     *(sp + 5) = (png_byte)(png_ptr->background.blue & 0xff);
                  }
                  else
                  {
                     png_uint_16 v = gamma_16[*(sp + 1) >> gamma_shift][*sp];
                     *sp = (png_byte)((v >> 8) & 0xff);
                     *(sp + 1) = (png_byte)(v & 0xff);
                     v = gamma_16[*(sp + 3) >> gamma_shift][*(sp + 2)];
                     *(sp + 2) = (png_byte)((v >> 8) & 0xff);
                     *(sp + 3) = (png_byte)(v & 0xff);
                     v = gamma_16[*(sp + 5) >> gamma_shift][*(sp + 4)];
                     *(sp + 4) = (png_byte)((v >> 8) & 0xff);
                     *(sp + 5) = (png_byte)(v & 0xff);
                  }
               }
            }
            else
#endif
            {
               sp = row;
               for (i = 0; i < row_width; i++, sp += 6)
               {
                  png_uint_16 r = (png_uint_16)(((*sp) << 8) + *(sp + 1));
                  png_uint_16 g = (png_uint_16)(((*(sp + 2)) << 8)
                      + *(sp + 3));
                  png_uint_16 b = (png_uint_16)(((*(sp + 4)) << 8)
                      + *(sp + 5));
                  if (r == png_ptr->trans_color.red &&
                      g == png_ptr->trans_color.green &&
                      b == png_ptr->trans_color.blue)
                  {
                     *sp = (png_byte)((png_ptr->background.red >> 8) & 0xff);
                     *(sp + 1) = (png_byte)(png_ptr->background.red & 0xff);
                     *(sp + 2) = (png_byte)((png_ptr->background.green >> 8)
                             & 0xff);
                     *(sp + 3) = (png_byte)(png_ptr->background.green
                             & 0xff);
                     *(sp + 4) = (png_byte)((png_ptr->background.blue >> 8)
                             & 0xff);
                     *(sp + 5) = (png_byte)(png_ptr->background.blue & 0xff);
                  }
               }
            }
         }
         break;
      }
      case PNG_COLOR_TYPE_GRAY_ALPHA:
      {
         if (row_info->bit_depth == 8)
         {
#ifdef PNG_READ_GAMMA_SUPPORTED
            if (gamma_to_1 != NULL && gamma_from_1 != NULL &&
                gamma_table != NULL)
            {
               sp = row;
               for (i = 0; i < row_width; i++, sp += 2)
               {
                  png_uint_16 a = *(sp + 1);
                  if (a == 0xff)
                     *sp = gamma_table[*sp];
                  else if (a == 0)
                  {
                     /* Background is already in screen gamma */
                     *sp = (png_byte)png_ptr->background.gray;
                  }
                  else
                  {
                     png_byte v, w;
                     v = gamma_to_1[*sp];
                     png_composite(w, v, a, png_ptr->background_1.gray);
                     if (optimize == 0)
                        w = gamma_from_1[w];
                     *sp = w;
                  }
               }
            }
            else
#endif
            {
               sp = row;
               for (i = 0; i < row_width; i++, sp += 2)
               {
                  png_byte a = *(sp + 1);
                  if (a == 0)
                     *sp = (png_byte)png_ptr->background.gray;
                  else if (a < 0xff)
                     png_composite(*sp, *sp, a, png_ptr->background.gray);
               }
            }
         }
         else /* if (png_ptr->bit_depth == 16) */
         {
#ifdef PNG_READ_GAMMA_SUPPORTED
            if (gamma_16 != NULL && gamma_16_from_1 != NULL &&
                gamma_16_to_1 != NULL)
            {
               sp = row;
               for (i = 0; i < row_width; i++, sp += 4)
               {
                  png_uint_16 a = (png_uint_16)(((*(sp + 2)) << 8)
                      + *(sp + 3));
                  if (a == (png_uint_16)0xffff)
                  {
                     png_uint_16 v;
                     v = gamma_16[*(sp + 1) >> gamma_shift][*sp];
                     *sp = (png_byte)((v >> 8) & 0xff);
                     *(sp + 1) = (png_byte)(v & 0xff);
                  }
                  else if (a == 0)
                  {
                     /* Background is already in screen gamma */
                     *sp = (png_byte)((png_ptr->background.gray >> 8)
                             & 0xff);
                     *(sp + 1) = (png_byte)(png_ptr->background.gray & 0xff);
                  }
                  else
                  {
                     png_uint_16 g, v, w;
                     g = gamma_16_to_1[*(sp + 1) >> gamma_shift][*sp];
                     png_composite_16(v, g, a, png_ptr->background_1.gray);
                     if (optimize != 0)
                        w = v;
                     else
                        w = gamma_16_from_1[(v & 0xff) >>
                            gamma_shift][v >> 8];
                     *sp = (png_byte)((w >> 8) & 0xff);
                     *(sp + 1) = (png_byte)(w & 0xff);
                  }
               }
            }
            else
#endif
            {
               sp = row;
               for (i = 0; i < row_width; i++, sp += 4)
               {
                  png_uint_16 a = (png_uint_16)(((*(sp + 2)) << 8)
                      + *(sp + 3));
                  if (a == 0)
                  {
                     *sp = (png_byte)((png_ptr->background.gray >> 8) & 0xff);
                     *(sp + 1) = (png_byte)(png_ptr->background.gray & 0xff);
                  }
                  else if (a < 0xffff)
                  {
                     png_uint_16 g, v;
                     g = (png_uint_16)(((*sp) << 8) + *(sp + 1));
                     png_composite_16(v, g, a, png_ptr->background.gray);
                     *sp = (png_byte)((v >> 8) & 0xff);
                     *(sp + 1) = (png_byte)(v & 0xff);
                  }
               }
            }
         }
         break;
      }
      case PNG_COLOR_TYPE_RGB_ALPHA:
      {
         if (row_info->bit_depth == 8)
         {
#ifdef PNG_READ_GAMMA_SUPPORTED
            if (gamma_to_1 != NULL && gamma_from_1 != NULL &&
                gamma_table != NULL)
            {
               sp = row;
               for (i = 0; i < row_width; i++, sp += 4)
               {
                  png_byte a = *(sp + 3);
                  if (a == 0xff)
                  {
                     *sp = gamma_table[*sp];
                     *(sp + 1) = gamma_table[*(sp + 1)];
                     *(sp + 2) = gamma_table[*(sp + 2)];
                  }
                  else if (a == 0)
                  {
                     /* Background is already in screen gamma */
                     *sp = (png_byte)png_ptr->background.red;
                     *(sp + 1) = (png_byte)png_ptr->background.green;
                     *(sp + 2) = (png_byte)png_ptr->background.blue;
                  }
                  else
                  {
                     png_byte v, w;
                     v = gamma_to_1[*sp];
                     png_composite(w, v, a, png_ptr->background_1.red);
                     if (optimize == 0) w = gamma_from_1[w];
                     *sp = w;
                     v = gamma_to_1[*(sp + 1)];
                     png_composite(w, v, a, png_ptr->background_1.green);
                     if (optimize == 0) w = gamma_from_1[w];
                     *(sp + 1) = w;
                     v = gamma_to_1[*(sp + 2)];
                     png_composite(w, v, a, png_ptr->background_1.blue);
                     if (optimize == 0) w = gamma_from_1[w];
                     *(sp + 2) = w;
                  }
               }
            }
            else
#endif
            {
               sp = row;
               for (i = 0; i < row_width; i++, sp += 4)
               {
                  png_byte a = *(sp + 3);
                  if (a == 0)
                  {
                     *sp = (png_byte)png_ptr->background.red;
                     *(sp + 1) = (png_byte)png_ptr->background.green;
                     *(sp + 2) = (png_byte)png_ptr->background.blue;
                  }
                  else if (a < 0xff)
                  {
                     png_composite(*sp, *sp, a, png_ptr->background.red);
                     png_composite(*(sp + 1), *(sp + 1), a,
                         png_ptr->background.green);
                     png_composite(*(sp + 2), *(sp + 2), a,
                         png_ptr->background.blue);
                  }
               }
            }
         }
         else /* if (row_info->bit_depth == 16) */
         {
#ifdef PNG_READ_GAMMA_SUPPORTED
            if (gamma_16 != NULL && gamma_16_from_1 != NULL &&
                gamma_16_to_1 != NULL)
            {
               sp = row;
               for (i = 0; i < row_width; i++, sp += 8)
               {
                  png_uint_16 a = (png_uint_16)(((png_uint_16)(*(sp + 6))
                      << 8) + (png_uint_16)(*(sp + 7)));
                  if (a == (png_uint_16)0xffff)
                  {
                     png_uint_16 v;
                     v = gamma_16[*(sp + 1) >> gamma_shift][*sp];
                     *sp = (png_byte)((v >> 8) & 0xff);
                     *(sp + 1) = (png_byte)(v & 0xff);
                     v = gamma_16[*(sp + 3) >> gamma_shift][*(sp + 2)];
                     *(sp + 2) = (png_byte)((v >> 8) & 0xff);
                     *(sp + 3) = (png_byte)(v & 0xff);
                     v = gamma_16[*(sp + 5) >> gamma_shift][*(sp + 4)];
                     *(sp + 4) = (png_byte)((v >> 8) & 0xff);
                     *(sp + 5) = (png_byte)(v & 0xff);
                  }
                  else if (a == 0)
                  {
                     /* Background is already in screen gamma */
                     *sp = (png_byte)((png_ptr->background.red >> 8) & 0xff);
                     *(sp + 1) = (png_byte)(png_ptr->background.red & 0xff);
                     *(sp + 2) = (png_byte)((png_ptr->background.green >> 8)
                             & 0xff);
                     *(sp + 3) = (png_byte)(png_ptr->background.green
                             & 0xff);
                     *(sp + 4) = (png_byte)((png_ptr->background.blue >> 8)
                             & 0xff);
                     *(sp + 5) = (png_byte)(png_ptr->background.blue & 0xff);
                  }
                  else
                  {
                     png_uint_16 v, w;
                     v = gamma_16_to_1[*(sp + 1) >> gamma_shift][*sp];
                     png_composite_16(w, v, a, png_ptr->background_1.red);
                     if (optimize != 0)
                        w = v;
                     else
                        w = gamma_16_from_1[(w & 0xff) >> gamma_shift][w >> 8];
                     *sp = (png_byte)((w >> 8) & 0xff);
                     *(sp + 1) = (png_byte)(w & 0xff);
                     v = gamma_16_to_1[*(sp + 3) >> gamma_shift][*(sp + 2)];
                     png_composite_16(w, v, a, png_ptr->background_1.green);
                     if (optimize != 0)
                        w = v;
                     else
                        w = gamma_16_from_1[(w & 0xff) >> gamma_shift][w >> 8];
                     *(sp + 2) = (png_byte)((w >> 8) & 0xff);
                     *(sp + 3) = (png_byte)(w & 0xff);
                     v = gamma_16_to_1[*(sp + 5) >> gamma_shift][*(sp + 4)];
                     png_composite_16(w, v, a, png_ptr->background_1.blue);
                     if (optimize != 0)
                        w = v;
                     else
                        w = gamma_16_from_1[(w & 0xff) >> gamma_shift][w >> 8];
                     *(sp + 4) = (png_byte)((w >> 8) & 0xff);
                     *(sp + 5) = (png_byte)(w & 0xff);
                  }
               }
            }
            else
#endif
            {
               sp = row;
               for (i = 0; i < row_width; i++, sp += 8)
               {
                  png_uint_16 a = (png_uint_16)(((png_uint_16)(*(sp + 6))
                      << 8) + (png_uint_16)(*(sp + 7)));
                  if (a == 0)
                  {
                     *sp = (png_byte)((png_ptr->background.red >> 8) & 0xff);
                     *(sp + 1) = (png_byte)(png_ptr->background.red & 0xff);
                     *(sp + 2) = (png_byte)((png_ptr->background.green >> 8)
                             & 0xff);
                     *(sp + 3) = (png_byte)(png_ptr->background.green
                             & 0xff);
                     *(sp + 4) = (png_byte)((png_ptr->background.blue >> 8)
                             & 0xff);
                     *(sp + 5) = (png_byte)(png_ptr->background.blue & 0xff);
                  }
                  else if (a < 0xffff)
                  {
                     png_uint_16 v;
                     png_uint_16 r = (png_uint_16)(((*sp) << 8) + *(sp + 1));
                     png_uint_16 g = (png_uint_16)(((*(sp + 2)) << 8)
                         + *(sp + 3));
                     png_uint_16 b = (png_uint_16)(((*(sp + 4)) << 8)
                         + *(sp + 5));
                     png_composite_16(v, r, a, png_ptr->background.red);
                     *sp = (png_byte)((v >> 8) & 0xff);
                     *(sp + 1) = (png_byte)(v & 0xff);
                     png_composite_16(v, g, a, png_ptr->background.green);
                     *(sp + 2) = (png_byte)((v >> 8) & 0xff);
                     *(sp + 3) = (png_byte)(v & 0xff);
                     png_composite_16(v, b, a, png_ptr->background.blue);
                     *(sp + 4) = (png_byte)((v >> 8) & 0xff);
                     *(sp + 5) = (png_byte)(v & 0xff);
                  }
               }
            }
         }
         break;
      }
      default:
         break;
   }
}
}

// End of focal method
}

// Very small assertion helper and test harness
static int tests_run = 0;
static int tests_failed = 0;

#define EXPECT(cond, msg) do { \
    tests_run++; \
    if (!(cond)) { \
        std::cerr << "TEST FAILED: " << msg << "\n"; \
        ++tests_failed; \
    } else { \
        std::cout << "TEST PASSED: " << msg << "\n"; \
    } \
} while (0)


// Test 1: Gray (8-bit) - Ensure trans_color.gray is replaced by background.gray
void test_gray8_trans_to_background() {
    // Prepare row: 3 pixels, 1 byte per pixel (8-bit grayscale)
    png_uint_32 width = 3;
    png_row_info ri;
    ri.width = width;
    ri.color_type = PNG_COLOR_TYPE_GRAY;
    ri.bit_depth = 8;
    unsigned char row_buf[3] = {128, 50, 128};

    png_struct_ptr:
    png_struct png_ptr;
    png_ptr.flags = 0;
    png_ptr.trans_color.gray = 128;  // target to replace
    png_ptr.background.gray = 200;   // new value

    // Call the focal function
    png_do_compose(&ri, row_buf, &png_ptr);

    // Check results: first and third bytes should be background.gray (200)
    EXPECT(row_buf[0] == 200, "Gray8: trans pixel 0 replaced with background gray");
    EXPECT(row_buf[1] == 50,  "Gray8: non-trans pixel unchanged");
    EXPECT(row_buf[2] == 200, "Gray8: trans pixel 2 replaced with background gray");
}

// Test 2: RGB (8-bit) - Ensure trans_color.rgb is replaced by background.rgb
void test_rgb8_trans_to_background() {
    // 2 pixels, 3 bytes per pixel
    png_uint_32 width = 2;
    png_row_info ri;
    ri.width = width;
    ri.color_type = PNG_COLOR_TYPE_RGB;
    ri.bit_depth = 8;
    unsigned char row_buf[6] = {10,20,30, 40,50,60};

    png_struct png_ptr;
    png_ptr.flags = 0;
    png_ptr.trans_color.red = 10;
    png_ptr.trans_color.green = 20;
    png_ptr.trans_color.blue = 30;
    png_ptr.background.red = 100;
    png_ptr.background.green = 110;
    png_ptr.background.blue = 120;

    png_do_compose(&ri, row_buf, &png_ptr);

    // First pixel should be replaced by background colors
    EXPECT(row_buf[0] == 100, "RGB8: trans red replaced with background red");
    EXPECT(row_buf[1] == 110, "RGB8: trans green replaced with background green");
    EXPECT(row_buf[2] == 120, "RGB8: trans blue replaced with background blue");

    // Second pixel unchanged
    EXPECT(row_buf[3] == 40, "RGB8: non-trans red unchanged");
    EXPECT(row_buf[4] == 50, "RGB8: non-trans green unchanged");
    EXPECT(row_buf[5] == 60, "RGB8: non-trans blue unchanged");
}

// Test 3: Gray Alpha (8-bit) - alpha == 0 replaces gray with background.gray
void test_gray_alpha8_alpha_zero() {
    // 1 pixel, 2 bytes per pixel (gray, alpha)
    png_uint_32 width = 1;
    png_row_info ri;
    ri.width = width;
    ri.color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
    ri.bit_depth = 8;
    unsigned char row_buf[2] = {50, 0}; // alpha zero

    png_struct png_ptr;
    png_ptr.flags = 0;
    png_ptr.background.gray = 77;

    png_do_compose(&ri, row_buf, &png_ptr);

    // Gray should be replaced by background.gray
    EXPECT(row_buf[0] == 77, "GrayAlpha8: alpha=0 -> gray becomes background.gray");
    EXPECT(row_buf[1] == 0, "GrayAlpha8: alpha unchanged");
}

// Test 4: RGB Alpha (8-bit) - alpha == 0 replaces RGB with background RGB
void test_rgb_alpha8_alpha_zero() {
    // 1 pixel, 4 bytes per pixel (R,G,B,A)
    png_uint_32 width = 1;
    png_row_info ri;
    ri.width = width;
    ri.color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    ri.bit_depth = 8;
    unsigned char row_buf[4] = {20, 30, 40, 0}; // alpha 0

    png_struct png_ptr;
    png_ptr.flags = 0;
    png_ptr.background.red = 5;
    png_ptr.background.green = 6;
    png_ptr.background.blue = 7;

    png_do_compose(&ri, row_buf, &png_ptr);

    // RGB should be replaced by background colors; alpha remains unchanged
    EXPECT(row_buf[0] == 5,  "RGBAlpha8: alpha=0 -> red becomes background.red");
    EXPECT(row_buf[1] == 6,  "RGBAlpha8: alpha=0 -> green becomes background.green");
    EXPECT(row_buf[2] == 7,  "RGBAlpha8: alpha=0 -> blue becomes background.blue");
    EXPECT(row_buf[3] == 0,  "RGBAlpha8: alpha unchanged");
}

// Test 5: RGB Alpha (8-bit) - alpha == 0xff (fully opaque) should leave color unchanged
void test_rgb_alpha8_alpha_full_opaque_no_change() {
    // 1 pixel, 4 bytes per pixel
    png_uint_32 width = 1;
    png_row_info ri;
    ri.width = width;
    ri.color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    ri.bit_depth = 8;
    unsigned char row_buf[4] = {11, 22, 33, 0xFF}; // fully opaque, should not modify color

    png_struct png_ptr;
    png_ptr.flags = 0;
    png_ptr.background.red = 99;
    png_ptr.background.green = 101;
    png_ptr.background.blue = 103;

    // Save a copy of original to compare after compose
    unsigned char orig[4];
    std::memcpy(orig, row_buf, 4);

    png_do_compose(&ri, row_buf, &png_ptr);

    // Since alpha == 0xFF and no gamma path, the color should remain unchanged
    EXPECT(std::memcmp(row_buf, orig, 4) == 0, "RGBAlpha8: alpha=0xFF -> color unchanged");
}


// Simple test runner
int main() {
    std::cout << "Running focal method test suite for png_do_compose (minimal harness)\n";

    test_gray8_trans_to_background();
    test_rgb8_trans_to_background();
    test_gray_alpha8_alpha_zero();
    test_rgb_alpha8_alpha_zero();
    test_rgb_alpha8_alpha_full_opaque_no_change();

    std::cout << "Tests run: " << tests_run << ", Failures: " << tests_failed << "\n";

    // Return non-zero if any test failed
    return (tests_failed == 0) ? 0 : 1;
}