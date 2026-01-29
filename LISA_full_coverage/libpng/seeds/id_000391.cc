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
//<ID> 391
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_info *info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_uint_32 initial_res_x = (png_uint_32)300;
    png_uint_32 initial_res_y = (png_uint_32)300;
    int unit_type = 1;
    png_byte buffer[8];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Configure
    png_set_pHYs((png_const_structrp)png_ptr, info_ptr, initial_res_x, initial_res_y, unit_type);
    png_set_filter((png_structrp)png_ptr, 0, 0);
    png_set_chunk_malloc_max((png_structrp)png_ptr, (png_alloc_size_t)4096);

    // step 3: Operate / Validate
    png_uint_32 x_ppm = png_get_x_pixels_per_meter((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_voidp io_ptr = png_get_io_ptr((png_const_structrp)png_ptr);
    png_byte filter_type = png_get_filter_type((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    buffer[0] = filter_type;
    buffer[1] = (png_byte)(x_ppm & 0xFFu);
    buffer[2] = (png_byte)((x_ppm >> 8) & 0xFFu);
    buffer[3] = (png_byte)((x_ppm >> 16) & 0xFFu);
    buffer[4] = (png_byte)((x_ppm >> 24) & 0xFFu);
    buffer[5] = (png_byte)(io_ptr != NULL);

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);
    // API sequence test completed successfully
    return 66;
}