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
//<ID> 287
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    void *user_mem = malloc(128);
    memset(user_mem, 0xA5, 128);
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_set_benign_errors((png_structrp)png_ptr, 1);
    png_set_check_for_invalid_index((png_structrp)png_ptr, 0);
    png_set_filter((png_structrp)png_ptr, 0, 0);
    png_set_crc_action((png_structrp)png_ptr, 1, 1);
    png_set_bgr((png_structrp)png_ptr);
    png_set_invert_alpha((png_structrp)png_ptr);
    png_set_invert_mono((png_structrp)png_ptr);
    png_voidp block1 = png_calloc((png_const_structrp)png_ptr, (png_alloc_size_t)32);
    png_voidp block2 = png_malloc_warn((png_const_structrp)png_ptr, (png_alloc_size_t)16);
    png_uint_32 cache_max = png_get_chunk_cache_max((png_const_structrp)png_ptr);
    png_const_charp ver = png_get_header_ver((png_const_structrp)png_ptr);
    png_const_charp copy = png_get_copyright((png_const_structrp)png_ptr);
    jmp_buf *jb = png_set_longjmp_fn((png_structrp)png_ptr, NULL, sizeof(jmp_buf));
    png_free((png_const_structrp)png_ptr, block1);
    png_free((png_const_structrp)png_ptr, block2);
    free(user_mem);
    (void)cache_max;
    (void)ver;
    (void)copy;
    (void)jb;
    return 66;
}