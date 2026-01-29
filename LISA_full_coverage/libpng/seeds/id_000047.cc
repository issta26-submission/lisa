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
//<ID> 47
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Initialize / Declarations
    png_structrp png_ptr = (png_structrp)NULL;
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_uint_32 width = (png_uint_32)800;
    png_uint_32 height = (png_uint_32)600;
    int bit_depth = 8;
    int color_type = 2;
    int interlace_method = 0;
    int compression_method = 0;
    int filter_method = 0;

    // step 2: Configure
    png_set_IHDR((png_const_structrp)png_ptr, info_ptr, width, height, bit_depth, color_type, interlace_method, compression_method, filter_method);
    png_set_progressive_read_fn((png_structrp)png_ptr, (png_voidp)info_ptr, (png_progressive_info_ptr)NULL, (png_progressive_row_ptr)NULL, (png_progressive_end_ptr)NULL);

    // step 3: Operate
    png_const_charp version_str = png_get_libpng_ver((png_const_structrp)png_ptr);
    png_const_charp copyright_str = png_get_copyright((png_const_structrp)png_ptr);
    (void)version_str;
    (void)copyright_str;

    // step 4: Validate / Cleanup
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = (png_info *)info_ptr;
    png_destroy_write_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict);

    // API sequence test completed successfully
    return 66;
}