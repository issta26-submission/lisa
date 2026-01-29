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
//<ID> 338
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    void *user_mem = malloc(128);
    memset(user_mem, 0xA5, 128);
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = (png_inforp)NULL;

    // step 2: Configure
    png_set_user_limits((png_structrp)png_ptr, (png_uint_32)1024, (png_uint_32)768);
    png_set_crc_action((png_structrp)png_ptr, 1, 1);

    // step 3: Operate / Validate
    png_const_charp ver = png_get_libpng_ver((png_const_structrp)png_ptr);
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);
    png_voidp buffer = png_calloc((png_const_structrp)png_ptr, (png_alloc_size_t)128);
    memset(buffer, 0x5A, (size_t)128);
    ((unsigned char *)buffer)[0] = (unsigned char)(chunk_max & 0xFFu);
    ((unsigned char *)buffer)[1] = ver ? (unsigned char)ver[0] : (unsigned char)0;

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, buffer);
    png_free_data((png_const_structrp)png_ptr, info_ptr, (png_uint_32)0xFFFFFFFFu, 0);
    free(user_mem);
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}