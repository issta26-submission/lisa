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
//<ID> 280
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    void *user_mem = malloc(128);
    memset(user_mem, 0xA5, 128);
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    unsigned char chunk_name[4] = { 't', 'E', 'X', 't' };
    png_uint_32 chunk_length = (png_uint_32)7;
    png_uint_32 ppm_value = 0;
    unsigned char *buf = (unsigned char *)user_mem;

    png_set_write_fn((png_structrp)png_ptr, (png_voidp)user_mem, NULL, NULL);
    png_set_benign_errors((png_structrp)png_ptr, 1);

    ppm_value = png_get_pixels_per_meter((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    buf[0] = (unsigned char)(ppm_value & 0xFF);
    buf[1] = (unsigned char)((ppm_value >> 8) & 0xFF);
    buf[2] = (unsigned char)((ppm_value >> 16) & 0xFF);
    buf[3] = (unsigned char)((ppm_value >> 24) & 0xFF);

    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);
    memset(user_mem, 0x00, 128);
    free(user_mem);

    (void)chunk_name;
    (void)chunk_length;
    (void)ppm_value;
    return 66;
}