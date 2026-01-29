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
//<ID> 454
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpng_api_sequence() {
    // step 1: Create structures
    png_structp png_ptr = png_create_write_struct_2(
        (png_const_charp)"1.6.37",
        (png_voidp)NULL,
        (png_error_ptr)NULL,
        (png_error_ptr)NULL,
        (png_voidp)NULL,
        (png_malloc_ptr)NULL,
        (png_free_ptr)NULL
    );
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);

    // small local buffer
    unsigned char buffer[16];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Prepare and set mastering display color volume (mDCV)
    double white_x = 0.3127, white_y = 0.3290;
    double red_x = 0.64, red_y = 0.33;
    double green_x = 0.3, green_y = 0.6;
    double blue_x = 0.15, blue_y = 0.06;
    double mastering_display_maximum_luminance = 100.0;
    double mastering_display_minimum_luminance = 0.05;

    png_set_mDCV(
        (png_const_structrp)png_ptr,
        (png_inforp)info_ptr,
        white_x, white_y,
        red_x, red_y,
        green_x, green_y,
        blue_x, blue_y,
        mastering_display_maximum_luminance,
        mastering_display_minimum_luminance
    );

    // step 3: Query and adjust chunk cache limits and some write parameters
    png_uint_32 cache_max = png_get_chunk_cache_max((png_const_structrp)png_ptr);
    png_set_chunk_cache_max((png_structrp)png_ptr, cache_max + 1024u);
    png_set_chunk_malloc_max((png_structrp)png_ptr, 2048);

    png_set_sig_bytes((png_structrp)png_ptr, 0);
    png_set_filter((png_structrp)png_ptr, 0, 0);
    png_set_compression_window_bits((png_structrp)png_ptr, 15);
    png_set_flush((png_structrp)png_ptr, 1);
    png_write_flush((png_structrp)png_ptr);

    // step 4: Cleanup
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);

    return 66;
}