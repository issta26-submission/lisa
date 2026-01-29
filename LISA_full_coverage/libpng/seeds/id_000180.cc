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
//<ID> 180
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_alloc_size_t max_chunk = png_get_chunk_malloc_max((png_const_structrp)png_ptr);

    // step 2: Configure
    png_set_compression_buffer_size((png_structrp)png_ptr, (size_t)max_chunk);
    png_benign_error((png_const_structrp)png_ptr, (png_const_charp)"benign: setting compression buffer");

    // step 3: Operate / Validate
    png_uint_32 image_h = png_get_image_height((png_const_structrp)png_ptr, (png_const_inforp)NULL);
    png_alloc_size_t buf_size = (png_alloc_size_t)(max_chunk + 16);
    png_voidp buffer = png_malloc_warn((png_const_structrp)png_ptr, buf_size);
    memset(buffer, 0xA5, (size_t)buf_size);
    unsigned int mix = ((unsigned int)image_h) ^ ((unsigned int)buf_size) ^ ((unsigned int)((unsigned char *)buffer)[0]);
    ((unsigned char *)buffer)[0] = (unsigned char)(mix & 0xFF);

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, buffer);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)NULL, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}