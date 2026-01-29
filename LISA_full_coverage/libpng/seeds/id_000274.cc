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
//<ID> 274
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    void *user_err = malloc(128);
    memset(user_err, 0x5A, 128);
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", user_err, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    unsigned char icc_profile[32];
    memset(icc_profile, 0xAB, sizeof(icc_profile));
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = 0;
    image.width = 10;
    image.format = 0;
    image.colormap_entries = 0;

    // step 2: Configure
    png_set_error_fn((png_structrp)png_ptr, user_err, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_set_iCCP((png_const_structrp)png_ptr, info_ptr, (png_const_charp)"ExampleICC", 0, (png_const_bytep)icc_profile, (png_uint_32)sizeof(icc_profile));

    // step 3: Operate / Validate
    png_uint_32 height = png_get_image_height((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    image.height = height;
    png_int_32 row_stride = (png_int_32)(image.width * 4);
    size_t alloc_rows = (size_t)(image.height + 1);
    size_t pixel_buf_size = (size_t)row_stride * alloc_rows;
    void *pixel_buffer = malloc(pixel_buf_size);
    memset(pixel_buffer, 0x7F, pixel_buf_size);
    size_t out_mem_bytes = (size_t)row_stride * alloc_rows + 128;
    void *out_memory = malloc(out_mem_bytes);
    memset(out_memory, 0x00, out_mem_bytes);
    int write_rc = png_image_write_to_memory((png_imagep)&image, out_memory, (png_alloc_size_t *)&out_mem_bytes, 1, pixel_buffer, row_stride, (const void *)NULL);

    // step 4: Cleanup
    free(pixel_buffer);
    free(out_memory);
    free(user_err);
    png_destroy_info_struct((png_const_structrp)png_ptr, (png_infopp)&info_ptr);

    // API sequence test completed successfully
    (void)write_rc;
    (void)height;
    (void)row_stride;
    return 66;
}