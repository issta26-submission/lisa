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
//<ID> 174
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);

    // step 2: Configure
    size_t comp_buf_size = (size_t)8192;
    png_set_compression_buffer_size((png_structrp)png_ptr, comp_buf_size);
    unsigned char icc_profile_buf[8];
    icc_profile_buf[0] = 0x10;
    icc_profile_buf[1] = 0x20;
    icc_profile_buf[2] = 0x30;
    icc_profile_buf[3] = 0x40;
    icc_profile_buf[4] = 0x50;
    icc_profile_buf[5] = 0x60;
    icc_profile_buf[6] = 0x70;
    icc_profile_buf[7] = 0x80;
    memset(icc_profile_buf + 4, 0, (size_t)4);
    png_const_charp icc_name = (png_const_charp)"example_icc";
    int compression_type = 0;
    png_const_bytep profile_ptr = (png_const_bytep)icc_profile_buf;
    png_uint_32 profile_len = (png_uint_32)8;
    png_set_iCCP((png_const_structrp)png_ptr, info_ptr, icc_name, compression_type, profile_ptr, profile_len);

    // step 3: Operate / Validate
    png_uint_32 user_hmax = png_get_user_height_max((png_const_structrp)png_ptr);
    png_uint_32 img_height = png_get_image_height((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    unsigned int mix = ((unsigned int)user_hmax) ^ ((unsigned int)img_height) ^ ((unsigned int)profile_len) ^ ((unsigned int)icc_profile_buf[0]);
    icc_profile_buf[0] = (unsigned char)(mix & 0xFF);

    // step 4: Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);

    // API sequence test completed successfully
    return 66;
}