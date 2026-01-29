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
//<ID> 167
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
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = (png_info *)info_ptr;

    // step 2: Configure
    png_set_write_fn((png_structrp)png_ptr, (png_voidp)NULL, (png_rw_ptr)NULL, (png_flush_ptr)NULL);
    png_set_alpha_mode((png_structrp)png_ptr, 1, 0.45455);

    // step 3: Operate / Validate
    png_int_32 x_offset = png_get_x_offset_microns((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_uint_32 user_hmax = png_get_user_height_max((png_const_structrp)png_ptr);
    png_alloc_size_t alloc_size = (png_alloc_size_t)16;
    png_voidp buffer = png_malloc((png_const_structrp)png_ptr, alloc_size);
    memset(buffer, (int)((unsigned int)(x_offset ^ (png_int_32)user_hmax) & 0xFF), (size_t)alloc_size);
    unsigned char first = ((unsigned char *)buffer)[0];
    unsigned int mix = ((unsigned int)first) ^ ((unsigned int)(x_offset & 0xFF)) ^ ((unsigned int)(user_hmax & 0xFF));
    (void)mix;
    png_free_default((png_const_structrp)png_ptr, buffer);

    // step 4: Cleanup
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}