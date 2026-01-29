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
//<ID> 276
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_voidp user_mem = malloc(256);
    memset(user_mem, 0x5A, 256);
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    unsigned char icc_profile[16];
    memset(icc_profile, 0x77, sizeof(icc_profile));
    png_uint_32 height_val;

    // step 2: Configure
    png_set_error_fn((png_structrp)png_ptr, user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_set_iCCP((png_const_structrp)png_ptr, info_ptr, (png_const_charp)"Example ICC", 0, (png_const_bytep)icc_profile, (png_uint_32)sizeof(icc_profile));

    // step 3: Operate / Validate
    height_val = png_get_image_height((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_image img;
    memset(&img, 0, sizeof(img));
    img.version = 1;
    img.width = 100;
    img.height = height_val + 1;
    img.format = 0;
    img.colormap_entries = 0;
    png_int_32 row_stride = (png_int_32)(img.width * 4);
    size_t mem_bytes = (size_t)(img.height * (png_uint_32)row_stride + 64);
    void *mem_out = malloc(mem_bytes);
    memset(mem_out, 0xAA, mem_bytes);
    void *buffer = malloc((size_t)img.height * (size_t)row_stride);
    memset(buffer, 0xFF, (size_t)img.height * (size_t)row_stride);
    int write_res = png_image_write_to_memory((png_imagep)&img, mem_out, (png_alloc_size_t *)&mem_bytes, 1, buffer, row_stride, (const void *)NULL);

    // step 4: Cleanup
    free(buffer);
    free(mem_out);
    png_destroy_info_struct((png_const_structrp)png_ptr, (png_infopp)&info_ptr);
    free(user_mem);
    // API sequence test completed successfully
    (void)write_res;
    (void)height_val;
    return 66;
}