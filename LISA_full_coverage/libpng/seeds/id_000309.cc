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
//<ID> 309
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    void *user_mem = malloc(128);
    memset(user_mem, 0xA5, 128);
    png_structp png_ptr = png_create_write_struct_2((png_const_charp)"1.6.37",
                                                   user_mem,
                                                   (png_error_ptr)NULL,
                                                   (png_error_ptr)NULL,
                                                   (png_voidp)NULL,
                                                   (png_malloc_ptr)NULL,
                                                   (png_free_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);

    // step 2: Configure
    png_color_16 bg;
    bg.index = 0;
    bg.red = (png_uint_16)40000;
    bg.green = (png_uint_16)30000;
    bg.blue = (png_uint_16)20000;
    bg.gray = (png_uint_16)25000;
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);
    png_set_background_fixed((png_structrp)png_ptr, (png_const_color_16p)&bg, 1, 1, (png_fixed_point)1000);

    // step 3: Operate / Validate
    png_alloc_size_t buf_size = chunk_max + (png_alloc_size_t)256;
    if (buf_size == 0) buf_size = (png_alloc_size_t)256; /* harmless safeguard without branching logic in API usage */
    png_voidp buffer = png_malloc((png_const_structrp)png_ptr, buf_size);
    memset(buffer, 0x5A, (size_t)buf_size);
    png_int_32 x_microns = png_get_x_offset_microns((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    ((unsigned char *)buffer)[0] = (unsigned char)(x_microns & 0xFF);

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, buffer);
    free(user_mem);
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);

    // API sequence test completed successfully
    (void)chunk_max;
    (void)x_microns;
    return 66;
}