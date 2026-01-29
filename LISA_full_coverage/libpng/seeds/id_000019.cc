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
//<ID> 19
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Initialize / Declarations
    png_structrp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = (png_inforp)NULL;
    png_uint_32 version = png_access_version_number();
    png_fixed_point gamma_fixed = (png_fixed_point)22000;
    int option_flag = (int)(version & 0xFFu);

    // step 2: Configure libpng behavior
    png_set_option(png_ptr, option_flag, 1);
    png_set_bgr(png_ptr);
    png_set_gAMA_fixed((png_const_structrp)png_ptr, info_ptr, gamma_fixed);

    // step 3: Operate (query state and adjust options)
    png_uint_32 x_pixels_per_meter = png_get_x_pixels_per_meter((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_set_option(png_ptr, option_flag ^ (int)(x_pixels_per_meter & 0xFFu), 0);

    // step 4: Validate-like cleanup
    png_struct *png_ptr_nonrestrict = png_ptr;
    png_destroy_write_struct(&png_ptr_nonrestrict, (png_infopp)NULL);
    (void)version;
    (void)x_pixels_per_meter;

    // API sequence test completed successfully
    return 66;
}