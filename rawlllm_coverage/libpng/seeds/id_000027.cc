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
//<ID> 160
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
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = (png_info *)info_ptr;

    // step 2: Configure
    png_uint_32 user_height_max = png_get_user_height_max((png_const_structrp)png_ptr);
    size_t buffer_size = (size_t) (user_height_max > 0 ? 256 : 256); /* keep straight-line code; use value */
    void *io_buffer = malloc(buffer_size);
    memset(io_buffer, 0, buffer_size);
    png_set_write_fn((png_structrp)png_ptr, (png_voidp)io_buffer, (png_rw_ptr)NULL, (png_flush_ptr)NULL);
    png_set_alpha_mode((png_structrp)png_ptr, 1, 2.2);

    // step 3: Operate / Validate
    png_int_32 x_offset = png_get_x_offset_microns((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    unsigned char *bytes = (unsigned char *)io_buffer;
    bytes[0] = (unsigned char)((unsigned int)x_offset & 0xFF);
    unsigned int mix = ((unsigned int)user_height_max) ^ ((unsigned int)x_offset) ^ ((unsigned int)bytes[0]);
    (void)mix;

    // step 4: Cleanup
    free(io_buffer);
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}