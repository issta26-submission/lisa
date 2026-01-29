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
//<ID> 306
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct_2((png_const_charp)"1.6.37",
                                                    (png_voidp)NULL,
                                                    (png_error_ptr)NULL,
                                                    (png_error_ptr)NULL,
                                                    (png_voidp)NULL,
                                                    (png_malloc_ptr)NULL,
                                                    (png_free_ptr)NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);

    // step 2: Configure
    png_color_16 background;
    memset(&background, 0, sizeof(background));
    background.red = (png_uint_16)0x8000;
    background.green = (png_uint_16)0x4000;
    background.blue = (png_uint_16)0x2000;
    background.gray = (png_uint_16)0x1000;
    background.index = (png_byte)1;
    png_set_background_fixed(png_ptr, (png_const_color_16p)&background, 1, 0, (png_fixed_point)12345);

    // step 3: Operate / Validate
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max(png_ptr);
    png_int_32 x_microns = png_get_x_offset_microns(png_ptr, info_ptr);

    // step 4: Cleanup
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);

    (void)chunk_max;
    (void)x_microns;

    // API sequence test completed successfully
    return 66;
}