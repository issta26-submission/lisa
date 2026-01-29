#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 144
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Properly create and initialize libpng structures
    png_struct *png_ptr = png_create_write_struct_2(NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    png_info *info_ptr = png_create_info_struct(png_ptr);

    // step 2: Prepare a background color structure
    png_color_16 background;
    memset(&background, 0, sizeof(background));
    background.index = (png_byte)2;
    background.red = (png_uint_16)1024;
    background.green = (png_uint_16)2048;
    background.blue = (png_uint_16)4096;
    background.gray = (png_uint_16)512;

    // step 3: Configure IHDR and background (keep original logic)
    png_set_IHDR((png_const_structrp)png_ptr, info_ptr,
                 (png_uint_32)800, (png_uint_32)600,
                 8, /* bit_depth */
                 2, /* color_type */
                 0, /* interlace_method */
                 0, /* compression_method */
                 0  /* filter_method */);
    png_set_bKGD((png_const_structrp)png_ptr, info_ptr, (png_const_color_16p)&background);

    // step 4: Operate / Validate
    png_bytepp rows = png_get_rows((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_uint_32 got_width = 0;
    png_uint_32 got_height = 0;
    int got_bit_depth = 0;
    int got_color_type = 0;
    int got_interlace = 0;
    int got_compression = 0;
    int got_filter = 0;
    png_uint_32 ihdr_flags = png_get_IHDR((png_const_structrp)png_ptr, (png_const_inforp)info_ptr,
                                          &got_width, &got_height,
                                          &got_bit_depth, &got_color_type,
                                          &got_interlace, &got_compression, &got_filter);
    (void)rows;
    png_uint_32 mix = ihdr_flags ^ got_width ^ got_height ^ (png_uint_32)background.red ^ (png_uint_32)got_bit_depth ^ (png_uint_32)got_color_type;
    (void)mix;

    // step 5: Cleanup
    png_destroy_info_struct((png_const_structrp)png_ptr, &info_ptr);
    png_free((png_const_structrp)png_ptr, (png_voidp)png_ptr);

    return 66;
}