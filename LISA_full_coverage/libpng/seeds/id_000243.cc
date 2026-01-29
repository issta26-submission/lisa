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
//<ID> 243
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    /* png_info is an incomplete type in the headers; avoid sizeof(png_info).
       Allocate a fixed-size block and use it as the info struct buffer. */
    png_inforp info_ptr = (png_inforp)png_malloc_warn((png_const_structrp)png_ptr, (png_alloc_size_t)256);
    memset((void *)info_ptr, 0, 256);
    png_info_init_3((png_infopp)&info_ptr, (size_t)256);
    png_time ptime;
    png_convert_from_time_t((png_timep)&ptime, (time_t)0x5AA55AA5);
    png_uint_32 width = (png_uint_32)800u;
    png_byte buf[4];
    png_save_uint_32((png_bytep)buf, width);

    // step 2: Configure
    png_set_unknown_chunk_location((png_const_structrp)png_ptr, (png_inforp)info_ptr, 3, 1);
    png_set_tRNS_to_alpha((png_structrp)png_ptr);

    // step 3: Operate / Validate
    png_start_read_image((png_structrp)png_ptr);
    png_uint_32 read_back = png_get_uint_32((png_const_bytep)buf);
    png_set_unknown_chunk_location((png_const_structrp)png_ptr, (png_inforp)info_ptr, (int)(read_back & 0xFFu), (int)((read_back >> 8) & 0xFFu));

    // step 4: Cleanup
    png_destroy_info_struct((png_const_structrp)png_ptr, (png_infopp)&info_ptr);

    // API sequence test completed successfully
    return 66;
}