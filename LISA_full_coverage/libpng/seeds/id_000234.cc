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
//<ID> 234
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);

    // step 2: Configure
    size_t initial_rowbytes = png_get_rowbytes((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    int strategy = (int)(initial_rowbytes & 3u);
    png_set_compression_strategy((png_structrp)png_ptr, strategy);
    png_set_tRNS_to_alpha((png_structrp)png_ptr);
    png_set_gray_to_rgb((png_structrp)png_ptr);

    // step 3: Operate / Validate
    png_alloc_size_t alloc_size = (png_alloc_size_t)(initial_rowbytes + 64u);
    png_voidp buffer = png_malloc_default((png_const_structrp)png_ptr, alloc_size);
    memset(buffer, 0x5Au, (size_t)alloc_size);

    // step 4: Cleanup
    png_free((png_const_structrp)png_ptr, buffer);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}