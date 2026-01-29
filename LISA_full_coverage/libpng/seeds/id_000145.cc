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
//<ID> 145
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpng_api_sequence() {
    png_structp png_ptr = png_create_write_struct_2("1.6.37", NULL, NULL, NULL, NULL, NULL, NULL);
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);

    png_set_IHDR(png_ptr, info_ptr, (png_uint_32)800, (png_uint_32)600, 8, 2, 0, 0, 0);

    png_color_16 background;
    background.index = (png_byte)1;
    background.red = (png_uint_16)300;
    background.green = (png_uint_16)400;
    background.blue = (png_uint_16)500;
    background.gray = (png_uint_16)128;
    png_set_bKGD(png_ptr, info_ptr, (png_const_color_16p)&background);

    png_bytepp rows = png_get_rows((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_uint_32 w = 0;
    png_uint_32 h = 0;
    int bit_depth = 0;
    int color_type = 0;
    int interlace_method = 0;
    int compression_method = 0;
    int filter_method = 0;
    png_get_IHDR((png_const_structrp)png_ptr, (png_const_inforp)info_ptr, &w, &h, &bit_depth, &color_type, &interlace_method, &compression_method, &filter_method);

    png_uint_32 mix = w ^ h ^ (png_uint_32)bit_depth ^ (png_uint_32)color_type ^ (png_uint_32)background.red ^ (png_uint_32)background.green ^ (png_uint_32)background.blue ^ (png_uint_32)interlace_method;
    (void)mix;
    (void)rows;

    void * tmp_buf = malloc((size_t)256);
    memset(tmp_buf, 0, (size_t)256);
    png_free((png_const_structrp)png_ptr, tmp_buf);

    return 66;
}