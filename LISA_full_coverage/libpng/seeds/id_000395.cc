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
//<ID> 395
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_info *info_ptr = (png_info *)NULL;
    png_byte buffer[256];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Configure
    png_set_filter((png_structrp)png_ptr, 0, 0);
    png_set_compression_level((png_structrp)png_ptr, 6);
    png_set_strip_16((png_structrp)png_ptr);

    // step 3: Operate / Validate
    png_byte filter_type = png_get_filter_type((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_uint_32 x_ppm = png_get_x_pixels_per_meter((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_voidp io_ptr = png_get_io_ptr((png_const_structrp)png_ptr);
    unsigned int fill_val = (unsigned int)(x_ppm & 0xFFu) ^ (unsigned int)filter_type;
    memset(buffer, (int)(fill_val & 0xFFu), sizeof(buffer));
    (void)io_ptr;
    (void)filter_type;
    (void)x_ppm;
    (void)fill_val;

    // step 4: Cleanup
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);

    // API sequence test completed successfully
    return 66;
}