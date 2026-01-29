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
//<ID> 423
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Initialize
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_uint_32 cache_max = png_get_chunk_cache_max((png_const_structrp)png_ptr);

    // step 2: Configure
    png_set_compression_strategy(png_ptr, 1);

    // step 3: Operate / Validate
    png_write_flush(png_ptr);
    png_uint_32 io_state = png_get_io_state((png_const_structrp)png_ptr);
    size_t buf_size = (size_t)(cache_max % 1024U + 64U);
    char *buffer = new char[buf_size];
    memset(buffer, (int)(io_state & 0xFFU), buf_size);
    png_write_flush(png_ptr);

    // step 4: Cleanup
    delete [] buffer;
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}