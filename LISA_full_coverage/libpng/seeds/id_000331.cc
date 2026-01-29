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
//<ID> 331
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    void *user_mem = malloc(128);
    memset(user_mem, 0xAB, 128);
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = (png_inforp)NULL;

    // step 2: Configure
    png_const_charp ver = png_get_libpng_ver((png_const_structrp)png_ptr);
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);
    png_alloc_size_t alloc_size = chunk_max + (png_alloc_size_t)16;

    // step 3: Operate / Validate
    void *buffer = malloc((size_t)alloc_size);
    memset(buffer, 0x5A, (size_t)alloc_size);
    ((unsigned char *)buffer)[0] = (unsigned char)ver[0];
    png_free_default((png_const_structrp)png_ptr, buffer);
    png_free_data((png_const_structrp)png_ptr, info_ptr, (png_uint_32)0, 0);

    // step 4: Cleanup
    free(user_mem);
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    (void)chunk_max;
    (void)ver;
    return 66;
}