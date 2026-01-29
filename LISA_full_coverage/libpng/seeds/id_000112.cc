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
//<ID> 112
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* No-op write callback to avoid "Call to NULL write function" runtime error. */
static void test_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

/* No-op flush callback. */
static void test_flush(png_structp png_ptr) {
    (void)png_ptr;
}

int test_libpng_api_sequence() {
    /* step 1: Declarations / Initialize */
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_uint_32 ver_num = png_access_version_number();

    /* Install a valid (no-op) write function so subsequent write calls do not error. */
    png_set_write_fn((png_structrp)png_ptr, (png_voidp)NULL, (png_rw_ptr)test_write_data, (png_flush_ptr)test_flush);

    /* step 2: Configure */
    png_byte chunk_name[5];
    chunk_name[0] = 't';
    chunk_name[1] = 'E';
    chunk_name[2] = 'S';
    chunk_name[3] = 'T';
    chunk_name[4] = 0;
    size_t data_len = (size_t)((ver_num & 0xFFu) + 8u);
    png_byte * data_buf = (png_byte *)malloc(data_len);
    memset(data_buf, 0, data_len);
    data_buf[0] = (png_byte)(ver_num & 0xFFu);
    data_buf[1] = (png_byte)((ver_num >> 8) & 0xFFu);
    data_buf[2] = (png_byte)((ver_num >> 16) & 0xFFu);

    /* step 3: Operate / Validate */
    png_write_chunk((png_structrp)png_ptr, (png_const_bytep)chunk_name, (png_const_bytep)data_buf, data_len);
    png_uint_32 checksum = ((png_uint_32)data_buf[0] | ((png_uint_32)data_buf[1] << 8)) ^ ver_num;
    (void)checksum;

    /* step 4: Cleanup */
    free(data_buf);
    png_destroy_read_struct(&png_ptr_nonrestrict, (png_infopp)NULL, (png_infopp)NULL);

    /* API sequence test completed successfully */
    return 66;
}