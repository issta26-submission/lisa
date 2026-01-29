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
//<ID> 238
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_voidp scratch = png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)64);

    // step 2: Configure
    int strategy = (int)(png_access_version_number() & 3u);
    png_set_compression_strategy((png_structrp)png_ptr, strategy);
    png_set_gray_to_rgb((png_structrp)png_ptr);
    png_set_tRNS_to_alpha((png_structrp)png_ptr);

    // step 3: Operate / Validate
    memset(scratch, 0xA5, 64);
    (void)strategy;
    (void)info_ptr;

    // step 4: Cleanup
    png_free((png_const_structrp)png_ptr, scratch);
    png_free((png_const_structrp)png_ptr, (png_voidp)info_ptr);
    png_free((png_const_structrp)png_ptr, (png_voidp)png_ptr);

    // API sequence test completed successfully
    return 66;
}