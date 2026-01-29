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
//<ID> 499
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct("1.6.37", NULL, NULL, NULL);
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_uint_32 parsed_val = 0;
    unsigned char buf[4] = { 0x12, 0x34, 0x56, 0x78 };

    // step 2: Configure
    parsed_val = png_get_uint_31((png_const_structrp)png_ptr, (png_const_bytep)buf);
    int srgb_intent = (int)(parsed_val & 0x3u);
    png_set_sRGB((png_const_structrp)png_ptr, info_ptr, srgb_intent);

    // step 3: Operate / Validate
    png_set_strip_alpha((png_structrp)png_ptr);
    int alpha_mode = (int)((parsed_val >> 2) & 0x3u);
    double output_gamma = (double)(parsed_val & 0xFFu) / 10.0;
    png_set_add_alpha((png_structrp)png_ptr, (png_uint_32)(parsed_val & 0xFFu), alpha_mode);

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    (void)output_gamma; // suppress unused-variable warnings without changing logic
    return 66;
}