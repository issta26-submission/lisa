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
//<ID> 400
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct_2(
        (png_const_charp)"1.6.37",
        (png_voidp)NULL,
        (png_error_ptr)NULL,
        (png_error_ptr)NULL,
        (png_voidp)NULL,
        (png_malloc_ptr)NULL,
        (png_free_ptr)NULL
    );
    png_inforp info_ptr = (png_inforp)NULL;

    // step 2: Configure
    png_set_option((png_structrp)png_ptr, 1, 1);
    png_fixed_point int_white_x = (png_fixed_point)31270;
    png_fixed_point int_white_y = (png_fixed_point)32900;
    png_fixed_point int_red_x = (png_fixed_point)64000;
    png_fixed_point int_red_y = (png_fixed_point)33000;
    png_fixed_point int_green_x = (png_fixed_point)30000;
    png_fixed_point int_green_y = (png_fixed_point)60000;
    png_fixed_point int_blue_x = (png_fixed_point)15000;
    png_fixed_point int_blue_y = (png_fixed_point)6000;
    png_uint_32 max_luminance = (png_uint_32)1000000u; /* scaled by 10000 */
    png_uint_32 min_luminance = (png_uint_32)100u;
    png_set_mDCV_fixed(
        (png_const_structrp)png_ptr,
        info_ptr,
        int_white_x, int_white_y,
        int_red_x, int_red_y,
        int_green_x, int_green_y,
        int_blue_x, int_blue_y,
        max_luminance, min_luminance
    );

    // step 3: Operate / Validate
    png_byte current_pass = png_get_current_pass_number((png_const_structrp)png_ptr);
    png_uint_32 img_width = png_get_image_width((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    size_t alloc_size = (size_t)(img_width + 64u);
    png_voidp buffer = png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)alloc_size);
    memset(buffer, 0xA5, alloc_size);
    unsigned char *bytes = (unsigned char *)buffer;
    bytes[0] = (unsigned char)current_pass;
    bytes[1] = (unsigned char)(img_width & 0xFFu);
    bytes[2] = (unsigned char)((img_width >> 8) & 0xFFu);

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, buffer);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    return 66;
    // API sequence test completed successfully
}