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
//<ID> 118
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
static void write_noop(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}
static void flush_noop(png_structp png_ptr) {
    (void)png_ptr;
}

int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_uint_32 ver_num = png_access_version_number();
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    size_t chunk_len = (size_t)((ver_num & 0xFFu) + 8u);
    png_byte *chunk_name = (png_byte *)malloc((size_t)5);
    memset(chunk_name, 0, (size_t)5);
    png_byte *chunk_data = (png_byte *)malloc(chunk_len);
    memset(chunk_data, 0, chunk_len);

    // step 2: Configure
    chunk_name[0] = (png_byte)'T';
    chunk_name[1] = (png_byte)'E';
    chunk_name[2] = (png_byte)'S';
    chunk_name[3] = (png_byte)'T';
    chunk_data[0] = (png_byte)(ver_num & 0xFFu);
    chunk_data[1] = (png_byte)((ver_num >> 8) & 0xFFu);
    chunk_data[2] = (png_byte)((ver_num >> 16) & 0xFFu);
    int reset_res = png_reset_zstream((png_structrp)png_ptr);
    chunk_data[3] = (png_byte)(reset_res & 0xFF);

    // ensure a non-NULL write/flush callback is registered to avoid a NULL write function error
    png_set_write_fn((png_structrp)png_ptr, (png_voidp)NULL, (png_rw_ptr)write_noop, (png_flush_ptr)flush_noop);

    // step 3: Operate / Validate
    png_write_chunk((png_structrp)png_ptr, (png_const_bytep)chunk_name, (png_const_bytep)chunk_data, chunk_len);
    png_uint_32 checksum = ver_num ^ (png_uint_32)reset_res ^ (png_uint_32)chunk_data[0];
    (void)checksum;

    // step 4: Cleanup
    free(chunk_data);
    free(chunk_name);
    png_destroy_read_struct(&png_ptr_nonrestrict, (png_infopp)NULL, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}