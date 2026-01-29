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
//<ID> 297
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    void *user_mem = malloc(128);
    memset(user_mem, 0xAA, 128);
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_uint_32 cache_before = png_get_chunk_cache_max((png_const_structrp)png_ptr);
    png_const_charp header_ver = png_get_header_ver((png_const_structrp)png_ptr);
    png_voidp info_mem = png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)512);
    png_inforp info_ptr = (png_inforp)info_mem;
    png_info_init_3((png_infopp)&info_ptr, (size_t)512);

    // step 2: Configure
    char swidth[] = "1024.0";
    char sheight[] = "768.0";
    png_set_sCAL_s((png_const_structrp)png_ptr, info_ptr, 1, (png_const_charp)swidth, (png_const_charp)sheight);
    png_uint_32 cache_after = png_get_chunk_cache_max((png_const_structrp)png_ptr);

    // step 3: Operate / Validate
    png_voidp temp_buf = png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)64);
    memset(temp_buf, 0x55, 64);
    int out_unit = 0;
    double out_width = 0.0;
    double out_height = 0.0;
    png_uint_32 scal_ret = png_get_sCAL((png_const_structrp)png_ptr, (png_const_inforp)info_ptr, &out_unit, &out_width, &out_height);

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, temp_buf);
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);
    free(user_mem);

    // API sequence test completed successfully
    (void)cache_before;
    (void)header_ver;
    (void)cache_after;
    (void)scal_ret;
    (void)out_unit;
    (void)out_width;
    (void)out_height;
    return 66;
}