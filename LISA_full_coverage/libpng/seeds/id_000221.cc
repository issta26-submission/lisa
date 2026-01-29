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
//<ID> 221
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Create basic libpng structs
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);

    // step 2: Configure various non-I/O behaviors (safe, do not trigger actual I/O)
    png_set_benign_errors((png_structrp)png_ptr, 1);
    png_set_compression_level((png_structrp)png_ptr, 6);
    png_set_compression_method((png_structrp)png_ptr, 8);
    png_set_compression_strategy((png_structrp)png_ptr, 0);
    png_set_compression_window_bits((png_structrp)png_ptr, 15);
    png_set_compression_buffer_size((png_structrp)png_ptr, (size_t)8192);
    png_set_text_compression_level((png_structrp)png_ptr, 6);
    png_set_text_compression_method((png_structrp)png_ptr, 1);
    png_set_text_compression_window_bits((png_structrp)png_ptr, 15);
    png_set_keep_unknown_chunks((png_structrp)png_ptr, 0, (png_const_bytep)NULL, 0);

    // Image/format transforms that do not require I/O callbacks
    png_set_sRGB((png_const_structrp)png_ptr, (png_inforp)info_ptr, 0);
    png_set_tRNS_to_alpha((png_structrp)png_ptr);
    png_set_strip_alpha((png_structrp)png_ptr);
    png_set_expand_gray_1_2_4_to_8((png_structrp)png_ptr);
    png_set_palette_to_rgb((png_structrp)png_ptr);

    // step 3: Query some safe getters (do not require image data or I/O)
    png_const_charp ver = png_get_libpng_ver((png_const_structrp)png_ptr);
    png_const_charp header_ver = png_get_header_version((png_const_structrp)png_ptr);
    png_const_charp copy = png_get_copyright((png_const_structrp)png_ptr);
    png_uint_32 cur_row = png_get_current_row_number((png_const_structrp)png_ptr);
    png_uint_32 img_w = png_get_image_width((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    size_t rowbytes = png_get_rowbytes((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_byte bit_depth = png_get_bit_depth((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_byte interlace = png_get_interlace_type((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_const_bytep sig = png_get_signature((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    float aspect = png_get_pixel_aspect_ratio((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_uint_32 max_height = png_get_user_height_max((png_const_structrp)png_ptr);

    // silence unused-variable warnings
    (void)ver; (void)header_ver; (void)copy;
    (void)cur_row; (void)img_w; (void)rowbytes;
    (void)bit_depth; (void)interlace; (void)sig;
    (void)aspect; (void)max_height;

    // step 4: Return a fixed code (no explicit destroy calls used)
    return 66;
}