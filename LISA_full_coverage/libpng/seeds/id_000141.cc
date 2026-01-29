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
//<ID> 141
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

    png_uint_32 width = (png_uint_32)8;
    png_uint_32 height = (png_uint_32)2;
    int bit_depth = 8;
    int color_type = 2; // RGB
    int interlace_method = 0;
    int compression_method = 0;
    int filter_method = 0;

    // step 2: Configure
    png_set_IHDR((png_const_structrp)png_ptr, info_ptr, width, height, bit_depth, color_type, interlace_method, compression_method, filter_method);

    png_color_16 background;
    background.index = (png_byte)1;
    background.red = (png_uint_16)1000;
    background.green = (png_uint_16)2000;
    background.blue = (png_uint_16)3000;
    background.gray = (png_uint_16)4000;
    png_set_bKGD((png_const_structrp)png_ptr, info_ptr, (png_const_color_16p)&background);

    png_uint_32 row_bytes = width * 3u; // width * channels (RGB)
    png_bytepp rows = (png_bytepp)malloc((size_t)height * sizeof(png_bytep));
    png_bytep row0 = (png_bytep)malloc((size_t)row_bytes);
    png_bytep row1 = (png_bytep)malloc((size_t)row_bytes);
    memset(row0, 0x11, (size_t)row_bytes);
    memset(row1, 0x22, (size_t)row_bytes);
    rows[0] = row0;
    rows[1] = row1;
    png_set_rows((png_structrp)png_ptr, info_ptr, rows);

    // step 3: Operate / Validate
    png_uint_32 out_width = 0;
    png_uint_32 out_height = 0;
    int out_bit_depth = 0;
    int out_color_type = 0;
    int out_interlace = 0;
    int out_compression = 0;
    int out_filter = 0;
    png_get_IHDR((png_const_structrp)png_ptr, (png_const_inforp)info_ptr, &out_width, &out_height, &out_bit_depth, &out_color_type, &out_interlace, &out_compression, &out_filter);

    png_bytepp fetched_rows = png_get_rows((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_byte first_byte = fetched_rows[0][0];
    png_uint_32 checksum = out_width ^ out_height ^ (png_uint_32)out_bit_depth ^ (png_uint_32)first_byte ^ (png_uint_32)background.red;
    (void)checksum;

    // step 4: Cleanup
    free(row1);
    free(row0);
    free(rows);
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    return 66;
    // API sequence test completed successfully
}