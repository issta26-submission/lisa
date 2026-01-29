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
//<ID> 172
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);

    // step 2: Configure
    png_set_compression_buffer_size((png_structrp)png_ptr, (size_t)16384);
    const png_byte icc_profile_data[6] = { 0x10, 0x20, 0x30, 0x40, 0x50, 0x60 };
    png_set_iCCP((png_const_structrp)png_ptr, info_ptr, (png_const_charp)"libpng-sample-icc", 0, (png_const_bytep)icc_profile_data, (png_uint_32)6);

    // step 3: Operate / Validate
    png_uint_32 user_hmax = png_get_user_height_max((png_const_structrp)png_ptr);
    png_uint_32 img_h = png_get_image_height((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    size_t alloc_size = (size_t)user_hmax + (size_t)img_h + (size_t)32;
    png_voidp buffer = png_malloc_warn((png_const_structrp)png_ptr, (png_alloc_size_t)alloc_size);
    memset(buffer, 0, alloc_size);
    unsigned char *b = (unsigned char *)buffer;
    b[0] = (unsigned char)((user_hmax ^ img_h) & 0xFF);
    (void)b[0];

    // step 4: Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    free(buffer);

    // API sequence test completed successfully
    return 66;
}