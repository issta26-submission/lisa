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
//<ID> 361
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Initialize
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_alloc_size_t data_size = (png_alloc_size_t)32;
    png_voidp raw_mem = png_malloc_default((png_const_structrp)png_ptr, data_size);
    memset(raw_mem, 0xA5, (size_t)data_size);

    // Use a valid non-zero location for the unknown chunk to satisfy libpng's requirements.
    png_unknown_chunk unknown = { { 'u', 'N', 'K', 'N', 0 }, (png_byte*)raw_mem, (size_t)data_size, (png_byte)1 };
    png_const_unknown_chunkp unknowns = &unknown;

    // step 2: Configure
    png_set_unknown_chunks((png_const_structrp)png_ptr, info_ptr, unknowns, 1);
    png_set_text_compression_level((png_structrp)png_ptr, 6);

    // step 3: Operate / Validate
    png_const_charp ver = png_get_header_version((png_const_structrp)png_ptr);
    ((unsigned char*)unknown.data)[0] = (unsigned char)ver[0];

    // step 4: Cleanup
    png_free((png_const_structrp)png_ptr, raw_mem);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    (void)ver;
    return 66;
}