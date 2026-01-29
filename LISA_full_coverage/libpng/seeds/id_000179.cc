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
//<ID> 179
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
    png_uint_32 proflen = (png_uint_32)32;
    unsigned char *profile = (unsigned char *)malloc((size_t)proflen);
    memset(profile, 0xAB, (size_t)proflen);

    // step 2: Configure
    png_set_compression_buffer_size((png_structrp)png_ptr, (size_t)8192);
    png_set_iCCP((png_const_structrp)png_ptr, (png_inforp)info_ptr, (png_const_charp)"embedded_icc", 0, (png_const_bytep)profile, proflen);

    // step 3: Operate / Validate
    png_uint_32 user_hmax = png_get_user_height_max((png_const_structrp)png_ptr);
    png_uint_32 image_h = png_get_image_height((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    unsigned int mix = ((unsigned int)user_hmax) ^ ((unsigned int)image_h) ^ ((unsigned int)profile[0]);
    profile[0] = (unsigned char)(mix & 0xFF);
    (void)mix;

    // step 4: Cleanup
    free(profile);
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);

    // API sequence test completed successfully
    return 66;
}