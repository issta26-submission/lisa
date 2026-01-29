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
//<ID> 305
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    void *user_mem = malloc((size_t)128);
    memset(user_mem, 0x5A, (size_t)128);
    png_structp png_ptr = png_create_write_struct_2((png_const_charp)"1.6.37",
                                                   (png_voidp)user_mem,
                                                   (png_error_ptr)NULL,
                                                   (png_error_ptr)NULL,
                                                   (png_voidp)user_mem,
                                                   (png_malloc_ptr)NULL,
                                                   (png_free_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);

    // step 2: Configure
    png_set_compression_mem_level((png_structrp)png_ptr, 8);
    png_color_16 background;
    background.index = (png_byte)0;
    background.red = (png_uint_16)100;
    background.green = (png_uint_16)150;
    background.blue = (png_uint_16)200;
    background.gray = (png_uint_16)0;
    png_set_background_fixed((png_structrp)png_ptr, (png_const_color_16p)&background, 1, 0, (png_fixed_point)0);
    png_set_IHDR((png_const_structrp)png_ptr, info_ptr, (png_uint_32)640, (png_uint_32)480, 8, 2, 0, 0, 0);

    // step 3: Operate / Validate
    png_alloc_size_t max_chunk = png_get_chunk_malloc_max((png_const_structrp)png_ptr);
    size_t buf_size = (size_t)max_chunk + 1U;
    png_voidp buffer = png_malloc((png_const_structrp)png_ptr, (png_alloc_size_t)buf_size);
    memset(buffer, 0xA5, buf_size);
    png_int_32 x_offset_microns = png_get_x_offset_microns((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    (void)x_offset_microns;

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, buffer);
    free(user_mem);
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);

    // API sequence test completed successfully
    return 66;
}