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
//<ID> 170
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);

    // step 2: Configure
    png_set_compression_buffer_size(png_ptr, (size_t)8192);
    png_uint_32 width = 100;
    png_uint_32 height = 50;
    int bit_depth = 8;
    int color_type = 2;
    int interlace_method = 0;
    int compression_method = 0;
    int filter_method = 0;
    png_set_IHDR((png_const_structrp)png_ptr, info_ptr, width, height, bit_depth, color_type, interlace_method, compression_method, filter_method);
    unsigned char icc_profile[4];
    icc_profile[0] = 0x11;
    icc_profile[1] = 0x22;
    icc_profile[2] = 0x33;
    icc_profile[3] = 0x44;
    png_set_iCCP((png_const_structrp)png_ptr, info_ptr, (png_const_charp)"Test ICC", 0, (png_const_bytep)icc_profile, (png_uint_32)4);

    // step 3: Operate / Validate
    png_uint_32 user_hmax = png_get_user_height_max((png_const_structrp)png_ptr);
    png_uint_32 image_h = png_get_image_height((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    unsigned char buffer[16];
    memset(buffer, 0, (size_t)16);
    unsigned int mix = ((unsigned int)user_hmax) ^ ((unsigned int)image_h) ^ ((unsigned int)icc_profile[0]);
    buffer[0] = (unsigned char)(mix & 0xFF);
    (void)mix;

    // step 4: Cleanup
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);
    // API sequence test completed successfully
    return 66;
}