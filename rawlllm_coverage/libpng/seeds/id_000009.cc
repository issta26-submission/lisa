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
//<ID> 70
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structrp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = (png_info *)info_ptr;

    // step 2: Configure
    png_set_IHDR((png_const_structrp)png_ptr, (png_inforp)info_ptr, (png_uint_32)800, (png_uint_32)600, 8, 2, 0, 0, 0);
    int prev_option = png_set_option((png_structrp)png_ptr, 0, 1);
    png_set_write_status_fn((png_structrp)png_ptr, (png_write_status_ptr)NULL);

    // step 3: Operate / Validate
    png_byte filter_type = png_get_filter_type((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    int combined = prev_option + (int)filter_type;
    (void)combined;
    (void)filter_type;
    (void)prev_option;

    // step 4: Cleanup
    png_destroy_write_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict);

    // API sequence test completed successfully
    return 66;
}