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
//<ID> 491
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
    png_inforp info_ptr = (png_inforp)NULL;
    png_byte buf[4];
    memset(buf, 0, (size_t)4);

    // step 2: Configure
    png_set_expand_16((png_structrp)png_ptr);
    png_set_sRGB((png_const_structrp)png_ptr, info_ptr, 1);
    png_set_alpha_mode((png_structrp)png_ptr, 0, 1.0);

    // step 3: Operate / Validate
    buf[0] = 0x7F;
    buf[1] = 0xFF;
    buf[2] = 0xAA;
    buf[3] = 0x55;
    png_uint_32 v = png_get_uint_32((png_const_bytep)buf);
    png_set_alpha_mode((png_structrp)png_ptr, (int)(v & 3u), (double)(v % 1000u) / 1000.0);

    // step 4: Cleanup
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);

    // API sequence test completed successfully
    return 66;
}