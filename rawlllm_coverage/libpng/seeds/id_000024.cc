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
//<ID> 147
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

    png_uint_32 width = 128;
    png_uint_32 height = 64;
    int bit_depth = 8;
    int color_type = 2;
    int interlace_method = 0;
    int compression_method = 0;
    int filter_method = 0;

    // step 2: Configure
    png_set_IHDR((png_const_structrp)png_ptr, info_ptr, width, height, bit_depth, color_type, interlace_method, compression_method, filter_method);

    png_color_16 background;
    memset(&background, 0, sizeof(background));
    background.index = (png_byte)1;
    background.red = (png_uint_16)400;
    background.green = (png_uint_16)300;
    background.blue = (png_uint_16)200;
    background.gray = (png_uint_16)100;
    png_set_bKGD((png_const_structrp)png_ptr, info_ptr, (png_const_color_16p)&background);

    // step 3: Operate / Validate
    png_uint_32 r_width = 0;
    png_uint_32 r_height = 0;
    int r_bit_depth = 0;
    int r_color_type = 0;
    int r_interlace = 0;
    int r_compression = 0;
    int r_filter = 0;
    png_get_IHDR((png_const_structrp)png_ptr, (png_const_inforp)info_ptr, &r_width, &r_height, &r_bit_depth, &r_color_type, &r_interlace, &r_compression, &r_filter);
    png_bytepp rows = png_get_rows((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_uint_32 check = width ^ height ^ r_width ^ r_height ^ (png_uint_32)r_bit_depth ^ (png_uint_32)r_color_type;
    (void)check;
    (void)rows;

    // step 4: Cleanup
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}