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
//<ID> 431
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize using a write struct so write callbacks are allowed
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_info *info_ptr = (png_info *)NULL;

    // step 2: Query some information
    png_const_charp version = png_get_header_version((png_const_structrp)png_ptr);
    png_uint_32 width_max = png_get_user_width_max((png_const_structrp)png_ptr);
    float y_offset_inches = png_get_y_offset_inches((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);

    // step 3: Compute a derived value and configure the png struct
    png_uint_32 derived_cache = width_max + (png_uint_32)(y_offset_inches >= 0.0f ? y_offset_inches : -y_offset_inches);
    png_set_chunk_malloc_max((png_structrp)png_ptr, (png_alloc_size_t)derived_cache);
    png_set_write_fn((png_structrp)png_ptr, (png_voidp)NULL, (png_rw_ptr)NULL, (png_flush_ptr)NULL);

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // silence unused-variable warnings
    (void)version;
    (void)y_offset_inches;
    (void)width_max;
    (void)derived_cache;

    return 66;
}