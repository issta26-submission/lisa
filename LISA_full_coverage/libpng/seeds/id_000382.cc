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
//<ID> 382
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structrp png_ptr = (png_structrp)NULL;
    png_info *info_ptr = (png_info *)png_calloc((png_const_structrp)png_ptr, (png_alloc_size_t)1);

    // step 2: Configure
    png_const_charp ver = png_get_libpng_ver((png_const_structrp)png_ptr);
    png_set_write_status_fn((png_structrp)png_ptr, (png_write_status_ptr)NULL);
    png_set_mem_fn((png_structrp)png_ptr, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);

    // step 3: Operate / Validate
    png_warning((png_const_structrp)png_ptr, ver);
    png_uint_32 max_width = png_get_user_width_max((png_const_structrp)png_ptr);
    (void)max_width;
    png_const_charp header = png_get_header_ver((png_const_structrp)png_ptr);
    (void)header;

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}