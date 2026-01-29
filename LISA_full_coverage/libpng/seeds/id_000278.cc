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
//<ID> 278
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    void *user_error_mem = malloc(32);
    memset(user_error_mem, 0x5A, 32);
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", user_error_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    unsigned char icc_profile[16];
    memset(icc_profile, 0xC3, sizeof(icc_profile));
    unsigned char sample_buffer[64];
    memset(sample_buffer, 0x7E, sizeof(sample_buffer));

    // step 2: Configure
    png_set_error_fn((png_structrp)png_ptr, user_error_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_set_iCCP((png_const_structrp)png_ptr, info_ptr, (png_const_charp)"ICC Profile", 0, (png_const_bytep)icc_profile, (png_uint_32)sizeof(icc_profile));

    // step 3: Operate / Validate
    png_uint_32 img_height = png_get_image_height((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = 1;
    image.width = 1;
    image.height = img_height;
    image.format = 0;
    image.flags = 0;
    image.colormap_entries = 0;
    image.warning_or_error = 0;
    void *out_mem = malloc(512);
    png_alloc_size_t out_size = (png_alloc_size_t)512;
    png_int_32 row_stride = 1;
    png_image_write_to_memory((png_imagep)&image, out_mem, &out_size, 1, (const void *)sample_buffer, row_stride, (const void *)NULL);
    ((unsigned char *)user_error_mem)[0] = (unsigned char)(out_size & 0xFF);
    ((unsigned char *)user_error_mem)[1] = (unsigned char)((out_size >> 8) & 0xFF);

    // step 4: Cleanup
    png_destroy_info_struct((png_const_structrp)png_ptr, (png_infopp)&info_ptr);
    free(out_mem);
    free(user_error_mem);

    // API sequence test completed successfully
    return 66;
}