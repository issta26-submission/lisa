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
//<ID> 405
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
    png_voidp mem = (png_voidp)NULL;

    // step 2: Configure
    png_set_option((png_structrp)png_ptr, 3, 1);
    png_set_mDCV_fixed((png_const_structrp)png_ptr, (png_inforp)info_ptr,
                      (png_fixed_point)31270, (png_fixed_point)32900,   // white x,y
                      (png_fixed_point)6400,  (png_fixed_point)3300,    // red x,y
                      (png_fixed_point)3000,  (png_fixed_point)6000,    // green x,y
                      (png_fixed_point)1500,  (png_fixed_point)600,     // blue x,y
                      (png_uint_32)10000000U, (png_uint_32)0U);

    // step 3: Operate / Validate
    mem = png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)64);
    png_byte pass = png_get_current_pass_number((png_const_structrp)png_ptr);
    png_uint_32 width = png_get_image_width((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    *((png_uint_32 *)mem) = (png_uint_32)((png_uint_32)pass + width);
    png_free_default((png_const_structrp)png_ptr, mem);
    mem = (png_voidp)NULL;

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}