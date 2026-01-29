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
//<ID> 269
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    void *user_mem = malloc(64);
    memset(user_mem, 0xA5, 64);
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);

    // step 2: Configure
    png_set_error_fn((png_structrp)png_ptr, (png_voidp)user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_set_compression_level((png_structrp)png_ptr, 6);

    // step 3: Operate / Validate
    png_uint_32 cache_max = png_get_chunk_cache_max((png_const_structrp)png_ptr);
    png_const_charp ver = png_get_header_version((png_const_structrp)png_ptr);
    unsigned char *buf = (unsigned char *)user_mem;
    buf[0] = (unsigned char)(cache_max & 0xFF);
    buf[1] = (unsigned char)((cache_max >> 8) & 0xFF);
    buf[2] = (unsigned char)ver[0];

    // step 4: Cleanup
    free(user_mem);

    // API sequence test completed successfully
    return 66;
}