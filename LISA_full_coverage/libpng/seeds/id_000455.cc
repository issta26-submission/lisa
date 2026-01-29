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
//<ID> 455
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
    png_bytep buffer = (png_bytep)png_calloc((png_const_structrp)png_ptr, (png_alloc_size_t)64);
    memset(buffer, 0, (size_t)64);
    double white_x = 0.0, white_y = 0.0;
    double red_x = 0.0, red_y = 0.0;
    double green_x = 0.0, green_y = 0.0;
    double blue_x = 0.0, blue_y = 0.0;
    double mastering_max = 0.0, mastering_min = 0.0;
    int alpha_mode = 1;
    double output_gamma = 2.2;

    // step 2: Configure
    png_set_IHDR((png_const_structrp)png_ptr, (png_inforp)info_ptr, (png_uint_32)800U, (png_uint_32)600U, 8, 6, 0, 0, 0);
    png_set_alpha_mode((png_structrp)png_ptr, alpha_mode, output_gamma);

    // step 3: Operate / Validate
    png_const_charp ver = png_get_libpng_ver((png_const_structrp)png_ptr);
    png_uint_32 mdcv_result = png_get_mDCV((png_const_structrp)png_ptr, (png_const_inforp)info_ptr, &white_x, &white_y, &red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y, &mastering_max, &mastering_min);
    png_save_uint_32((png_bytep)buffer, (png_int_32)mdcv_result);
    png_save_int_32((png_bytep)(buffer + 4), (png_int_32)(int)(mastering_max));

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, (png_voidp)buffer);
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);

    // API sequence test completed successfully
    (void)ver;
    (void)white_x;
    (void)white_y;
    (void)red_x;
    (void)red_y;
    (void)green_x;
    (void)green_y;
    (void)blue_x;
    (void)blue_y;
    (void)mastering_min;
    return 66;
}