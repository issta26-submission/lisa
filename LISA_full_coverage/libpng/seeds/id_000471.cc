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
//<ID> 471
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_uint_32 bkgd_flags = (png_uint_32)0;
    const size_t exif_size = (size_t)32;
    png_bytep exif_buf = (png_bytep)png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)exif_size);
    memset(exif_buf, 0x45, exif_size);
    png_color_16p bg_ptr = (png_color_16p)NULL;
    unsigned char saved_bytes[2] = {0, 0};

    // step 2: Configure
    png_set_filter((png_structrp)png_ptr, 0, 0);
    png_set_eXIf((png_const_structrp)png_ptr, (png_inforp)NULL, (png_bytep)exif_buf);

    // step 3: Operate / Validate
    bkgd_flags = png_get_bKGD((png_const_structrp)png_ptr, (png_inforp)NULL, &bg_ptr);
    png_save_uint_16((png_bytep)saved_bytes, (unsigned int)(bkgd_flags & 0xFFFFU));
    unsigned int reconstructed = ((unsigned int)saved_bytes[0] << 8) | (unsigned int)saved_bytes[1];
    png_set_filter((png_structrp)png_ptr, (int)(reconstructed & 0xFF), (int)((reconstructed >> 8) & 0xFF));

    // step 4: Cleanup
    png_free((png_const_structrp)png_ptr, (png_voidp)exif_buf);
    png_free((png_const_structrp)png_ptr, (png_voidp)png_ptr);

    // API sequence test completed successfully
    return 66;
}