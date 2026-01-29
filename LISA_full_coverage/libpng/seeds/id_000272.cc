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
//<ID> 272
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    void *user_mem = malloc(128);
    memset(user_mem, 0xA5, 128);
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    unsigned char icc_profile[64];
    memset(icc_profile, 0x5A, sizeof(icc_profile));

    // step 2: Configure
    png_set_error_fn((png_structrp)png_ptr, (png_voidp)user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_set_iCCP((png_const_structrp)png_ptr, info_ptr, (png_const_charp)"Example ICC", 0, (png_const_bytep)icc_profile, (png_uint_32)sizeof(icc_profile));

    // step 3: Operate / Validate
    png_uint_32 img_height = png_get_image_height((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = (png_uint_32)1;
    image.width = (png_uint_32)16;
    image.height = img_height;
    image.format = (png_uint_32)0;
    unsigned char pixel_buffer[16 * 4];
    memset(pixel_buffer, 0x7F, sizeof(pixel_buffer));
    void *out_memory = malloc(4096);
    png_alloc_size_t out_size = (png_alloc_size_t)4096;
    int write_ok = png_image_write_to_memory((png_imagep)&image, out_memory, &out_size, 1, (const void *)pixel_buffer, (png_int_32)(image.width * 4), (const void *)NULL);

    // step 4: Cleanup
    png_destroy_info_struct((png_const_structrp)png_ptr, (png_infopp)&info_ptr);
    free(out_memory);
    free(user_mem);

    // API sequence test completed successfully
    (void)write_ok;
    (void)img_height;
    return 66;
}