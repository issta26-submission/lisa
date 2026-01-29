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
//<ID> 398
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_info *info_ptr = (png_info *)NULL;

    // step 2: Query some properties (read-only, no write callbacks required)
    png_uint_32 user_width_max = png_get_user_width_max((png_const_structrp)png_ptr);
    png_uint_32 chunk_cache_max = png_get_chunk_cache_max((png_const_structrp)png_ptr);
    png_const_charp header_ver = png_get_header_version((png_const_structrp)png_ptr);
    png_const_charp copyright = png_get_copyright((png_const_structrp)png_ptr);

    // step 3: Configure transformations that do not require write callbacks
    png_set_strip_alpha((png_structrp)png_ptr);
    png_set_gray_to_rgb((png_structrp)png_ptr);
    png_set_strip_16((png_structrp)png_ptr);
    png_set_bgr((png_structrp)png_ptr);
    png_set_invert_alpha((png_structrp)png_ptr);
    png_set_invert_mono((png_structrp)png_ptr);

    // step 4: Allocate and use memory via libpng alloc/free
    png_alloc_size_t alloc_size = (png_alloc_size_t)256;
    png_voidp buffer = png_malloc((png_const_structrp)png_ptr, alloc_size);

    // set memory functions (NULL pointers are passed as placeholders)
    png_set_mem_fn((png_structrp)png_ptr, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);

    // additional safe reads that accept a possibly-NULL info pointer
    png_byte bit_depth = png_get_bit_depth((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_byte channels = png_get_channels((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);

    // step 5: Cleanup allocated memory
    png_free((png_const_structrp)png_ptr, buffer);

    // silence unused variable warnings
    (void)info_ptr;
    (void)user_width_max;
    (void)chunk_cache_max;
    (void)header_ver;
    (void)copyright;
    (void)bit_depth;
    (void)channels;

    // API sequence test completed successfully
    return 66;
}