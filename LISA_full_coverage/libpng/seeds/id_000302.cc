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
//<ID> 302
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    void *user_mem = malloc((size_t)256);
    memset(user_mem, 0x5A, (size_t)256);
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37",
                                                   (png_voidp)NULL,
                                                   (png_error_ptr)NULL,
                                                   (png_error_ptr)NULL,
                                                   user_mem,
                                                   (png_malloc_ptr)NULL,
                                                   (png_free_ptr)NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);

    png_color_16 background;
    background.index = (png_byte)0;
    background.red = (png_uint_16)0x8000;
    background.green = (png_uint_16)0x8000;
    background.blue = (png_uint_16)0x8000;
    background.gray = (png_uint_16)0x8000;
    png_set_background_fixed(png_ptr, (png_const_color_16p)&background, 1, 0, (png_fixed_point)0);

    png_uint_32 max_h = png_get_user_height_max(png_ptr);
    size_t alloc_size = (size_t)max_h + (size_t)16;
    void *buffer = malloc(alloc_size);
    memset(buffer, (int)(max_h & 0xFF), alloc_size);

    free(buffer);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);
    free(user_mem);

    return 66;
}