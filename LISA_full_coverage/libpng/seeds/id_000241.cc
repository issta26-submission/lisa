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
//<ID> 241
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = (png_inforp)NULL;
    png_time ptime;
    time_t now = (time_t)1618033988;
    png_uint_32 io_state = png_get_io_state((png_const_structrp)png_ptr);
    png_uint_32 chunk_type = png_get_io_chunk_type((png_const_structrp)png_ptr);

    // step 2: Configure
    png_set_tRNS_to_alpha((png_structrp)png_ptr);
    png_set_unknown_chunk_location((png_const_structrp)png_ptr, (png_inforp)info_ptr, (int)(chunk_type & 0x7FFFFFFFu), (int)(io_state & 0x7FFFFFFFu));
    png_convert_from_time_t(&ptime, now);

    // step 3: Operate / Validate
    png_start_read_image((png_structrp)png_ptr);
    png_uint_32 current_row = png_get_current_row_number((png_const_structrp)png_ptr);
    unsigned char buf[4];
    memset(buf, 0, sizeof(buf));
    png_save_uint_32((png_bytep)buf, current_row);

    // step 4: Cleanup
    png_destroy_info_struct((png_const_structrp)png_ptr, (png_infopp)&info_ptr);

    // API sequence test completed successfully
    return 66;
}