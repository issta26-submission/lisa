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
//<ID> 438
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_info *info_ptr = (png_info *)NULL;
    double file_gamma = 0.0;
    png_fixed_point y_offset = (png_fixed_point)0;
    png_uint_32 gamma_flag = (png_uint_32)0;

    // step 2: Configure
    png_set_chunk_cache_max((png_structrp)png_ptr, (png_uint_32)1024U);
    png_set_write_fn((png_structrp)png_ptr, (png_voidp)NULL, (png_rw_ptr)NULL, (png_flush_ptr)NULL);

    // step 3: Operate / Validate
    gamma_flag = png_get_gAMA((png_const_structrp)png_ptr, (png_const_inforp)info_ptr, &file_gamma);
    y_offset = png_get_y_offset_inches_fixed((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);
    (void)gamma_flag;
    (void)file_gamma;
    (void)y_offset;
    // API sequence test completed successfully
    return 66;
}