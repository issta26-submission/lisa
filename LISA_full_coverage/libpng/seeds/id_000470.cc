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
//<ID> 470
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_bytep exif = (png_bytep)png_malloc((png_const_structrp)png_ptr, (png_alloc_size_t)16);
    memset(exif, 0, (size_t)16);
    png_color_16p background = (png_color_16p)png_malloc((png_const_structrp)png_ptr, (png_alloc_size_t)sizeof(png_color_16));
    memset((png_voidp)background, 0, (size_t)sizeof(png_color_16));
    png_inforp info_ptr = (png_inforp)png_malloc((png_const_structrp)png_ptr, (png_alloc_size_t)1);

    // step 2: Configure
    png_set_filter((png_structrp)png_ptr, 0, 0);

    // step 3: Operate / Validate
    png_uint_32 exif_len = png_get_eXIf((png_const_structrp)png_ptr, info_ptr, &exif);
    (void)exif_len;
    png_uint_32 bkgd_flags = png_get_bKGD((png_const_structrp)png_ptr, info_ptr, &background);
    exif[0] = (png_byte)(bkgd_flags & 0xFFU);
    exif[1] = (png_byte)((bkgd_flags >> 8) & 0xFFU);
    (void)bkgd_flags;

    // step 4: Cleanup
    png_free((png_const_structrp)png_ptr, (png_voidp)background);
    png_free((png_const_structrp)png_ptr, (png_voidp)exif);
    png_free((png_const_structrp)png_ptr, (png_voidp)info_ptr);
    png_free((png_const_structrp)png_ptr, (png_voidp)png_ptr);

    // API sequence test completed successfully
    return 66;
}