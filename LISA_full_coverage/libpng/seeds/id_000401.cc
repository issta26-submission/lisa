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
//<ID> 401
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Initialize
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_info *info_ptr = (png_info *)NULL;

    // step 2: Configure
    png_set_option((png_structrp)png_ptr, 1, 1);
    png_set_mDCV_fixed((png_const_structrp)png_ptr, (png_inforp)info_ptr,
                      (png_fixed_point)10000, (png_fixed_point)10000,
                      (png_fixed_point)31270, (png_fixed_point)32900,
                      (png_fixed_point)6400,  (png_fixed_point)3300,
                      (png_fixed_point)3000,  (png_fixed_point)6000,
                      (png_uint_32)100000u, (png_uint_32)0u);

    // step 3: Operate / Validate
    png_uint_32 width = png_get_image_width((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_byte pass = png_get_current_pass_number((png_const_structrp)png_ptr);
    size_t alloc_size = (size_t)((width == 0u) ? 1u : (width * 4u));
    png_voidp buffer = png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)alloc_size);
    memset(buffer, 0xA5, alloc_size);
    ((unsigned char *)buffer)[0] = (unsigned char)pass;
    png_free_default((png_const_structrp)png_ptr, buffer);

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}