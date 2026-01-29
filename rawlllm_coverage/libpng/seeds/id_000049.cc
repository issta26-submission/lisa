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
//<ID> 340
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    void *user_mem = malloc(64);
    memset(user_mem, 0x6A, 64);
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);

    // step 2: Configure
    png_const_charp ver = png_get_libpng_ver((png_const_structrp)png_ptr);
    png_set_error_fn((png_structrp)png_ptr, user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_alloc_size_t proflen = (png_alloc_size_t)32u;
    png_voidp profile_v = png_calloc((png_const_structrp)png_ptr, proflen);
    memset(profile_v, 0xAB, (size_t)proflen);
    ((unsigned char *)profile_v)[0] = (unsigned char)ver[0];
    png_set_iCCP((png_const_structrp)png_ptr, info_ptr, (png_const_charp)"ICC Profile", 0, (png_const_bytep)profile_v, (png_uint_32)proflen);

    // step 3: Operate / Validate
    ((unsigned char *)profile_v)[1] = (unsigned char)ver[1];
    png_const_charp ver2 = png_get_libpng_ver((png_const_structrp)png_ptr);
    (void)ver2;

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, profile_v);
    free(user_mem);
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);

    // API sequence test completed successfully
    return 66;
}