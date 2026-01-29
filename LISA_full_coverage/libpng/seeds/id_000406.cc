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
//<ID> 406
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_info *info_ptr = (png_info *)NULL;
    png_voidp mem = (png_voidp)NULL;
    png_uint_32 image_width = (png_uint_32)0;
    png_byte current_pass = (png_byte)0;

    // step 2: Configure
    mem = png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)128);
    memset(mem, 0, 128);
    png_set_option((png_structrp)png_ptr, 1, 1);
    png_set_mDCV_fixed((png_const_structrp)png_ptr, (png_inforp)info_ptr,
                       (png_fixed_point)31270, (png_fixed_point)32900,
                       (png_fixed_point)6400, (png_fixed_point)3300,
                       (png_fixed_point)3000, (png_fixed_point)6000,
                       (png_fixed_point)15000, (png_fixed_point)12000,
                       (png_uint_32)1000000U, (png_uint_32)0U);

    // step 3: Operate / Validate
    current_pass = png_get_current_pass_number((png_const_structrp)png_ptr);
    image_width = png_get_image_width((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    {
        unsigned char *buf = (unsigned char *)mem;
        buf[0] = (unsigned char)(image_width & 0xFFu);
        buf[1] = (unsigned char)((image_width >> 8) & 0xFFu);
        buf[2] = (unsigned char)((image_width >> 16) & 0xFFu);
        buf[3] = (unsigned char)((image_width >> 24) & 0xFFu);
        buf[4] = (unsigned char)current_pass;
        png_uint_32 combined = image_width + (png_uint_32)current_pass;
        buf[5] = (unsigned char)(combined & 0xFFu);
        (void)combined;
    }

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, mem);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    return 66; // API sequence test completed successfully
}