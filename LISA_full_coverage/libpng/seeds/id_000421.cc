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
//<ID> 421
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Initialize
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);

    // step 2: Configure
    png_set_compression_strategy((png_structrp)png_ptr, 1);
    png_uint_32 cache_before = png_get_chunk_cache_max((png_const_structrp)png_ptr);

    // step 3: Operate / Validate
    png_uint_32 io_state = png_get_io_state((png_const_structrp)png_ptr);
    png_write_flush((png_structrp)png_ptr);
    png_uint_32 cache_after = png_get_chunk_cache_max((png_const_structrp)png_ptr);
    unsigned char buffer[12];
    png_uint_32 *buf32 = (png_uint_32 *)buffer;
    buf32[0] = cache_before;
    buf32[1] = io_state;
    buf32[2] = cache_after;
    memset(buffer + 11, 0x7F, 1);

    // step 4: Cleanup
    png_ptr = (png_structp)NULL;
    (void)buffer;
    (void)buf32;
    (void)cache_before;
    (void)io_state;
    (void)cache_after;
    // API sequence test completed successfully
    return 66;
}