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
//<ID> 432
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);

    // step 2: Configure
    png_set_chunk_cache_max((png_structrp)png_ptr, (png_uint_32)1024U);
    png_set_write_fn((png_structrp)png_ptr, (png_voidp)NULL, (png_rw_ptr)NULL, (png_flush_ptr)NULL);

    // step 3: Operate / Validate
    double file_gamma = 0.0;
    png_uint_32 g_ret = png_get_gAMA((png_const_structrp)png_ptr, (png_const_inforp)info_ptr, &file_gamma);
    png_uint_32 height = png_get_image_height((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_set_chunk_cache_max((png_structrp)png_ptr, (png_uint_32)((double)g_ret + file_gamma + (double)height));
    png_uint_32 width = png_get_image_width((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_uint_32 ppi = png_get_pixels_per_inch((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    (void)width;
    (void)ppi;

    // step 4: Cleanup
    png_voidp tmp = png_malloc((png_const_structrp)png_ptr, (png_alloc_size_t)16);
    png_free((png_const_structrp)png_ptr, tmp);
    png_free((png_const_structrp)png_ptr, (png_voidp)info_ptr);

    return 66;
}