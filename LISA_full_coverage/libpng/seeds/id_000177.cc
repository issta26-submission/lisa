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
//<ID> 177
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
    png_uint_32 icc_len = (png_uint_32)64;
    png_bytep icc_profile = (png_bytep)malloc((size_t)icc_len);
    memset(icc_profile, 0xA5, (size_t)icc_len);
    png_set_iCCP((png_const_structrp)png_ptr, info_ptr, (png_const_charp)"ICC Profile", 0, (png_const_bytep)icc_profile, icc_len);

    // step 3: Operate / Validate
    png_uint_32 user_hmax = png_get_user_height_max((png_const_structrp)png_ptr);
    png_uint_32 image_h = png_get_image_height((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    unsigned int mix = ((unsigned int)user_hmax) ^ ((unsigned int)image_h) ^ ((unsigned int)icc_profile[0]);
    icc_profile[0] = (png_byte)(mix & 0xFF);
    (void)mix;

    // step 4: Cleanup
    png_destroy_info_struct((png_const_structrp)png_ptr, &info_ptr);
    free(icc_profile);
    free((void*)png_ptr);

    // API sequence test completed successfully
    return 66;
}