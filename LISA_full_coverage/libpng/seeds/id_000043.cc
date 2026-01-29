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
//<ID> 43
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structrp png_ptr = png_create_read_struct_2((png_const_charp)NULL,
                                                    (png_voidp)NULL,
                                                    (png_error_ptr)NULL,
                                                    (png_error_ptr)NULL,
                                                    (png_voidp)NULL,
                                                    (png_malloc_ptr)NULL,
                                                    (png_free_ptr)NULL);
    png_const_charp lib_ver = png_get_header_version((png_const_structrp)png_ptr);

    // step 2: Configure libpng (some basic configuration calls)
    png_set_user_limits(png_ptr, (png_uint_32)1024, (png_uint_32)768);
    png_set_filter(png_ptr, 0, 0);
    png_set_swap(png_ptr);
    png_set_expand_gray_1_2_4_to_8(png_ptr);
    png_set_filler(png_ptr, (png_uint_32)0xFFFF, 0);

    // step 3: Operate (use returned string in a harmless way)
    (void)lib_ver;

    // step 4: Validate / Cleanup using available APIs (avoid png_destroy_read_struct)
    png_data_freer((png_const_structrp)png_ptr, (png_inforp)NULL, 1, (png_uint_32)0xFFFFFFFFu);
    png_free_data((png_const_structrp)png_ptr, (png_inforp)NULL, (png_uint_32)0xFFFFFFFFu, 0);

    // API sequence test completed successfully
    return 66;
}