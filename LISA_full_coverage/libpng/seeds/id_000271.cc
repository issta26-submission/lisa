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
//<ID> 271
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    void *user_mem = malloc(256);
    memset(user_mem, 0x5A, 256);
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    unsigned char icc_profile[32];
    memset(icc_profile, 0xAB, sizeof(icc_profile));
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = 0x010000; /* arbitrary version */
    image.width = 8;
    image.height = 8;
    image.format = 0;
    png_alloc_size_t out_size = (png_alloc_size_t)4096;
    void *out_memory = malloc((size_t)out_size);

    // step 2: Configure
    png_set_error_fn((png_structrp)png_ptr, (png_voidp)user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_set_iCCP((png_const_structrp)png_ptr, info_ptr, (png_const_charp)"Example ICC", 0, (png_const_bytep)icc_profile, (png_uint_32)sizeof(icc_profile));

    // step 3: Operate / Validate
    png_uint_32 got_h = png_get_image_height((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    image.height = image.height + got_h;
    png_int_32 row_stride = (png_int_32)(image.width * 4);
    png_alloc_size_t pixel_buf_size = (png_alloc_size_t)((png_uint_32)row_stride * image.height);
    void *pixel_buf = malloc((size_t)(pixel_buf_size));
    memset(pixel_buf, 0x7F, (size_t)pixel_buf_size);
    int write_ok = png_image_write_to_memory((png_imagep)&image, out_memory, &out_size, 1, pixel_buf, row_stride, (const void *)NULL);
    (void)write_ok;
    (void)got_h;

    // step 4: Cleanup
    free(pixel_buf);
    free(out_memory);
    png_free_default((png_const_structrp)png_ptr, (png_voidp)info_ptr);
    png_free_default((png_const_structrp)png_ptr, (png_voidp)png_ptr);
    free(user_mem);

    // API sequence test completed successfully
    return 66;
}