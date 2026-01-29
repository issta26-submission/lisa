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
//<ID> 10
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_struct *png_ptr = png_create_write_struct((const char *)"libpng", nullptr, nullptr, nullptr);
    png_info *info_ptr = nullptr;
    png_uint_32 version = png_access_version_number();
    png_fixed_point int_file_gamma = (png_fixed_point)22000;
    png_uint_32 x_pixels_per_meter = 0;
    int opt_result1 = 0;
    int opt_result2 = 0;

    // step 2: Configure libpng behavior
    opt_result1 = png_set_option(png_ptr, (int)(version & 0x3U), 1);
    png_set_bgr(png_ptr);
    png_set_gAMA_fixed(png_ptr, info_ptr, int_file_gamma);

    // step 3: Operate / Query
    x_pixels_per_meter = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    opt_result2 = png_set_option(png_ptr, (int)(x_pixels_per_meter & 0x7U), (int)((x_pixels_per_meter >> 3) & 1U));

    // step 4: Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);

    // API sequence test completed successfully
    (void)opt_result1;
    (void)opt_result2;
    (void)x_pixels_per_meter;
    (void)version;
    return 66;
}