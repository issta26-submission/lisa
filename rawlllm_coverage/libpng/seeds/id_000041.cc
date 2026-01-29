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
//<ID> 273
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
    unsigned char icc_profile[32];
    memset(icc_profile, 0x1F, sizeof(icc_profile));
    png_image image;
    memset((void *)&image, 0, sizeof(image));
    image.version = 1;
    image.width = 64;
    image.height = 16;
    image.format = 0;
    image.flags = 0;
    image.colormap_entries = 0;
    image.warning_or_error = 0;
    png_alloc_size_t out_size = 512;
    void *out_mem = malloc(out_size);
    memset(out_mem, 0, out_size);

    // step 2: Configure
    png_set_error_fn((png_structrp)png_ptr, (png_voidp)user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_set_iCCP((png_const_structrp)png_ptr, info_ptr, (png_const_charp)"ICC Profile", 0, (png_const_bytep)icc_profile, (png_uint_32)sizeof(icc_profile));

    // step 3: Operate / Validate
    png_uint_32 info_height = png_get_image_height((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    int write_ret = png_image_write_to_memory(&image, out_mem, &out_size, 1, (const void *)icc_profile, (png_int_32)info_height, (const void *)NULL);
    (void)write_ret;
    (void)info_height;
    (void)out_size;

    // step 4: Cleanup
    png_image_free(&image);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);
    free(out_mem);
    free(user_mem);

    // API sequence test completed successfully
    return 66;
}