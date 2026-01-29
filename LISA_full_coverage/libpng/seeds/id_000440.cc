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
//<ID> 440
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
    unsigned char signature[8];
    memset(signature, 0, sizeof(signature));
    png_voidp tmp_mem = png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)16);

    // step 2: Configure
    png_set_IHDR((png_const_structrp)png_ptr, info_ptr, (png_uint_32)640U, (png_uint_32)480U, 8, 2, 0, 0, 0);

    // step 3: Operate / Validate
    png_const_bytep sig_ptr = png_get_signature((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    memcpy(signature, sig_ptr, sizeof(signature));
    png_uint_32 io_state = png_get_io_state((png_const_structrp)png_ptr);
    ((unsigned char *)tmp_mem)[0] = (unsigned char)(io_state & 0xFFU);
    png_free_default((png_const_structrp)png_ptr, tmp_mem);

    // step 4: Cleanup
    png_free_data((png_const_structrp)png_ptr, info_ptr, (png_uint_32)0xFFFFFFFFU, 0);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    (void)signature;
    (void)io_state;
    return 66;
}