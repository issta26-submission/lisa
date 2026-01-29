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
//<ID> 307
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Initialize
    void *user_mem = malloc(256);
    memset(user_mem, 0x5A, 256);
    png_structp png_ptr = png_create_write_struct_2((png_const_charp)"1.6.37",
                                                   user_mem,
                                                   (png_error_ptr)NULL,
                                                   (png_error_ptr)NULL,
                                                   (png_voidp)user_mem,
                                                   (png_malloc_ptr)png_malloc_default,
                                                   (png_free_ptr)png_free_default);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);

    // step 2: Configure (use iCCP as a configuration step in place of the unavailable background API)
    unsigned char icc_profile[4] = {0x01, 0x02, 0x03, 0x04};
    png_set_iCCP((png_const_structrp)png_ptr, info_ptr, (png_const_charp)"ICC", 0, (png_const_bytep)icc_profile, (png_uint_32)sizeof(icc_profile));
    png_set_IHDR((png_const_structrp)png_ptr, info_ptr,
                 (png_uint_32)128, (png_uint_32)64,
                 8, 2, 0, 0, 0);

    // step 3: Operate / Validate
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);
    png_voidp chunk_buffer = png_malloc((png_const_structrp)png_ptr, chunk_max ? chunk_max : (png_alloc_size_t)64);
    memset(chunk_buffer, 0xA5, (size_t)(chunk_max ? chunk_max : (png_alloc_size_t)64));
    png_int_32 offset_x = 0;
    png_int_32 offset_y = 0;
    int unit_type = 0;
    png_get_oFFs((png_const_structrp)png_ptr, (png_const_inforp)info_ptr, &offset_x, &offset_y, &unit_type);
    ((unsigned char *)chunk_buffer)[0] = (unsigned char)(offset_x & 0xFF);

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, chunk_buffer);
    free(user_mem);

    (void)chunk_max;
    (void)offset_x;
    (void)offset_y;
    (void)unit_type;
    (void)png_ptr;
    (void)info_ptr;

    // API sequence test completed successfully
    return 66;
}