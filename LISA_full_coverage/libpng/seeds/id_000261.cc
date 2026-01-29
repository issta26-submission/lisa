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
//<ID> 261
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_const_charp version = (png_const_charp)"1.6.37";
    png_structp png_ptr = png_create_write_struct_2(version, (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_voidp mem_ptr = png_get_mem_ptr((png_const_structrp)png_ptr);
    double white_x = 0.0, white_y = 0.0;
    double red_x = 0.0, red_y = 0.0;
    double green_x = 0.0, green_y = 0.0;
    double blue_x = 0.0, blue_y = 0.0;
    double mastering_display_maximum_luminance = 0.0;
    double mastering_display_minimum_luminance = 0.0;
    png_uint_32 mdcv_flags = 0;
    int alpha_mode = 1;
    double output_gamma = 2.2;

    // step 2: Configure
    png_set_alpha_mode((png_structrp)png_ptr, alpha_mode, output_gamma);

    // step 3: Operate / Validate
    mdcv_flags = png_get_mDCV((png_const_structrp)png_ptr, (png_const_inforp)NULL,
                              &white_x, &white_y,
                              &red_x, &red_y,
                              &green_x, &green_y,
                              &blue_x, &blue_y,
                              &mastering_display_maximum_luminance, &mastering_display_minimum_luminance);
    (void)mdcv_flags;
    (void)white_x; (void)white_y;
    (void)red_x; (void)red_y;
    (void)green_x; (void)green_y;
    (void)blue_x; (void)blue_y;
    (void)mastering_display_maximum_luminance; (void)mastering_display_minimum_luminance;

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, mem_ptr);

    // API sequence test completed successfully
    return 66;
}