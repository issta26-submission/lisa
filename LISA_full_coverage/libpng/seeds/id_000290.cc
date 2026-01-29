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
//<ID> 290
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct_2("1.6.37", NULL, NULL, NULL, NULL, NULL, NULL);
    png_const_charp header_ver = png_get_header_ver((png_const_structrp)png_ptr);
    png_uint_32 cache_max = png_get_user_width_max((png_const_structrp)png_ptr);

    // step 2: Setup / Configure
    png_voidp info_mem = png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)128);
    png_inforp info_ptr = (png_inforp)info_mem;
    png_set_sCAL_s((png_const_structrp)png_ptr, info_ptr, 1, (png_const_charp)"10.0", (png_const_charp)"20.0");

    // step 3: Operate / Validate
    png_alloc_size_t buf_size = (png_alloc_size_t)(((cache_max & 0xFFu) + 32u));
    png_voidp buf = png_malloc_default((png_const_structrp)png_ptr, buf_size);
    unsigned char seed = (unsigned char)header_ver[0];
    memset(buf, (int)seed, (size_t)buf_size);

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, buf);
    png_free_default((png_const_structrp)png_ptr, info_mem);
    png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);

    // API sequence test completed successfully
    (void)cache_max;
    (void)header_ver;
    (void)seed;
    return 66;
}