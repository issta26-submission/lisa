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
//<ID> 114
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpng_api_sequence() {
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    FILE *fp = tmpfile();
    png_init_io((png_structrp)png_ptr, fp);
    png_const_charp verstr = png_get_libpng_ver((png_const_structrp)png_ptr);
    size_t data_len = (size_t)((unsigned char)verstr[0] + 4u);
    png_uint_32 ver_first_byte = (png_uint_32)(unsigned char)verstr[0];
    unsigned char *data = (unsigned char *)malloc(data_len);
    memset(data, 0, data_len);
    png_byte chunk_name[4] = { 't', 'E', 'X', 't' };
    int reset_res = png_reset_zstream((png_structrp)png_ptr);
    data[0] = (unsigned char)ver_first_byte;
    data[1] = (unsigned char)(reset_res & 0xFF);
    data[2] = (unsigned char)((ver_first_byte ^ (png_uint_32)reset_res) & 0xFF);
    png_write_chunk((png_structrp)png_ptr, (png_const_bytep)chunk_name, (png_const_bytep)data, data_len);
    png_uint_32 checksum = ver_first_byte ^ (png_uint_32)data[0] ^ (png_uint_32)reset_res;
    (void)checksum;
    free(data);
    fclose(fp);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    return 66;
}