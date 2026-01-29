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
//<ID> 447
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_uint_32 width = (png_uint_32)800U;
    png_uint_32 height = (png_uint_32)600U;
    int bit_depth = 8;
    int color_type = 2; // Truecolor
    png_voidp buffer = png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)8U);
    memset(buffer, 0, (size_t)8U);

    // step 2: Configure
    png_set_IHDR((png_const_structrp)png_ptr, (png_inforp)info_ptr, width, height, bit_depth, color_type, 0, 0, 0);

    // step 3: Operate / Validate
    png_const_bytep signature = png_get_signature((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_uint_32 io_state = png_get_io_state((png_const_structrp)png_ptr);
    (void)io_state;
    if (signature) { /* silence unused-result in environments that check, but do not branch on it */ }
    memcpy(buffer, (const void*)signature, (size_t)8U);
    png_save_uint_32((png_bytep)buffer, width);
    png_free_default((png_const_structrp)png_ptr, buffer);

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}