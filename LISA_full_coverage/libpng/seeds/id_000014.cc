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
//<ID> 14
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structrp png_ptr = png_create_write_struct((png_const_charp)nullptr, (png_voidp)nullptr, (png_error_ptr)nullptr, (png_error_ptr)nullptr);
    png_inforp info_ptr = nullptr;
    png_uint_32 version = png_access_version_number();
    unsigned char *buffer = (unsigned char *)malloc(128);
    memset(buffer, 0xA5, 128);
    png_fixed_point fixed_gamma = (png_fixed_point)22000;
    int option_id = 0;
    int option_on = 1;
    png_uint_32 pixels_per_meter = 0;

    // step 2: Configure libpng behavior
    png_set_option(png_ptr, option_id, option_on);
    png_set_bgr(png_ptr);
    png_set_gAMA_fixed((png_const_structrp)png_ptr, info_ptr, fixed_gamma);

    // step 3: Operate / Validate
    pixels_per_meter = png_get_x_pixels_per_meter((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    (void)version;
    (void)pixels_per_meter;
    (void)buffer;

    // step 4: Cleanup
    free(buffer);
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);
    // API sequence test completed successfully
    return 66;
}