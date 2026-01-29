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
//<ID> 479
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_voidp info_mem = png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)256);
    png_inforp info_ptr = (png_inforp)info_mem;
    png_bytep exif = (png_bytep)png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)64);
    memset(exif, 0, (size_t)64);

    // step 2: Configure
    png_set_eXIf((png_const_structrp)png_ptr, info_ptr, exif);
    png_set_filter((png_structrp)png_ptr, 0, 0x1F);

    // step 3: Operate / Validate
    png_color_16 background_storage;
    memset(&background_storage, 0, sizeof(background_storage));
    png_color_16p background_ptr = &background_storage;
    png_uint_32 bkgd_flags = png_get_bKGD((png_const_structrp)png_ptr, info_ptr, &background_ptr);
    png_bytep scratch = (png_bytep)png_malloc_warn((png_const_structrp)png_ptr, (png_alloc_size_t)8);
    memset(scratch, 0, (size_t)8);
    png_save_int_32((png_bytep)scratch, (png_int_32)bkgd_flags);
    (void)background_ptr;
    (void)background_storage;

    // step 4: Cleanup
    png_free((png_const_structrp)png_ptr, scratch);
    png_free((png_const_structrp)png_ptr, exif);
    png_free((png_const_structrp)png_ptr, info_mem);
    png_free((png_const_structrp)png_ptr, (png_voidp)png_ptr);

    // API sequence test completed successfully
    return 66;
}