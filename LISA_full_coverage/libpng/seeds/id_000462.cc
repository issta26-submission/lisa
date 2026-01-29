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
//<ID> 462
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_byte signature[8] = {137U, 80U, 78U, 71U, 13U, 10U, 26U, 10U};
    png_voidp scratch = png_malloc((png_const_structrp)png_ptr, (png_alloc_size_t)32);
    memset(scratch, 0, (size_t)32);

    // step 2: Configure
    png_const_charp ver = png_get_libpng_ver((png_const_structrp)png_ptr);
    png_set_mem_fn((png_structrp)png_ptr, (png_voidp)ver, (png_malloc_ptr)NULL, (png_free_ptr)NULL);

    // step 3: Operate / Validate
    int sig_result = png_sig_cmp((png_const_bytep)signature, (size_t)0, (size_t)8);
    png_set_strip_alpha((png_structrp)png_ptr);
    png_set_alpha_mode((png_structrp)png_ptr, sig_result, 2.2);
    png_voidp mem_check = png_get_mem_ptr((png_const_structrp)png_ptr);
    (void)mem_check;

    // step 4: Cleanup
    png_free((png_const_structrp)png_ptr, scratch);
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    (void)ver;
    return 66;
}