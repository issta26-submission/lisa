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
//<ID> 494
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct_2(
        (png_const_charp)"1.6.37",
        (png_voidp)NULL,
        (png_error_ptr)NULL,
        (png_error_ptr)NULL,
        (png_voidp)NULL,
        (png_malloc_ptr)NULL,
        (png_free_ptr)NULL
    );
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_byte buf[4];
    memset(buf, 0, 4);

    // step 2: Configure
    png_set_strip_16((png_structrp)png_ptr);
    png_set_sRGB((png_const_structrp)png_ptr, info_ptr, 1);

    // step 3: Operate / Validate
    buf[0] = 0x7F; buf[1] = 0xFF; buf[2] = 0xFF; buf[3] = 0xFF;
    png_uint_32 parsed = png_get_uint_32((png_const_bytep)buf);
    png_save_uint_32((png_bytep)buf, parsed);

    // step 4: Cleanup
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);

    // API sequence test completed successfully
    return 66;
}