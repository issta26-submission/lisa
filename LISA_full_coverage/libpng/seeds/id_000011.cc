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
//<ID> 11
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_const_charp verstr = png_get_libpng_ver((png_const_structrp)png_ptr);
    png_fixed_point file_gamma = (png_fixed_point)22222;

    // step 2: Configure
    png_set_option((png_structrp)png_ptr, 0, 1);
    png_set_gamma_fixed((png_structrp)png_ptr, file_gamma, file_gamma);

    // step 3: Operate / Validate-like use of values
    png_uint_32 x_pixels_per_meter = png_get_x_pixels_per_meter((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    int opt_flag = (int)(((unsigned char)verstr[0]) & 1u);
    png_set_option((png_structrp)png_ptr, 1, opt_flag);
    (void)x_pixels_per_meter;
    (void)verstr;

    // step 4: Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);

    // API sequence test completed successfully
    return 66;
}