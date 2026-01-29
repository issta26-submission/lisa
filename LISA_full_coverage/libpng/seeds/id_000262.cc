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
//<ID> 262
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_voidp mem_before = png_get_mem_ptr((png_const_structrp)png_ptr);
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
    png_set_alpha_mode((png_structrp)png_ptr, 1, 2.2);

    // step 3: Operate / Validate
    png_uint_32 mdcv_result = png_get_mDCV((png_const_structrp)png_ptr, (png_const_inforp)NULL,
                                          &white_x, &white_y,
                                          &red_x, &red_y,
                                          &green_x, &green_y,
                                          &blue_x, &blue_y,
                                          &mastering_display_maximum_luminance,
                                          &mastering_display_minimum_luminance);
    png_voidp mem_after = png_get_mem_ptr((png_const_structrp)png_ptr);
    png_uint_32 combined = mdcv_result + (png_uint_32)(white_x + white_y + red_x + red_y + green_x + green_y + blue_x + blue_y + mastering_display_maximum_luminance + mastering_display_minimum_luminance);
    (void)combined;
    (void)mem_before;
    (void)mem_after;

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, (png_voidp)mem_after);

    // API sequence test completed successfully
    return 66;
}