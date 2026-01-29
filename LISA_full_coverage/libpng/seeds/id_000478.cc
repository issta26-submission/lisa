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
//<ID> 478
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = (png_inforp)NULL;
    info_ptr = (png_inforp)png_create_info_struct((png_const_structrp)png_ptr);
    png_byte exif_data[32];
    memset(exif_data, 0, (size_t)sizeof(exif_data));
    png_color_16 background;
    png_color_16p bg_ptr = &background;

    // step 2: Configure
    png_set_eXIf((png_const_structrp)png_ptr, info_ptr, (png_bytep)exif_data);
    png_set_filter((png_structrp)png_ptr, 0, 0x0F);

    // step 3: Operate / Validate
    png_uint_32 bkgd_flags = png_get_bKGD((png_const_structrp)png_ptr, info_ptr, &bg_ptr);
    unsigned char saved[2];
    png_save_uint_16((png_bytep)saved, (unsigned int)(bkgd_flags & 0xFFFF));
    (void)bg_ptr;
    (void)saved;

    // step 4: Cleanup
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);

    // API sequence test completed successfully
    return 66;
}