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
//<ID> 318
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    void *user_mem = malloc(256);
    memset(user_mem, 0x5A, 256);
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);
    png_alloc_size_t buf_size = chunk_max + (png_alloc_size_t)256;
    png_voidp buffer = png_malloc((png_const_structrp)png_ptr, buf_size);

    // step 2: Configure
    png_set_compression_buffer_size((png_structrp)png_ptr, (size_t)buf_size);
    png_set_alpha_mode((png_structrp)png_ptr, 1, 2.2);
    png_set_bgr((png_structrp)png_ptr);
    png_set_tRNS_to_alpha((png_structrp)png_ptr);

    // step 3: Operate / Validate
    memset(buffer, 0xAA, (size_t)buf_size);
    png_const_charp ver = png_get_libpng_ver((png_const_structrp)png_ptr);
    ((unsigned char *)buffer)[0] = (unsigned char)(ver[0]);
    png_voidp user_transform = png_get_user_transform_ptr((png_const_structrp)png_ptr);
    (void)user_transform;

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, buffer);
    free(user_mem);
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}