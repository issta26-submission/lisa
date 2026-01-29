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
//<ID> 377
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

    // step 2: Configure
    png_set_compression_strategy((png_structrp)png_ptr, 1);
    png_bytep exif_buf = (png_bytep)png_malloc_warn((png_const_structrp)png_ptr, 128);
    memset(exif_buf, 0x5A, (size_t)128);
    png_set_eXIf_1((png_const_structrp)png_ptr, info_ptr, (png_uint_32)1, exif_buf);
    png_set_unknown_chunk_location((png_const_structrp)png_ptr, info_ptr, 0, 1);
    png_set_mDCV_fixed((png_const_structrp)png_ptr, info_ptr,
                       (png_fixed_point)10000, (png_fixed_point)10000,
                       (png_fixed_point)34000, (png_fixed_point)16000,
                       (png_fixed_point)13250, (png_fixed_point)34500,
                       (png_fixed_point)7500,  (png_fixed_point)3000,
                       (png_uint_32)1000000, (png_uint_32)10000);

    // step 3: Operate / Validate
    double white_x = 0.0;
    double white_y = 0.0;
    double red_x = 0.0;
    double red_y = 0.0;
    double green_x = 0.0;
    double green_y = 0.0;
    double blue_x = 0.0;
    double blue_y = 0.0;
    double max_lum = 0.0;
    double min_lum = 0.0;
    png_uint_32 mdcv_ret = png_get_mDCV((png_const_structrp)png_ptr, (png_const_inforp)info_ptr,
                                       &white_x, &white_y,
                                       &red_x, &red_y,
                                       &green_x, &green_y,
                                       &blue_x, &blue_y,
                                       &max_lum, &min_lum);
    (void)mdcv_ret;
    double luminance_sum = white_x + white_y + red_x + red_y + green_x + green_y + blue_x + blue_y + max_lum + min_lum;
    int new_location = (int)(((long) (luminance_sum > 0.0 ? 1 : 0)) & 0x1);
    png_set_unknown_chunk_location((png_const_structrp)png_ptr, info_ptr, 0, new_location);
    png_set_compression_strategy((png_structrp)png_ptr, 2);

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, (png_voidp)exif_buf);
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);

    // API sequence test completed successfully
    return 66;
}