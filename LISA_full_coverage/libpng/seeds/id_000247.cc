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
//<ID> 247
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
    png_time time_val;
    time_t sample_time = (time_t)1618033988;
    png_byte buf[4] = {0};
    png_uint_32 saved_value = 0;

    // step 2: Configure
    png_set_unknown_chunk_location((png_const_structrp)png_ptr, info_ptr, 1, 2);
    png_set_tRNS_to_alpha((png_structrp)png_ptr);

    // step 3: Operate / Validate
    png_start_read_image((png_structrp)png_ptr);
    png_convert_from_time_t((png_timep)&time_val, sample_time);
    saved_value = png_get_current_row_number((png_const_structrp)png_ptr);
    png_save_uint_32((png_bytep)buf, saved_value);
    png_uint_32 io_state = png_get_io_state((png_const_structrp)png_ptr);
    png_save_uint_32((png_bytep)buf, io_state);

    // step 4: Cleanup
    png_destroy_info_struct((png_const_structrp)png_ptr, (png_infopp)&info_ptr);

    // API sequence test completed successfully
    return 66;
}