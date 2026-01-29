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
//<ID> 461
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);

    // small local buffers for integer save/load helpers
    unsigned char buf16[2];
    unsigned char buf32[4];
    // initialize buffers
    buf16[0] = 0;
    buf16[1] = 0;
    buf32[0] = 0;
    buf32[1] = 0;
    buf32[2] = 0;
    buf32[3] = 0;

    // step 2: Configure various options
    png_set_strip_alpha((png_structrp)png_ptr);
    png_set_sig_bytes((png_structrp)png_ptr, 0);
    png_set_user_limits((png_structrp)png_ptr, (png_uint_32)4096, (png_uint_32)4096);
    png_set_compression_buffer_size((png_structrp)png_ptr, (size_t)1024);

    // adjust chunk malloc max using current value
    png_alloc_size_t current_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);
    png_set_chunk_malloc_max((png_structrp)png_ptr, (png_alloc_size_t)(current_max > 0 ? current_max : 1));

    // step 3: Operate / Validate (use integer helpers)
    png_save_uint_16((png_bytep)buf16, (unsigned int)0xABCDu);
    png_save_int_32((png_bytep)buf32, (png_int_32)-12345678);

    png_uint_16 got16 = png_get_uint_16((png_const_bytep)buf16);
    png_int_32 got32 = png_get_int_32((png_const_bytep)buf32);

    // query some metadata-related getters (no effect but exercises APIs)
    png_uint_32 img_height = png_get_image_height((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_byte channels = png_get_channels((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // avoid unused-variable warnings
    (void)got16;
    (void)got32;
    (void)img_height;
    (void)channels;
    (void)current_max;

    return 66;
}