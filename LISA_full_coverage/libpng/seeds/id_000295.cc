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
//<ID> 295
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = (png_inforp)png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)1);
    memset(info_ptr, 0, (size_t)1);
    png_info_init_3((png_infopp)&info_ptr, 0);

    // step 2: Configure
    png_set_sCAL_s((png_const_structrp)png_ptr, info_ptr, 1, (png_const_charp)"2.50", (png_const_charp)"3.50");

    // step 3: Operate / Validate
    png_uint_32 cache_max = png_get_chunk_cache_max((png_const_structrp)png_ptr);
    png_const_charp ver = png_get_header_ver((png_const_structrp)png_ptr);
    png_voidp buffer = png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)128);
    memset(buffer, 0x7F, (size_t)128);
    (void)cache_max;
    (void)ver;

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, buffer);
    png_free_default((png_const_structrp)png_ptr, (png_voidp)info_ptr);
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}