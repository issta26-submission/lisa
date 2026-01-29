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
//<ID> 378
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);

    // step 2: Setup / Allocate EXIF buffer and configure compression
    png_uint_32 exif_size = (png_uint_32)128;
    png_bytep exif_buf = (png_bytep)png_malloc_warn((png_const_structrp)png_ptr, (png_alloc_size_t)exif_size);
    memset((void *)exif_buf, 0xAB, (size_t)exif_size);
    png_set_compression_strategy((png_structrp)png_ptr, 1);

    // step 3: Operate / Attach EXIF and set unknown chunk location, then query mDCV
    png_set_eXIf_1((png_const_structrp)png_ptr, info_ptr, exif_size, exif_buf);
    png_set_unknown_chunk_location((png_const_structrp)png_ptr, info_ptr, 0, 1);
    double white_x = 0.0, white_y = 0.0;
    double red_x = 0.0, red_y = 0.0;
    double green_x = 0.0, green_y = 0.0;
    double blue_x = 0.0, blue_y = 0.0;
    double mastering_max = 0.0, mastering_min = 0.0;
    png_uint_32 mdcv_ret = png_get_mDCV((png_const_structrp)png_ptr, (png_const_inforp)info_ptr,
                                       &white_x, &white_y, &red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y,
                                       &mastering_max, &mastering_min);
    (void)mdcv_ret;
    (void)white_x; (void)white_y; (void)red_x; (void)red_y; (void)green_x; (void)green_y; (void)blue_x; (void)blue_y;
    (void)mastering_max; (void)mastering_min;

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, (png_voidp)exif_buf);
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);

    return 66;
    // API sequence test completed successfully
}