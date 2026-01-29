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
//<ID> 359
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Create structures
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);

    // step 2: Allocate a buffer using the library allocator based on chunk malloc max
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);
    png_alloc_size_t exif_size = chunk_max + (png_alloc_size_t)16;
    png_bytep exif_buf = (png_bytep)png_malloc_default((png_const_structrp)png_ptr, exif_size);
    memset(exif_buf, 0xA5, (size_t)exif_size);

    // step 3: Set a valid gamma and set sCAL fixed values, then retrieve them
    png_set_gAMA((png_const_structrp)png_ptr, info_ptr, 1.0);
    png_set_sCAL_fixed((png_const_structrp)png_ptr, info_ptr, 1, (png_fixed_point)1000, (png_fixed_point)2000);
    int sc_unit = 0;
    png_fixed_point sc_width = 0;
    png_fixed_point sc_height = 0;
    png_uint_32 sc_ret = png_get_sCAL_fixed((png_const_structrp)png_ptr, (png_const_inforp)info_ptr, &sc_unit, &sc_width, &sc_height);
    (void)sc_ret;
    (void)sc_unit;
    (void)sc_width;
    (void)sc_height;

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, (png_voidp)exif_buf);
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);

    return 66;
}