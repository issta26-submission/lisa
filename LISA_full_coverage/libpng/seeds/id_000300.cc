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
//<ID> 300
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Initialize
    void *user_mem = malloc(128);
    memset(user_mem, 0x5A, 128);
    png_structp png_ptr = png_create_write_struct_2((png_const_charp)"1.6.37",
                                                    user_mem,
                                                    (png_error_ptr)NULL,
                                                    (png_error_ptr)NULL,
                                                    (png_voidp)NULL,
                                                    (png_malloc_ptr)NULL,
                                                    (png_free_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);

    // step 2: Configure
    png_color_16 background;
    background.index = 0;
    background.red = 100;
    background.green = 150;
    background.blue = 200;
    background.gray = 50;
    png_set_background_fixed((png_structrp)png_ptr, (png_const_color_16p)&background, 1, 0, (png_fixed_point)2200);
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);

    // step 3: Operate / Validate
    png_voidp buffer = malloc((size_t)chunk_max + 16);
    memset(buffer, 0xAA, (size_t)chunk_max + 16);
    png_int_32 x_offset_microns = png_get_x_offset_microns((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    size_t derived_check = (size_t)((png_int_32)(x_offset_microns) + (png_int_32)((png_int_32)chunk_max & 0x7FFFFFFF));
    (void)derived_check;

    // step 4: Cleanup
    free(buffer);
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);
    free(user_mem);

    // API sequence test completed successfully
    return 66;
}