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
//<ID> 270
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    void * user_error_mem = malloc(64);
    memset(user_error_mem, 0xEE, 64);
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)user_error_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    unsigned char icc_profile[16];
    memset(icc_profile, 0x42, sizeof(icc_profile));

    // step 2: Configure
    png_set_error_fn((png_structrp)png_ptr, user_error_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_set_iCCP((png_const_structrp)png_ptr, info_ptr, (png_const_charp)"example.icc", 0, (png_const_bytep)icc_profile, (png_uint_32)sizeof(icc_profile));

    // step 3: Operate / Validate
    png_uint_32 height = png_get_image_height((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = 1;
    image.width = 8;
    image.height = height + 1;
    image.format = 0;
    image.flags = 0;
    image.colormap_entries = 0;
    image.warning_or_error = 0;
    unsigned int pixel_count = (unsigned int)(image.width * image.height);
    png_int_32 row_stride = (png_int_32)(image.width * 4);
    unsigned int buf_size = pixel_count * 4;
    void * buffer = malloc(buf_size ? buf_size : 1);
    memset(buffer, 0x7F, buf_size ? buf_size : 1);
    void * out_mem = malloc(1024);
    png_alloc_size_t out_size = (png_alloc_size_t)1024;
    int write_result = png_image_write_to_memory((png_imagep)&image, out_mem, &out_size, 1, buffer, row_stride, (const void *)NULL);
    unsigned char * out_bytes = (unsigned char *)out_mem;
    if (out_size > 0) {
        out_bytes[0] = (unsigned char)(write_result & 0xFF);
    }

    // step 4: Cleanup
    free(buffer);
    free(out_mem);
    free(user_error_mem);
    png_destroy_info_struct((png_const_structrp)png_ptr, (png_infopp)&info_ptr);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)NULL, (png_infopp)NULL);

    // API sequence test completed successfully
    (void)out_bytes;
    (void)write_result;
    return 66;
}