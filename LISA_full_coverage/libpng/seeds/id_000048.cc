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
//<ID> 48
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Initialize
    png_structrp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = png_create_info_struct(png_ptr);
    png_const_charp libver = png_get_libpng_ver((png_const_structrp)png_ptr);
    png_const_charp copyright_str = png_get_copyright((png_const_structrp)png_ptr);
    png_uint_32 width = (png_uint_32)128;
    png_uint_32 height = (png_uint_32)64;

    // step 2: Configure
    png_set_progressive_read_fn(png_ptr, (png_voidp)info_ptr, (png_progressive_info_ptr)NULL, (png_progressive_row_ptr)NULL, (png_progressive_end_ptr)NULL);
    png_set_IHDR((png_const_structrp)png_ptr, info_ptr, width, height, 8, 2, 0, 0, 0);

    // step 3: Operate / Validate
    size_t rowbytes = png_get_rowbytes((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_const_charp header_ver = png_get_header_ver((png_const_structrp)png_ptr);
    (void)libver;
    (void)copyright_str;
    (void)header_ver;
    (void)rowbytes;

    // step 4: Cleanup
    png_struct *png_ptr_nonrestrict = png_ptr;
    png_info *info_ptr_nonrestrict = info_ptr;
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}