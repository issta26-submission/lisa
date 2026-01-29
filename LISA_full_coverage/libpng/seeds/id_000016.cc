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
//<ID> 16
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpng_api_sequence() {
    // step 1: Initialize
    png_structrp png_ptr = nullptr;
    png_inforp info_ptr = nullptr;
    png_uint_32 version = png_access_version_number();
    unsigned char *row = (unsigned char *)malloc(4);
    memset(row, 0, 4);
    png_fixed_point gamma_fixed = (png_fixed_point)2200;
    png_ptr = png_create_write_struct("1.6.37", nullptr, nullptr, nullptr);

    // step 2: Configure
    png_set_bgr(png_ptr);
    png_set_gAMA_fixed(png_ptr, info_ptr, gamma_fixed);

    // step 3: Operate
    png_uint_32 pixels_per_meter = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    int option = (int)(pixels_per_meter & 0xFFu);
    int onoff = (int)(version & 1u);
    png_set_option(png_ptr, option, onoff);

    // step 4: Cleanup
    // Cast to the expected pointer-to-pointer types to respect the restrict qualifiers
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);
    free(row);
    // API sequence test completed successfully
    return 66;
}