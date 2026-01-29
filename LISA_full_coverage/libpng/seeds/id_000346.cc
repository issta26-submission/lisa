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
//<ID> 346
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    void *user_mem = malloc(64);
    memset(user_mem, 0x5A, 64);
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);

    // step 2: Configure
    void *error_ctx = malloc(32);
    memset(error_ctx, 0xAA, 32);
    png_set_error_fn((png_structrp)png_ptr, error_ctx, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_alloc_size_t profile_size = (png_alloc_size_t)32;
    png_voidp profile = png_calloc((png_const_structrp)png_ptr, profile_size);
    memset(profile, 0xBB, (size_t)profile_size);
    png_set_iCCP((png_const_structrp)png_ptr, info_ptr, (png_const_charp)"ICCProfile", 0, (png_const_bytep)profile, (png_uint_32)profile_size);

    // step 3: Operate / Validate
    png_const_charp ver = png_get_libpng_ver((png_const_structrp)png_ptr);
    ((unsigned char *)profile)[0] = (unsigned char)ver[0];

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, profile);
    free(error_ctx);
    free(user_mem);
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);

    // API sequence test completed successfully
    (void)ver;
    return 66;
}