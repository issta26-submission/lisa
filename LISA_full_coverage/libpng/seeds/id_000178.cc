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
//<ID> 178
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
    png_set_compression_buffer_size((png_structrp)png_ptr, (size_t)8192);
    static const png_byte icc_profile[] = { 0x02u, 0x04u, 0x06u, 0x08u };
    png_set_iCCP((png_const_structrp)png_ptr, info_ptr, (png_const_charp)"ICC Profile", 0, (png_const_bytep)icc_profile, (png_uint_32)sizeof(icc_profile));

    // step 3: Operate / Validate
    png_uint_32 user_hmax = png_get_user_height_max((png_const_structrp)png_ptr);
    png_uint_32 img_h = png_get_image_height((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    void * buffer = malloc((size_t)16);
    memset(buffer, 0, (size_t)16);
    unsigned char * cbuf = (unsigned char *)buffer;
    unsigned int mix = ((unsigned int)user_hmax) ^ ((unsigned int)img_h) ^ ((unsigned int)cbuf[0]) ^ ((unsigned int)icc_profile[0]);
    cbuf[0] = (unsigned char)(mix & 0xFF);
    (void)mix;

    // step 4: Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    free(buffer);

    // API sequence test completed successfully
    return 66;
}