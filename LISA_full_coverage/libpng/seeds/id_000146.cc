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
//<ID> 146
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = (png_info *)info_ptr;

    png_uint_32 set_width = (png_uint_32)64;
    png_uint_32 set_height = (png_uint_32)4;
    int set_bit_depth = 8;
    int set_color_type = 2; /* RGB */
    int set_interlace = 0;
    int set_compression = 0;
    int set_filter = 0;

    png_color_16 background;
    background.index = (png_byte)0;
    background.red = (png_uint_16)100;
    background.green = (png_uint_16)150;
    background.blue = (png_uint_16)200;
    background.gray = (png_uint_16)0;

    // step 2: Configure
    png_set_IHDR((png_const_structrp)png_ptr, info_ptr, set_width, set_height, set_bit_depth, set_color_type, set_interlace, set_compression, set_filter);
    png_set_bKGD((png_const_structrp)png_ptr, info_ptr, (png_const_color_16p)&background);

    // step 3: Operate / Validate
    png_uint_32 got = 0;
    png_uint_32 width_out = 0;
    png_uint_32 height_out = 0;
    int bit_depth_out = 0;
    int color_type_out = 0;
    int interlace_out = 0;
    int compression_out = 0;
    int filter_out = 0;
    got = png_get_IHDR((png_const_structrp)png_ptr, (png_const_inforp)info_ptr, &width_out, &height_out, &bit_depth_out, &color_type_out, &interlace_out, &compression_out, &filter_out);

    png_bytepp rows = png_get_rows((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);

    size_t row_bytes = (size_t)width_out * 3u;
    size_t total_bytes = row_bytes * (size_t)height_out;
    png_byte *data = (png_byte *)malloc(total_bytes ? total_bytes : 1);
    memset(data, 0xA5, total_bytes ? total_bytes : 1);

    png_bytepp myrows = (png_bytepp)malloc(sizeof(png_byte *) * 4);
    myrows[0] = data + 0 * row_bytes;
    myrows[1] = data + 1 * row_bytes;
    myrows[2] = data + 2 * row_bytes;
    myrows[3] = data + 3 * row_bytes;

    png_uint_32 mix = got;
    mix ^= width_out;
    mix ^= height_out;
    mix ^= (png_uint_32)bit_depth_out;
    mix ^= (png_uint_32)color_type_out;
    mix ^= (png_uint_32)myrows[0][0];
    mix ^= (png_uint_32)myrows[1][0];
    (void)mix;

    // step 4: Cleanup
    free(myrows);
    free(data);
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}