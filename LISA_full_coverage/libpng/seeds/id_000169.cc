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
//<ID> 169
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_voidp write_io_ptr = png_malloc((png_const_structrp)png_ptr, (png_alloc_size_t)256);
    memset(write_io_ptr, 0, (size_t)256);

    // step 2: Configure
    png_set_write_fn((png_structrp)png_ptr, write_io_ptr, (png_rw_ptr)NULL, (png_flush_ptr)NULL);
    png_set_alpha_mode((png_structrp)png_ptr, 1, 2.200000);

    // step 3: Operate / Validate
    png_uint_32 user_hmax = png_get_user_height_max((png_const_structrp)png_ptr);
    png_int_32 xoff_microns = png_get_x_offset_microns((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    unsigned char *wbuf = (unsigned char *)write_io_ptr;
    unsigned int mix = ((unsigned int)user_hmax) ^ ((unsigned int)xoff_microns) ^ ((unsigned int)wbuf[0]);
    wbuf[0] = (unsigned char)(mix & 0xFF);
    (void)mix;

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, write_io_ptr);

    // API sequence test completed successfully
    return 66;
}