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
//<ID> 166
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = (png_info *)info_ptr;

    // step 2: Configure
    png_uint_32 width = 320;
    png_uint_32 height = 240;
    int bit_depth = 8;
    int color_type = 2;
    int interlace_method = 0;
    int compression_method = 0;
    int filter_method = 0;
    png_set_IHDR((png_const_structrp)png_ptr, info_ptr, width, height, bit_depth, color_type, interlace_method, compression_method, filter_method);
    png_voidp io_buffer = png_malloc_warn((png_const_structrp)png_ptr, (png_alloc_size_t)256);
    memset(io_buffer, 0, (size_t)256);
    png_set_write_fn((png_structrp)png_ptr, io_buffer, (png_rw_ptr)NULL, (png_flush_ptr)NULL);
    png_set_alpha_mode((png_structrp)png_ptr, 1, 2.200000);

    // step 3: Operate / Validate
    png_uint_32 user_hmax = png_get_user_height_max((png_const_structrp)png_ptr);
    png_int_32 x_off = png_get_x_offset_microns((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_uint_32 mix = user_hmax ^ (png_uint_32)x_off ^ width ^ height ^ (png_uint_32)bit_depth;
    ((unsigned char *)io_buffer)[0] = (unsigned char)(mix & 0xFF);
    ((unsigned char *)io_buffer)[1] = (unsigned char)((mix >> 8) & 0xFF);
    ((unsigned char *)io_buffer)[2] = (unsigned char)((mix >> 16) & 0xFF);

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, io_buffer);
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}