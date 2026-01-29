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
//<ID> 18
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = (png_infop)NULL;
    png_uint_32 version = png_access_version_number();
    png_uint_32 x_pixels_per_meter = 0;
    png_fixed_point gamma_value = (png_fixed_point)(version & 0xFFFF);

    // step 2: Configure libpng behavior
    png_set_option(png_ptr, (int)(version & 0x7F), 1);
    png_set_bgr(png_ptr);
    png_set_gAMA_fixed((png_const_structrp)png_ptr, info_ptr, gamma_value);

    // step 3: Operate / Query
    x_pixels_per_meter = png_get_x_pixels_per_meter((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    (void)x_pixels_per_meter;
    png_set_option(png_ptr, 0, 0);

    // step 4: Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return 66;
    // API sequence test completed successfully
}