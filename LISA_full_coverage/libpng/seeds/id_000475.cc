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
//<ID> 475
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Create write struct and prepare buffers
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = (png_inforp)NULL;

    png_byte exif_data[32];
    memset(exif_data, 0x45, (size_t)32);

    // step 2: Configure using available APIs
    png_set_eXIf_1((png_const_structrp)png_ptr, info_ptr, (png_uint_32)32, (png_bytep)exif_data);
    png_set_compression_level((png_structrp)png_ptr, 6);

    // step 3: Operate / Validate
    png_color_16 color;
    memset(&color, 0, (size_t)sizeof(color));
    png_color_16p bg_ptr = &color;
    png_uint_32 bkgd_flags = png_get_bKGD((png_const_structrp)png_ptr, info_ptr, &bg_ptr);

    png_byte scratch[4];
    memset(scratch, 0, (size_t)4);
    png_save_uint_32((png_bytep)scratch, (png_uint_32)bkgd_flags);

    (void)bg_ptr;
    (void)bkgd_flags;

    // step 4: Cleanup
    png_destroy_write_struct(&png_ptr, (png_infopp)NULL);

    return 66;
}