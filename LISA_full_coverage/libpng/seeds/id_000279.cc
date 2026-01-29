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
//<ID> 279
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
    unsigned char icc_profile[16];
    unsigned char pixel_buffer[256];
    void *write_memory = malloc(1024);
    png_alloc_size_t write_memory_bytes = (png_alloc_size_t)1024;
    int write_result;
    memset(icc_profile, 0x5A, sizeof(icc_profile));
    memset(pixel_buffer, 0x7F, sizeof(pixel_buffer));
    memset(write_memory, 0x00, (size_t)write_memory_bytes);

    // step 2: Configure
    png_set_error_fn((png_structrp)png_ptr, (png_voidp)icc_profile, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_set_iCCP((png_const_structrp)png_ptr, info_ptr, (png_const_charp)"example.icc", 0, (png_const_bytep)icc_profile, (png_uint_32)sizeof(icc_profile));

    // step 3: Operate / Validate
    png_uint_32 image_height = png_get_image_height((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = 1;
    image.width = 16;
    image.height = image_height + (png_uint_32)1;
    image.format = 0;
    image.flags = 0;
    write_result = png_image_write_to_memory((png_imagep)&image, write_memory, &write_memory_bytes, 1, (const void *)pixel_buffer, (png_int_32)16, (const void *)NULL);

    // step 4: Cleanup
    png_destroy_info_struct((png_const_structrp)png_ptr, (png_infopp)&info_ptr);
    free(write_memory);

    // API sequence test completed successfully
    (void)write_result;
    (void)image_height;
    return 66;
}