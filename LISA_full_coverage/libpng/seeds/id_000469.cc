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
//<ID> 469
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_byte signature[8] = { 0x89, 'P', 'N', 'G', '\r', '\n', 0x1A, '\n' };
    png_voidp scratch = png_malloc((png_const_structrp)png_ptr, (png_alloc_size_t)16);
    memset(scratch, 0, (size_t)16);

    // step 2: Configure
    png_set_strip_alpha((png_structrp)png_ptr);
    png_set_alpha_mode((png_structrp)png_ptr, 1, 2.2);

    // step 3: Operate / Validate
    int sig_check = png_sig_cmp((png_const_bytep)signature, (size_t)0, (size_t)8);
    png_const_charp ver = png_get_libpng_ver((png_const_structrp)png_ptr);
    png_save_uint_16((png_bytep)scratch, (unsigned int)(sig_check & 0xFFFF));
    (void)ver;
    (void)sig_check;

    // step 4: Cleanup
    png_free((png_const_structrp)png_ptr, scratch);
    png_free((png_const_structrp)png_ptr, (png_voidp)png_ptr);

    // API sequence test completed successfully
    return 66;
}