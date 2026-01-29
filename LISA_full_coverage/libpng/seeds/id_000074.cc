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
//<ID> 74
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structrp png_ptr = (png_structrp)png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = (png_info *)info_ptr;

    // step 2: Configure
    png_uint_32 width = (png_uint_32)800;
    png_uint_32 height = (png_uint_32)600;
    int bit_depth = 8;
    int color_type = 2;
    int interlace_method = 0;
    int compression_method = 0;
    int filter_method = 0;
    png_set_IHDR((png_const_structrp)png_ptr, (png_inforp)info_ptr, width, height, bit_depth, color_type, interlace_method, compression_method, filter_method);
    png_set_write_status_fn((png_structrp)png_ptr, (png_write_status_ptr)NULL);
    int option_result = png_set_option((png_structrp)png_ptr, 0, 1);

    // step 3: Operate / Validate
    png_byte filter_type = png_get_filter_type((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_uint_32 combined = width ^ height ^ (png_uint_32)filter_type ^ (png_uint_32)(option_result & 0xFFFFFFFFU);
    (void)combined;
    (void)filter_type;

    // step 4: Cleanup
    png_destroy_write_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict);
    // API sequence test completed successfully
    return 66;
}