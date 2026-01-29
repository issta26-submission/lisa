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
//<ID> 155
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_set_strip_16((png_structrp)png_ptr);
    png_voidp warn_buf = png_malloc_warn((png_const_structrp)png_ptr, (png_alloc_size_t)128);

    // step 2: Configure
    png_uint_32 width = 2;
    png_uint_32 height = 2;
    int bit_depth = 16;
    int color_type = 2; /* RGB */
    int interlace_method = 0;
    int compression_method = 0;
    int filter_method = 0;
    png_set_IHDR((png_structrp)png_ptr, info_ptr, width, height, bit_depth, color_type, interlace_method, compression_method, filter_method);
    memset(warn_buf, 0xAA, 128);

    // step 3: Operate / Validate
    png_charp purpose = (png_charp)NULL;
    png_int_32 X0 = 0;
    png_int_32 X1 = 0;
    int pcal_type = 0;
    int pcal_nparams = 0;
    png_charp units = (png_charp)NULL;
    png_charpp params = (png_charpp)NULL;
    png_uint_32 pcal_ret = png_get_pCAL((png_const_structrp)png_ptr, info_ptr, &purpose, &X0, &X1, &pcal_type, &pcal_nparams, &units, &params);
    png_const_charp copyr = png_get_copyright((png_const_structrp)png_ptr);
    unsigned int mix = (unsigned int)pcal_ret ^ (unsigned int)X0 ^ (unsigned int)X1 ^ (unsigned int)pcal_type ^ (unsigned int)pcal_nparams ^ (unsigned int)(unsigned char)copyr[0];
    (void)mix;

    // step 4: Cleanup
    free(warn_buf);
    png_infop info_ptr_local = info_ptr;
    png_destroy_info_struct((png_const_structrp)png_ptr, &info_ptr_local);

    // API sequence test completed successfully
    return 66;
}