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
//<ID> 316
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);
    png_voidp scratch = png_malloc((png_const_structrp)png_ptr, (png_alloc_size_t)256);
    memset(scratch, 0xA5, (size_t)256);

    // step 2: Configure
    png_set_compression_buffer_size((png_structrp)png_ptr, (size_t)8192);
    png_set_tRNS_to_alpha((png_structrp)png_ptr);
    png_set_alpha_mode((png_structrp)png_ptr, 1, 2.2);
    png_set_bgr((png_structrp)png_ptr);

    // step 3: Operate / Validate
    png_const_charp ver = png_get_libpng_ver((png_const_structrp)png_ptr);
    png_alloc_size_t buf_size = chunk_max + (png_alloc_size_t)128;
    png_voidp buffer = png_malloc((png_const_structrp)png_ptr, buf_size);
    memset(buffer, 0x5A, (size_t)buf_size);
    ((unsigned char *)buffer)[0] = (unsigned char)ver[0];

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, buffer);
    png_free_default((png_const_structrp)png_ptr, scratch);
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    (void)ver;
    (void)chunk_max;
    (void)buf_size;
    return 66;
}