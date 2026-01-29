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
//<ID> 393
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
    png_uint_32 x_pixels_per_meter = (png_uint_32)0;
    png_voidp io_ptr = (png_voidp)NULL;
    png_byte filter_type = (png_byte)0;

    // step 2: Configure
    png_set_chunk_cache_max((png_structrp)png_ptr, (png_uint_32)4096u);
    png_set_chunk_malloc_max((png_structrp)png_ptr, (png_alloc_size_t)8192u);

    // step 3: Operate / Validate
    io_ptr = png_get_io_ptr((png_const_structrp)png_ptr);
    filter_type = png_get_filter_type((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    x_pixels_per_meter = png_get_x_pixels_per_meter((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_set_chunk_cache_max((png_structrp)png_ptr, x_pixels_per_meter);
    png_set_filter((png_structrp)png_ptr, (int)filter_type, (int)filter_type);
    png_set_user_transform_info((png_structrp)png_ptr, io_ptr, (int)((x_pixels_per_meter >> 8) & 0xFF), (int)(filter_type & 0xFF));

    // step 4: Cleanup
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);
    // API sequence test completed successfully
    return 66;
}