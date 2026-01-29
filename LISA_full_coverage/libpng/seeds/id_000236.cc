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
//<ID> 236
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

    // step 2: Configure
    png_set_compression_strategy((png_structrp)png_ptr, 1);
    png_set_gray_to_rgb((png_structrp)png_ptr);

    // step 3: Operate / Validate
    png_set_tRNS_to_alpha((png_structrp)png_ptr);
    png_set_IHDR((png_const_structrp)png_ptr, (png_inforp)info_ptr, (png_uint_32)320u, (png_uint_32)240u, 8, 2, 0, 0, 0);
    png_voidp scratch = png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)128);
    memset(scratch, 0xA5, 128);

    // step 4: Cleanup
    png_free((png_const_structrp)png_ptr, scratch);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}