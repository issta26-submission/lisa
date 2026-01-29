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
//<ID> 342
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_const_charp ver = png_get_libpng_ver((png_const_structrp)png_ptr);
    png_uint_32 proflen = (png_uint_32)32u;
    png_voidp profile = png_calloc((png_const_structrp)png_ptr, (png_alloc_size_t)proflen);
    memset(profile, 0x5A, (size_t)proflen);

    // step 2: Configure
    png_set_error_fn((png_structrp)png_ptr, (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    ((unsigned char *)profile)[0] = (unsigned char)ver[0];
    ((unsigned char *)profile)[1] = (unsigned char)ver[1];
    ((unsigned char *)profile)[2] = (unsigned char)ver[2];
    ((unsigned char *)profile)[proflen - 1] = (unsigned char)0u;
    png_set_iCCP((png_const_structrp)png_ptr, info_ptr, (png_const_charp)"ICCTEST", 0, (png_const_bytep)profile, proflen);

    // step 3: Operate / Validate
    png_const_charp ver_check = png_get_libpng_ver((png_const_structrp)png_ptr);
    ((unsigned char *)profile)[3] = (unsigned char)ver_check[0];

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, profile);
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);

    // API sequence test completed successfully
    (void)ver_check;
    (void)ver;
    return 66;
}