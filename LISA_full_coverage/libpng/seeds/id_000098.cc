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
//<ID> 98
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = (png_info *)info_ptr;
    unsigned char * buf = (unsigned char *)malloc((size_t)4);
    memset(buf, 0, (size_t)4);

    // step 2: Configure
    png_set_gray_to_rgb((png_structrp)png_ptr);
    png_const_charp ver = png_get_libpng_ver((png_const_structrp)png_ptr);
    buf[0] = (unsigned char)ver[0];
    buf[1] = 0x11u;
    buf[2] = 0x22u;
    buf[3] = 0x33u;

    // step 3: Operate / Validate
    png_uint_32 parsed = png_get_uint_31((png_const_structrp)png_ptr, (png_const_bytep)buf);
    png_uint_32 combined = parsed ^ (png_uint_32)buf[0];
    (void)combined;
    (void)parsed;
    (void)ver;

    // step 4: Cleanup
    free(buf);
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}