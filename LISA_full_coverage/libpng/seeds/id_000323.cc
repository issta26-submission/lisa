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
//<ID> 323
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpng_api_sequence() {
    /* step 1: Declarations / Initialize */
    void *user_mem = malloc(128);
    memset(user_mem, 0xA5, 128);

    png_structp png_ptr = png_create_read_struct_2(
        (png_const_charp)"1.6.37",
        user_mem,
        (png_error_ptr)NULL,
        (png_error_ptr)NULL,
        user_mem,
        (png_malloc_ptr)NULL,
        (png_free_ptr)NULL
    );

    /* step 2: Configure (benign, won't require the PNG header) */
    png_set_sig_bytes(png_ptr, 0);
    png_set_user_limits(png_ptr, (png_uint_32)2000, (png_uint_32)2000);

    /* step 3: Operate / Validate */
    png_uint_32 width = (png_uint_32)800;
    unsigned char buf[4];
    buf[0] = (unsigned char)((width >> 24) & 0xFF);
    buf[1] = (unsigned char)((width >> 16) & 0xFF);
    buf[2] = (unsigned char)((width >> 8) & 0xFF);
    buf[3] = (unsigned char)(width & 0xFF);

    png_int_32 parsed_width = png_get_int_32((png_const_bytep)buf);

    ((unsigned char *)user_mem)[0] = (unsigned char)(parsed_width & 0xFF);
    ((unsigned char *)user_mem)[1] = (unsigned char)((parsed_width >> 8) & 0xFF);
    ((unsigned char *)user_mem)[2] = (unsigned char)((parsed_width >> 16) & 0xFF);
    ((unsigned char *)user_mem)[3] = (unsigned char)((parsed_width >> 24) & 0xFF);

    /* step 4: Cleanup */
    free(user_mem);

    (void)png_ptr;
    (void)width;
    (void)parsed_width;
    return 66;
}