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
//<ID> 264
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    unsigned char mem_area[16];
    memset(mem_area, 0x5A, sizeof(mem_area));
    png_structp png_ptr = png_create_write_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)mem_area, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_voidp retrieved_mem = png_get_mem_ptr((png_const_structrp)png_ptr);

    // step 2: Configure
    int computed_mode = ((unsigned char *)retrieved_mem)[0] & 1;
    double computed_gamma = 1.0 + (((unsigned char *)retrieved_mem)[1] / 255.0) * 2.2;
    png_set_alpha_mode((png_structrp)png_ptr, computed_mode, computed_gamma);

    // step 3: Operate / Validate
    png_const_inforp info_ptr = (png_const_inforp)NULL;
    double white_x = 0.0, white_y = 0.0;
    double red_x = 0.0, red_y = 0.0;
    double green_x = 0.0, green_y = 0.0;
    double blue_x = 0.0, blue_y = 0.0;
    double mastering_display_maximum_luminance = 0.0, mastering_display_minimum_luminance = 0.0;
    png_uint_32 mdcv_flags = png_get_mDCV((png_const_structrp)png_ptr, info_ptr,
                                         &white_x, &white_y,
                                         &red_x, &red_y,
                                         &green_x, &green_y,
                                         &blue_x, &blue_y,
                                         &mastering_display_maximum_luminance,
                                         &mastering_display_minimum_luminance);
    (void)mdcv_flags;
    (void)white_x; (void)white_y; (void)red_x; (void)red_y;
    (void)green_x; (void)green_y; (void)blue_x; (void)blue_y;
    (void)mastering_display_maximum_luminance; (void)mastering_display_minimum_luminance;

    // step 4: Cleanup
    (void)png_ptr;
    // API sequence test completed successfully
    return 66;
}