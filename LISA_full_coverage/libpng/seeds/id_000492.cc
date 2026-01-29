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
//<ID> 492
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_inforp info_ptr = (png_inforp)NULL;
    png_byte buf[4];
    buf[0] = 0x7F; buf[1] = 0xFF; buf[2] = 0xAA; buf[3] = 0x55;

    // step 2: Configure
    png_uint_32 val = ((png_uint_32)buf[0] << 24) | ((png_uint_32)buf[1] << 16) | ((png_uint_32)buf[2] << 8) | (png_uint_32)buf[3];
    int srgb_intent = (int)(val & 0x3u);
    png_set_sRGB((png_const_structrp)png_ptr, info_ptr, srgb_intent);
    png_set_expand_16((png_structrp)png_ptr);
    int alpha_mode = (int)((val >> 2) & 0x1u);
    png_set_alpha_mode((png_structrp)png_ptr, alpha_mode, 2.2);

    // step 3: Operate / Validate
    (void)png_get_io_state((png_const_structrp)png_ptr); /* exercise png_ptr */
    png_uint_32 width = 0;
    png_uint_32 height = 0;
    int bit_depth = 0;
    int color_type = 0;
    int interlace_method = 0;
    int compression_method = 0;
    int filter_method = 0;
    (void)png_get_IHDR((png_const_structrp)png_ptr, (png_const_inforp)info_ptr, &width, &height, &bit_depth, &color_type, &interlace_method, &compression_method, &filter_method);
    size_t rowbytes = png_get_rowbytes((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    (void)rowbytes;
    (void)color_type;

    // step 4: Cleanup (leave png_ptr allocated; do a harmless query to exercise memory pointer)
    (void)png_get_mem_ptr((png_const_structrp)png_ptr);

    // API sequence test completed successfully
    return 66;
}