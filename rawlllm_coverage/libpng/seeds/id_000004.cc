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
//<ID> 45
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_const_structrp png_ptr = (png_const_structrp)NULL;
    png_inforp info_ptr = png_create_info_struct(png_ptr);
    png_uint_32 width = (png_uint_32)256;
    png_uint_32 height = (png_uint_32)128;
    int bit_depth = 8;
    int color_type = 2; /* PNG_COLOR_TYPE_RGB */
    int interlace_method = 0;
    int compression_method = 0;
    int filter_method = 0;

    // step 2: Configure libpng behavior
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, interlace_method, compression_method, filter_method);
    png_set_progressive_read_fn((png_structrp)png_ptr, (png_voidp)info_ptr, (png_progressive_info_ptr)NULL, (png_progressive_row_ptr)NULL, (png_progressive_end_ptr)NULL);

    // step 3: Operate / Query state
    png_const_charp version = png_get_libpng_ver(png_ptr);
    png_const_charp copyright = png_get_copyright(png_ptr);
    (void)version;
    (void)copyright;

    // step 4: Validate-like cleanup
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = info_ptr;
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}