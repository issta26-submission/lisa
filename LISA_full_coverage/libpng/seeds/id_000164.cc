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
//<ID> 164
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
    png_set_write_fn((png_structrp)png_ptr, (png_voidp)NULL, (png_rw_ptr)NULL, (png_flush_ptr)NULL);
    png_set_alpha_mode((png_structrp)png_ptr, 1, 2.2);

    // step 3: Operate / Validate
    png_uint_32 user_height_max = png_get_user_height_max((png_const_structrp)png_ptr);
    png_int_32 x_offset = png_get_x_offset_microns((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_const_charp ver = png_get_libpng_ver((png_const_structrp)png_ptr);
    png_voidp buffer = png_malloc_warn((png_const_structrp)png_ptr, (png_alloc_size_t)64);
    memset(buffer, 0, (size_t)64);
    unsigned int u0 = (unsigned int)user_height_max;
    unsigned int u1 = (unsigned int)x_offset;
    unsigned char first = (unsigned char)(ver != (png_const_charp)NULL ? ver[0] : 0);
    ((unsigned char *)buffer)[0] = first;
    unsigned int mix = u0 ^ u1 ^ ((unsigned int)first);
    (void)mix;

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, buffer);
    png_destroy_info_struct((png_const_structrp)png_ptr, &info_ptr_nonrestrict);
    png_destroy_read_struct(&png_ptr_nonrestrict, (png_infopp)NULL, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}