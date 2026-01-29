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
//<ID> 443
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
    png_uint_32 width = (png_uint_32)640U;
    png_uint_32 height = (png_uint_32)480U;
    int bit_depth = 8;
    int color_type = 2;
    int interlace_method = 0;
    int compression_method = 0;
    int filter_method = 0;

    // step 2: Configure
    png_set_IHDR((png_const_structrp)png_ptr, (png_inforp)info_ptr, width, height, bit_depth, color_type, interlace_method, compression_method, filter_method);
    png_voidp sig_buf = png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)8);
    memset(sig_buf, 0, (size_t)8);
    unsigned char local_buf[8];
    memset(local_buf, 0, sizeof(local_buf));

    // step 3: Operate / Validate
    png_const_bytep sig = png_get_signature((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_uint_32 io_state = png_get_io_state((png_const_structrp)png_ptr);
    png_save_uint_32((png_bytep)sig_buf, width);
    png_save_uint_32((png_bytep)local_buf, io_state);
    unsigned char first_sig_byte = sig[0];
    (void)first_sig_byte;

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, sig_buf);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    (void)local_buf;
    (void)io_state;
    return 66;
}