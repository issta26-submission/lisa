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
//<ID> 115
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_uint_32 ver_num = (png_uint_32)png_access_version_number();
    size_t data_len = (size_t)((ver_num & 0xFFu) + 8u);
    png_byte *chunk_data = (png_byte *)malloc(data_len);
    memset(chunk_data, 0, data_len);
    png_byte chunk_name_buf[4];
    chunk_name_buf[0] = (png_byte)'t';
    chunk_name_buf[1] = (png_byte)'E';
    chunk_name_buf[2] = (png_byte)'S';
    chunk_name_buf[3] = (png_byte)'T';

    // step 2: Configure
    int reset_result = png_reset_zstream((png_structrp)png_ptr);
    chunk_data[0] = (png_byte)(ver_num & 0xFFu);
    chunk_data[1] = (png_byte)((ver_num >> 8) & 0xFFu);
    chunk_data[2] = (png_byte)(reset_result & 0xFF);

    // step 3: Operate / Validate
    // Avoid calling libpng write callbacks (which are not set for a read struct).
    // Report chunk info as a benign chunk-related message instead of attempting to write.
    char msg[128];
    snprintf(msg, sizeof msg, "Chunk %.4s bytes: %u,%u,%u", (char*)chunk_name_buf, (unsigned)chunk_data[0], (unsigned)chunk_data[1], (unsigned)chunk_data[2]);
    png_chunk_benign_error((png_const_structrp)png_ptr, (png_const_charp)msg);
    png_uint_32 observed_ver = (png_uint_32)png_access_version_number();
    png_uint_32 checksum = ver_num ^ observed_ver ^ (png_uint_32)chunk_data[0] ^ (png_uint_32)chunk_data[2];
    (void)checksum;

    // step 4: Cleanup
    free(chunk_data);
    png_destroy_read_struct(&png_ptr_nonrestrict, (png_infopp)NULL, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}