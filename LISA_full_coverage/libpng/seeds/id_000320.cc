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
//<ID> 320
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
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = png_create_info_struct(png_ptr);

    // step 2: Configure
    png_set_IHDR(png_ptr, info_ptr, (png_uint_32)1024, (png_uint_32)768, 8, 2, 0, 0, 0);
    png_set_bgr(png_ptr);

    // step 3: Operate / Validate
    unsigned char buf[4];
    buf[0] = 0x12; buf[1] = 0x34; buf[2] = 0x56; buf[3] = 0x78;
    png_uint_32 val = png_get_uint_31(png_ptr, (png_const_bytep)buf);
    ((unsigned char *)user_mem)[0] = (unsigned char)(val & 0xFF);
    ((unsigned char *)user_mem)[1] = (unsigned char)((val >> 8) & 0xFF);
    ((unsigned char *)user_mem)[2] = (unsigned char)((val >> 16) & 0xFF);
    ((unsigned char *)user_mem)[3] = (unsigned char)((val >> 24) & 0xFF);

    // step 4: Cleanup
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);
    free(user_mem);
    (void)val;
    return 66;
}