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
//<ID> 404
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Initialize
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_inforp info_ptr = (png_inforp)NULL;
    png_voidp info_mem = (png_voidp)NULL;
    png_alloc_size_t alloc_size = (png_alloc_size_t)256;
    info_mem = png_malloc_default((png_const_structrp)png_ptr, alloc_size);
    info_ptr = (png_inforp)info_mem;
    memset(info_ptr, 0, (size_t)alloc_size);

    // step 2: Configure
    int opt_id = 1;
    png_set_option((png_structrp)png_ptr, opt_id, 1);
    png_fixed_point white_x = (png_fixed_point)31270;
    png_fixed_point white_y = (png_fixed_point)32900;
    png_fixed_point red_x = (png_fixed_point)64000;
    png_fixed_point red_y = (png_fixed_point)33000;
    png_fixed_point green_x = (png_fixed_point)30000;
    png_fixed_point green_y = (png_fixed_point)60000;
    png_fixed_point blue_x = (png_fixed_point)15000;
    png_fixed_point blue_y = (png_fixed_point)6000;
    png_uint_32 max_lum = (png_uint_32)1000000u;
    png_uint_32 min_lum = (png_uint_32)100u;
    png_set_mDCV_fixed((png_const_structrp)png_ptr, info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y, max_lum, min_lum);

    // step 3: Operate / Validate
    png_byte pass = png_get_current_pass_number((png_const_structrp)png_ptr);
    png_uint_32 width = png_get_image_width((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    (void)pass;
    (void)width;

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, info_mem);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)NULL, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}