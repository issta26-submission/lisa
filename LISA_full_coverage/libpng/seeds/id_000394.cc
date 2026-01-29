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
//<ID> 394
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_info *info_ptr = (png_info *)NULL;
    png_byte rowbuf[256];
    memset(rowbuf, 0xAA, sizeof(rowbuf));
    png_byte *row_ptr = rowbuf;

    // step 2: Configure (use available configuration APIs)
    png_set_user_limits((png_structrp)png_ptr, (png_uint_32)8192, (png_uint_32)8192);
    png_set_expand_gray_1_2_4_to_8((png_structrp)png_ptr);

    // step 3: Operate / Validate (query state; avoid calling read_row to prevent invalid read)
    png_voidp io_ptr = png_get_io_ptr((png_const_structrp)png_ptr);
    png_byte filter_type = png_get_filter_type((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_uint_32 img_width = png_get_image_width((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_uint_32 combined = img_width + (png_uint_32)filter_type;
    png_set_user_limits((png_structrp)png_ptr, combined + 1, (png_uint_32)4096);
    png_bytepp rows = png_get_rows((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    (void)io_ptr; (void)filter_type; (void)img_width; (void)combined; (void)row_ptr; (void)rows;

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);
    return 66; // API sequence test completed successfully
}