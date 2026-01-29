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
//<ID> 225
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Create and initialize a libpng read struct
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);

    // step 2: Configure a variety of options (straight-line sequence, no branches)
    png_set_error_fn((png_structrp)png_ptr, (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_set_bgr((png_structrp)png_ptr);
    png_set_strip_16((png_structrp)png_ptr);
    png_set_invert_alpha((png_structrp)png_ptr);
    png_set_packswap((png_structrp)png_ptr);
    png_set_swap_alpha((png_structrp)png_ptr);
    png_set_flush((png_structrp)png_ptr, 1);
    png_set_compression_level((png_structrp)png_ptr, 6);
    png_set_text_compression_level((png_structrp)png_ptr, 6);
    png_set_text_compression_mem_level((png_structrp)png_ptr, 8);

    // step 3: Query some read-only information
    size_t comp_buf = png_get_compression_buffer_size((png_const_structrp)png_ptr);
    png_uint_32 cache_max = png_get_chunk_cache_max((png_const_structrp)png_ptr);
    (void)comp_buf; // keep usage straightforward; no branching allowed
    (void)cache_max;

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)NULL, (png_infopp)NULL);

    // return a fixed value (preserving the original function signature and straight-line flow)
    return 66;
}