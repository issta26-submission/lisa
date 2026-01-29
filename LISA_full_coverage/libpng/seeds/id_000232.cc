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
//<ID> 232
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
    png_voidp buffer = png_malloc_warn((png_const_structrp)png_ptr, (png_alloc_size_t)256);
    memset(buffer, 0x5A, 256);

    // step 2: Configure
    png_set_compression_strategy((png_structrp)png_ptr, 2);
    png_set_packing((png_structrp)png_ptr);
    png_set_bgr((png_structrp)png_ptr);
    png_set_compression_mem_level((png_structrp)png_ptr, 8);
    png_set_sig_bytes((png_structrp)png_ptr, 0);
    png_set_check_for_invalid_index((png_structrp)png_ptr, 1);
    png_set_unknown_chunk_location((png_const_structrp)png_ptr, info_ptr, 1, 1);
    png_set_chunk_cache_max((png_structrp)png_ptr, (png_uint_32)1024);

    // step 3: Operate / Validate (query a couple of safe-to-call getters)
    png_set_option((png_structrp)png_ptr, 0, 1);
    png_const_charp ver = png_get_header_version((png_const_structrp)png_ptr);
    (void)ver;
    png_uint_32 cache_max = png_get_chunk_cache_max((png_const_structrp)png_ptr);
    (void)cache_max;

    // step 4: Cleanup
    png_free((png_const_structrp)png_ptr, buffer);
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);

    // API sequence test completed successfully
    return 66;
}