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
//<ID> 299
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37",
                                                   (png_voidp)NULL,
                                                   (png_error_ptr)NULL,
                                                   (png_error_ptr)NULL,
                                                   (png_voidp)NULL,
                                                   (png_malloc_ptr)NULL,
                                                   (png_free_ptr)NULL);
    png_inforp info_ptr = (png_inforp)NULL;

    // step 2: Configure
    png_set_sCAL_s((png_const_structrp)png_ptr, info_ptr, 1, (png_const_charp)"100.0", (png_const_charp)"200.0");

    // step 3: Operate / Validate
    png_const_charp header_ver = png_get_header_version((png_const_structrp)png_ptr);
    png_alloc_size_t cache_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);
    void *data_buf = malloc((size_t)256);
    memset(data_buf, 0xAA, (size_t)256);

    // step 4: Cleanup
    free(data_buf);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    (void)header_ver;
    (void)cache_max;

    // API sequence test completed successfully
    return 66;
}