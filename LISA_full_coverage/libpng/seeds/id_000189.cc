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
//<ID> 189
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);

    // step 2: Configure
    size_t comp_buf_size = (size_t)(chunk_max + (png_alloc_size_t)64);
    png_set_compression_buffer_size((png_structrp)png_ptr, comp_buf_size);

    // step 3: Operate / Validate
    png_uint_32 image_h = png_get_image_height((png_const_structrp)png_ptr, (png_const_inforp)NULL);
    size_t alloc_size = (size_t)((chunk_max & 0xFFu) + 32u);
    unsigned char *work_buf = (unsigned char *)malloc(alloc_size);
    memset(work_buf, 0x5A, alloc_size);
    work_buf[0] = (unsigned char)((work_buf[0] ^ (unsigned char)(image_h & 0xFFu) ^ (unsigned char)(chunk_max & 0xFFu)) & 0xFFu);
    png_benign_error((png_const_structrp)png_ptr, (png_const_charp)"benign warning: test sequence");

    // step 4: Cleanup
    free(work_buf);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)NULL, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}