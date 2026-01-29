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
//<ID> 291
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_alloc_size_t initial_size = (png_alloc_size_t)256;
    png_voidp buffer = png_malloc_default((png_const_structrp)png_ptr, initial_size);
    memset(buffer, 0xAA, (size_t)initial_size);

    // step 2: Configure
    png_set_sCAL_s((png_const_structrp)png_ptr, info_ptr, 1, (png_const_charp)"1.0", (png_const_charp)"1.0");

    // step 3: Operate / Validate
    png_const_charp ver = png_get_header_ver((png_const_structrp)png_ptr);
    png_uint_32 cache_max = png_get_chunk_cache_max((png_const_structrp)png_ptr);
    (void)ver;
    (void)cache_max;

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, buffer);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    // API sequence test completed successfully
    return 66;
}