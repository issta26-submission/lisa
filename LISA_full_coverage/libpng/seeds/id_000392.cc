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
//<ID> 392
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
    unsigned char local_buf[128];
    memset(local_buf, 0x00, sizeof local_buf);

    // step 2: Configure
    png_set_mem_fn((png_structrp)png_ptr, png_get_io_ptr((png_const_structrp)png_ptr), (png_malloc_ptr)NULL, (png_free_ptr)NULL);

    // step 3: Operate / Validate
    png_voidp io_ptr = png_get_io_ptr((png_const_structrp)png_ptr);
    png_byte filter_type = png_get_filter_type((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_uint_32 x_ppm = png_get_x_pixels_per_meter((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    unsigned int derived = (unsigned int)((filter_type & 0xFFu) + (x_ppm & 0xFFu));
    local_buf[0] = (unsigned char)(derived & 0xFFu);
    (void)io_ptr;
    (void)filter_type;
    (void)x_ppm;
    (void)derived;

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}