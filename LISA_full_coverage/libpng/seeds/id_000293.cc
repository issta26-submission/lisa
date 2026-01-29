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
//<ID> 293
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct_2((png_const_charp)"1.6.37",
                                                   (png_voidp)NULL,
                                                   (png_error_ptr)NULL,
                                                   (png_error_ptr)NULL,
                                                   (png_voidp)NULL,
                                                   (png_malloc_ptr)NULL,
                                                   (png_free_ptr)NULL);
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_const_charp swidth = (png_const_charp)"0.5";
    png_const_charp sheight = (png_const_charp)"0.5";

    // step 2: Configure (use a compression-level setter available in the API list)
    png_set_compression_level((png_structrp)png_ptr, 6);

    // step 3: Operate / Validate
    png_uint_32 cache_max = png_get_user_width_max((png_const_structrp)png_ptr);
    png_const_charp ver = png_get_header_ver((png_const_structrp)png_ptr);
    png_voidp buffer = png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)128);
    memset(buffer, 0xAA, (size_t)128);
    ((unsigned char *)buffer)[0] = (unsigned char)(cache_max & 0xFF);
    ((unsigned char *)buffer)[1] = (unsigned char)ver[0];

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, buffer);
    png_free_default((png_const_structrp)png_ptr, (png_voidp)info_ptr);
    png_free_default((png_const_structrp)png_ptr, (png_voidp)png_ptr);
    (void)cache_max;
    (void)ver;
    (void)swidth;
    (void)sheight;
    // API sequence test completed successfully
    return 66;
}