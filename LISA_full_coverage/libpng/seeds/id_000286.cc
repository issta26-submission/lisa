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
//<ID> 286
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
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", (png_voidp)user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);

    unsigned char *buf = (unsigned char *)user_mem;

    // step 2: Configure
    png_set_error_fn((png_structrp)png_ptr, (png_voidp)user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);

    // step 3: Operate / Validate
    png_uint_32 pixels_per_meter = png_get_pixels_per_meter((png_const_structrp)png_ptr, (png_const_inforp)NULL);
    buf[0] = (unsigned char)(pixels_per_meter & 0xFFu);
    buf[1] = (unsigned char)((pixels_per_meter >> 8) & 0xFFu);
    buf[2] = (unsigned char)((pixels_per_meter >> 16) & 0xFFu);
    buf[3] = (unsigned char)((pixels_per_meter >> 24) & 0xFFu);

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    free(user_mem);

    // API sequence test completed successfully
    return 66;
}