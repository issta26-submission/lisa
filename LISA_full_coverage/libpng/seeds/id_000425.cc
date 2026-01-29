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
//<ID> 425
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Initialize
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_uint_32 chunk_cache_max = png_get_chunk_cache_max((png_const_structrp)png_ptr);

    // step 2: Configure
    int strategy = (int)(chunk_cache_max & 3U);
    png_set_compression_strategy((png_structrp)png_ptr, strategy);

    // step 3: Operate / Validate
    unsigned char buffer[64];
    memset(buffer, strategy, sizeof(buffer));
    png_write_flush((png_structrp)png_ptr);
    png_uint_32 io_state = png_get_io_state((png_const_structrp)png_ptr);
    buffer[0] = (unsigned char)(io_state & 0xFFU);
    png_write_flush((png_structrp)png_ptr);

    // step 4: Cleanup
    png_set_compression_strategy((png_structrp)png_ptr, 0);
    png_write_flush((png_structrp)png_ptr);
    png_ptr = (png_structp)NULL;

    // API sequence test completed successfully
    return 66;
}