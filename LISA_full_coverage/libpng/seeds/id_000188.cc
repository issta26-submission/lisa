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
//<ID> 188
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);

    // step 2: Configure
    png_set_compression_buffer_size((png_structrp)png_ptr, (size_t)8192);
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);

    // step 3: Operate / Validate
    png_benign_error((png_const_structrp)png_ptr, (png_const_charp)"benign warning from test sequence");
    png_uint_32 image_h = png_get_image_height((png_const_structrp)png_ptr, (png_const_inforp)NULL);
    size_t alloc_size = (size_t)chunk_max + (size_t)16;
    unsigned char *buffer = (unsigned char *)malloc(alloc_size);
    memset(buffer, 0, alloc_size);
    buffer[0] = (unsigned char)((image_h ^ (png_uint_32)chunk_max) & 0xFF);

    // step 4: Cleanup
    free(buffer);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)NULL, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}