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
//<ID> 396
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
    unsigned char local_mem[64];
    memset(local_mem, 0xAA, sizeof(local_mem));

    // step 2: Configure
    png_set_mem_fn((png_structrp)png_ptr, (png_voidp)local_mem, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_set_chunk_malloc_max((png_structrp)png_ptr, (png_alloc_size_t)4096);

    // step 3: Operate / Validate
    png_voidp io_ptr = png_get_io_ptr((png_const_structrp)png_ptr);
    png_byte filter_type = png_get_filter_type((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_uint_32 x_pixels_per_meter = png_get_x_pixels_per_meter((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_set_option((png_structrp)png_ptr, 0, (int)(filter_type & 1));
    png_set_chunk_cache_max((png_structrp)png_ptr, x_pixels_per_meter);
    (void)io_ptr; (void)filter_type; (void)x_pixels_per_meter;

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}