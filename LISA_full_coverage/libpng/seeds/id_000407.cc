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
//<ID> 407
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_info *info_ptr = (png_info *)NULL;

    // step 2: Configure
    int initial_option_result = png_set_option((png_structrp)png_ptr, 3, 1);
    png_set_mDCV_fixed((png_const_structrp)png_ptr, info_ptr,
                       (png_fixed_point)10000, (png_fixed_point)10000,   // white_x, white_y
                       (png_fixed_point)31270, (png_fixed_point)32900,   // red_x, red_y
                       (png_fixed_point)6400,  (png_fixed_point)3000,    // green_x, green_y
                       (png_fixed_point)15000, (png_fixed_point)6000,    // blue_x, blue_y
                       (png_uint_32)1000000u, (png_uint_32)0u);          // max/min luminance scaled

    // step 3: Operate / Validate
    png_byte pass = png_get_current_pass_number((png_const_structrp)png_ptr);
    png_uint_32 width = png_get_image_width((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    size_t alloc_size = (size_t)((unsigned)pass + 1u) * 64u;
    png_voidp mem = png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)alloc_size);
    int option_after_ops = png_set_option((png_structrp)png_ptr, (int)(width & 0x7FFFFFFFu), (int)pass);
    png_free_default((png_const_structrp)png_ptr, mem);

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    (void)initial_option_result;
    (void)option_after_ops;
    (void)width;
    (void)pass;
    return 66;
}