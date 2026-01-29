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
//<ID> 182
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = (png_inforp)NULL;

    // step 2: Configure / Query limits
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);
    png_uint_32 user_height_max = png_get_user_height_max((png_const_structrp)png_ptr);

    // step 3: Operate / Validate
    size_t buffer_size = (size_t)chunk_max + (size_t)4096;
    unsigned char * buffer = (unsigned char *)malloc(buffer_size);
    memset(buffer, 0, buffer_size);
    buffer[0] = (unsigned char)((user_height_max ^ (png_uint_32)chunk_max) & 0xFF);
    png_set_sig_bytes((png_structrp)png_ptr, 1);

    // step 4: Cleanup
    free(buffer);
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}