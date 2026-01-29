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
//<ID> 281
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    void *user_mem = malloc(128);
    memset(user_mem, 0x5A, 128);
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);

    // step 2: Configure
    png_set_read_status_fn((png_structrp)png_ptr, (png_read_status_ptr)NULL);
    png_const_charp ver = png_get_libpng_ver((png_const_structrp)png_ptr);
    (void)ver; // keep the value to mimic configuration usage

    // step 3: Operate / Validate
    png_uint_32 pixels_per_inch = png_get_y_pixels_per_inch((png_const_structrp)png_ptr, (png_const_inforp)NULL);
    unsigned char *um = (unsigned char *)user_mem;
    um[0] = (unsigned char)(pixels_per_inch & 0xFF);
    um[1] = (unsigned char)((pixels_per_inch >> 8) & 0xFF);
    um[2] = (unsigned char)((pixels_per_inch >> 16) & 0xFF);
    um[3] = (unsigned char)((pixels_per_inch >> 24) & 0xFF);

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    free(user_mem);

    // API sequence test completed successfully
    return 66;
}