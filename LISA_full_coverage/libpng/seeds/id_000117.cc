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
//<ID> 117
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_uint_32 ver_num = png_access_version_number();
    png_structp png_ptr = png_create_write_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_byte *chunk_name = (png_byte *)malloc((size_t)4);
    memset(chunk_name, 0, (size_t)4);
    png_byte *data = (png_byte *)malloc((size_t)8);
    memset(data, 0, (size_t)8);

    // step 2: Configure
    chunk_name[0] = (png_byte)'t';
    chunk_name[1] = (png_byte)'E';
    chunk_name[2] = (png_byte)'S';
    chunk_name[3] = (png_byte)'T';
    data[0] = (png_byte)(ver_num & 0xFFu);
    data[1] = (png_byte)((ver_num >> 8) & 0xFFu);
    data[2] = (png_byte)((ver_num >> 16) & 0xFFu);
    data[3] = (png_byte)((ver_num >> 24) & 0xFFu);
    data[4] = 0xA5u;
    data[5] = 0x5Au;
    data[6] = 0xC3u;
    data[7] = 0x3Cu;
    int reset_rc = png_reset_zstream((png_structrp)png_ptr);
    (void)reset_rc;

    // step 3: Operate / Validate
    FILE *tmp = tmpfile();
    png_init_io((png_structrp)png_ptr, tmp);
    png_write_chunk_start((png_structrp)png_ptr, (png_const_bytep)chunk_name, (png_uint_32)8);
    (void)data; // data is prepared but not passed to any unavailable write-data API

    // step 4: Cleanup
    fclose(tmp);
    free(chunk_name);
    free(data);
    png_destroy_read_struct(&png_ptr_nonrestrict, (png_infopp)NULL, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}