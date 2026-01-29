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
//<ID> 260
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_voidp mem_ptr = png_get_mem_ptr((png_const_structrp)png_ptr);
    double white_x = 0.0;
    double white_y = 0.0;
    double red_x = 0.0;
    double red_y = 0.0;
    double green_x = 0.0;
    double green_y = 0.0;
    double blue_x = 0.0;
    double blue_y = 0.0;
    double mastering_display_maximum_luminance = 0.0;
    double mastering_display_minimum_luminance = 0.0;

    // step 2: Configure
    png_set_mem_fn((png_structrp)png_ptr, mem_ptr, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_set_alpha_mode((png_structrp)png_ptr, 1, 2.2);

    // step 3: Operate / Validate
    png_uint_32 mdcv_flags = png_get_mDCV((png_const_structrp)png_ptr, (png_const_inforp)NULL,
                                         &white_x, &white_y,
                                         &red_x, &red_y,
                                         &green_x, &green_y,
                                         &blue_x, &blue_y,
                                         &mastering_display_maximum_luminance,
                                         &mastering_display_minimum_luminance);
    unsigned char temp_buf[4];
    png_save_int_32((png_bytep)temp_buf, (png_int_32)mdcv_flags);
    png_voidp scratch = png_malloc_warn((png_const_structrp)png_ptr, (png_alloc_size_t)16);
    memset(scratch, 0x5A, 16);
    png_free((png_const_structrp)png_ptr, scratch);

    // step 4: Cleanup
    png_free((png_const_structrp)png_ptr, mem_ptr);

    // API sequence test completed successfully
    (void)white_x; (void)white_y; (void)red_x; (void)red_y;
    (void)green_x; (void)green_y; (void)blue_x; (void)blue_y;
    (void)mastering_display_maximum_luminance; (void)mastering_display_minimum_luminance;
    (void)temp_buf; (void)mdcv_flags; (void)png_ptr;
    return 66;
}