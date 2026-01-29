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
//<ID> 79
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structrp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = (png_info *)info_ptr;
    png_uint_32 width = (png_uint_32)640;
    png_uint_32 height = (png_uint_32)480;

    // step 2: Configure
    png_set_IHDR((png_const_structrp)png_ptr, (png_inforp)info_ptr, width, height, 8, 2, 0, 0, 0);
    (void)png_set_option((png_structrp)png_ptr, 1, 1);
    png_set_write_status_fn((png_structrp)png_ptr, (png_write_status_ptr)NULL);

    // step 3: Operate / Validate
    png_byte filter_type = png_get_filter_type((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_uint_32 read_width = 0;
    png_uint_32 read_height = 0;
    int bit_depth = 0;
    int color_type = 0;
    int interlace_method = 0;
    int compression_method = 0;
    int filter_method = 0;
    png_get_IHDR((png_const_structrp)png_ptr, (png_const_inforp)info_ptr, &read_width, &read_height, &bit_depth, &color_type, &interlace_method, &compression_method, &filter_method);
    unsigned int combined = (unsigned int)filter_type + (unsigned int)read_width + (unsigned int)read_height + (unsigned int)bit_depth + (unsigned int)color_type;
    (void)combined;

    // step 4: Cleanup
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}