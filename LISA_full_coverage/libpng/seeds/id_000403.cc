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
//<ID> 403
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
    png_voidp alloc_ptr = (png_voidp)NULL;
    png_uint_32 width = (png_uint_32)0;
    png_byte pass = (png_byte)0;
    int option_ret = 0;

    // step 2: Configure
    option_ret = png_set_option((png_structrp)png_ptr, 1, 1);

    // step 3: Operate / Validate
    width = png_get_image_width((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    pass = png_get_current_pass_number((png_const_structrp)png_ptr);
    png_fixed_point white_x = (png_fixed_point)(width & 0x7FFFFFFF);
    png_fixed_point white_y = (png_fixed_point)pass;
    png_fixed_point red_x = (png_fixed_point)(white_x + 1);
    png_fixed_point red_y = (png_fixed_point)(white_y + 1);
    png_fixed_point green_x = (png_fixed_point)(red_x + 1);
    png_fixed_point green_y = (png_fixed_point)(red_y + 1);
    png_fixed_point blue_x = (png_fixed_point)(green_x + 1);
    png_fixed_point blue_y = (png_fixed_point)(green_y + 1);
    png_uint_32 max_lum = (png_uint_32)(width + 100u);
    png_uint_32 min_lum = (png_uint_32)(pass + 1u);
    png_set_mDCV_fixed((png_const_structrp)png_ptr, (png_inforp)info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y, max_lum, min_lum);
    alloc_ptr = png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)64);
    memset(alloc_ptr, 0xA5, 64);

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, alloc_ptr);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}