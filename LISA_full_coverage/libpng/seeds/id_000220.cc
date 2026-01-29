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
//<ID> 220
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // Use opaque pointers (forward-declared types); do not dereference them.
    png_structrp png_ptr = nullptr;
    png_inforp info_ptr = nullptr;

    // Prepare fixed-point chromaticity values (png_fixed_point is an int typedef).
    png_fixed_point int_white_x  = 31270; // example value (1.0 in 1/100000 units)
    png_fixed_point int_white_y  = 32900;
    png_fixed_point int_red_x    = 64000;
    png_fixed_point int_red_y    = 33000;
    png_fixed_point int_green_x  = 30000;
    png_fixed_point int_green_y  = 60000;
    png_fixed_point int_blue_x   = 15000;
    png_fixed_point int_blue_y   = 6000;

    // Set cHRM (chromaticity) values in fixed-point form.
    // Note: first parameter is const png_structrp according to the requested API.
    png_set_cHRM_fixed((png_const_structrp)png_ptr, info_ptr,
                       int_white_x, int_white_y,
                       int_red_x, int_red_y,
                       int_green_x, int_green_y,
                       int_blue_x, int_blue_y);

    // Toggle byte swapping for png data (no-op with a nullptr png_ptr, but valid call).
    png_set_swap(png_ptr);

    // Set sample physical scale (sCAL) with unit and dimensions.
    int unit = 1; // e.g., meters
    double width = 1920.0;
    double height = 1080.0;
    png_set_sCAL((png_const_structrp)png_ptr, info_ptr, unit, width, height);

    // Retrieve sCAL values into local variables.
    int retrieved_unit = 0;
    double retrieved_width = 0.0;
    double retrieved_height = 0.0;
    png_uint_32 sCAL_return = png_get_sCAL((png_const_structrp)png_ptr,
                                           (png_const_inforp)info_ptr,
                                           &retrieved_unit,
                                           &retrieved_width,
                                           &retrieved_height);
    (void)sCAL_return; // silence unused variable warning

    // Add an alpha channel with a filler value (0xFF) and no special flags.
    png_uint_32 filler = 0xFFu;
    int flags = 0;
    png_set_add_alpha(png_ptr, filler, flags);

    return 66;
}