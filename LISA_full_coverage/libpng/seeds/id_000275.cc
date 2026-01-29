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
//<ID> 275
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_voidp user_mem = malloc(256);
    memset(user_mem, 0xA5, 256);
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    unsigned char icc_profile[64];
    memset(icc_profile, 0x5A, sizeof(icc_profile));
    png_set_error_fn((png_structrp)png_ptr, user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);

    // step 2: Configure
    png_set_iCCP((png_const_structrp)png_ptr, info_ptr, (png_const_charp)"Fuzz ICC Profile", 0, (png_const_bytep)icc_profile, (png_uint_32)sizeof(icc_profile));

    // step 3: Operate / Validate
    png_uint_32 height_info = png_get_image_height((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = 1;
    image.width = 8;
    image.height = height_info + 1;
    image.format = 0;
    png_int_32 row_stride = (png_int_32)(image.width * 4);
    png_alloc_size_t input_size = (png_alloc_size_t)row_stride * (png_alloc_size_t)image.height;
    void * input_buffer = malloc((size_t)input_size);
    memset(input_buffer, 0x7F, (size_t)input_size);
    void * out_memory = malloc(4096);
    png_alloc_size_t out_bytes = (png_alloc_size_t)4096;
    int write_result = png_image_write_to_memory(&image, out_memory, &out_bytes, 1, input_buffer, row_stride, (const void *)NULL);

    // step 4: Cleanup
    free(input_buffer);
    free(out_memory);
    png_destroy_info_struct((png_const_structrp)png_ptr, (png_infopp)&info_ptr);
    free(user_mem);

    (void)write_result;
    // API sequence test completed successfully
    return 66;
}