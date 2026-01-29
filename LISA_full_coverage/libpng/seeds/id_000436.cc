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
//<ID> 436
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_infop info_ptr = (png_infop)NULL;
    unsigned char io_buffer[32];
    memset(io_buffer, 0, sizeof(io_buffer));
    png_uint_32 width = (png_uint_32)0;
    float aspect = 0.0f;
    png_uint_32 rownum = (png_uint_32)0;

    // step 2: Configure
    info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_set_write_fn((png_structrp)png_ptr, (png_voidp)io_buffer, (png_rw_ptr)NULL, (png_flush_ptr)NULL);
    png_set_chunk_malloc_max((png_structrp)png_ptr, (png_alloc_size_t)1024U);

    // step 3: Operate / Validate
    width = png_get_image_width((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    aspect = png_get_pixel_aspect_ratio((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    rownum = png_get_current_row_number((png_const_structrp)png_ptr);
    io_buffer[0] = (unsigned char)((width & 0xFFU) ^ (rownum & 0xFFU) ^ (unsigned char)(((int)(aspect * 100.0f)) & 0xFF));
    (void)width;
    (void)aspect;
    (void)rownum;

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}