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
//<ID> 450
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    unsigned char buffer[16];
    memset(buffer, 0, sizeof(buffer));
    double white_x = 0.0, white_y = 0.0;
    double red_x = 0.0, red_y = 0.0;
    double green_x = 0.0, green_y = 0.0;
    double blue_x = 0.0, blue_y = 0.0;
    double max_luminance = 0.0, min_luminance = 0.0;

    // step 2: Configure
    png_set_IHDR((png_const_structrp)png_ptr, (png_inforp)info_ptr, (png_uint_32)800U, (png_uint_32)600U, 8, 2, 0, 0, 0);
    png_set_mDCV_fixed((png_const_structrp)png_ptr, (png_inforp)info_ptr,
                       (png_fixed_point)31270, (png_fixed_point)32900,   /* white x,y */
                       (png_fixed_point)64000, (png_fixed_point)33000,   /* red x,y */
                       (png_fixed_point)30000, (png_fixed_point)60000,   /* green x,y */
                       (png_fixed_point)15000, (png_fixed_point)10000,   /* blue x,y */
                       (png_uint_32)100000U, (png_uint_32)100U);         /* max/min luminance scaled */
    png_set_alpha_mode((png_structrp)png_ptr, 1, 2.2);

    // step 3: Operate / Validate
    png_uint_32 mdcv_flags = png_get_mDCV((png_const_structrp)png_ptr, (png_const_inforp)info_ptr,
                                         &white_x, &white_y, &red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y, &max_luminance, &min_luminance);
    png_const_charp ver = png_get_libpng_ver((png_const_structrp)png_ptr);
    (void)ver;
    png_save_uint_32((png_bytep)buffer, mdcv_flags);

    // step 4: Cleanup
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);

    // API sequence test completed successfully
    return 66;
}