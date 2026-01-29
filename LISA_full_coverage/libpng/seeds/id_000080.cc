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
//<ID> 80
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = (png_info *)NULL;

    // step 2: Configure
    int option_ret = png_set_option((png_structrp)png_ptr, 1, 1);
    png_const_charp copyright = png_get_copyright((png_const_structrp)png_ptr);
    png_uint_32 derived_value = (png_uint_32)(unsigned char)copyright[0];
    unsigned char buf[4];
    png_save_uint_32((png_bytep)buf, derived_value);

    // step 3: Operate / Validate
    png_uint_32 recovered = ((png_uint_32)buf[0] << 24) | ((png_uint_32)buf[1] << 16) | ((png_uint_32)buf[2] << 8) | (png_uint_32)buf[3];
    png_uint_32 max_width = png_get_user_width_max((png_const_structrp)png_ptr);
    png_uint_32 combined = recovered + max_width + (png_uint_32)option_ret;
    (void)combined;

    // step 4: Cleanup
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}