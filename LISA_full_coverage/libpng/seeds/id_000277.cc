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
//<ID> 277
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_set_error_fn((png_structrp)png_ptr, (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);

    // step 2: Configure
    unsigned char icc_profile[16];
    memset(icc_profile, 0x5A, sizeof(icc_profile));
    png_set_iCCP((png_const_structrp)png_ptr, info_ptr, (png_const_charp)"test-icc", 0, (png_const_bytep)icc_profile, (png_uint_32)sizeof(icc_profile));

    // step 3: Operate / Validate
    png_uint_32 img_height = png_get_image_height((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_image image;
    memset(&image, 0, sizeof(image));
    image.width = 100;
    image.height = img_height;
    image.format = 0;
    image.colormap_entries = 0;
    unsigned char pixel_buffer[300];
    memset(pixel_buffer, 0xAA, sizeof(pixel_buffer));
    png_int_32 row_stride = (png_int_32)(image.width * 3);
    void *out_memory = malloc((size_t)1024);
    png_alloc_size_t out_bytes = (png_alloc_size_t)1024;
    png_image_write_to_memory(&image, out_memory, &out_bytes, 1, (const void *)pixel_buffer, row_stride, (const void *)NULL);

    // step 4: Cleanup
    free(out_memory);
    png_destroy_info_struct((png_const_structrp)png_ptr, (png_infopp)&info_ptr);
    png_free_default((png_const_structrp)png_ptr, (png_voidp)png_ptr);

    // API sequence test completed successfully
    return 66;
}