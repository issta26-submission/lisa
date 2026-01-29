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
//<ID> 467
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_voidp buffer = png_malloc((png_const_structrp)png_ptr, (png_alloc_size_t)16);
    memset(buffer, 0, (size_t)16);

    // step 2: Configure
    png_byte signature[8] = {137u, 80u, 78u, 71u, 13u, 10u, 26u, 10u};
    int sig_result = png_sig_cmp((png_const_bytep)signature, (size_t)0, (size_t)8);
    png_set_strip_alpha((png_structrp)png_ptr);
    png_set_alpha_mode((png_structrp)png_ptr, sig_result, 2.2);

    // step 3: Operate / Validate
    png_const_charp ver = png_get_libpng_ver((png_const_structrp)png_ptr);
    ((unsigned char *)buffer)[0] = (unsigned char)(ver ? ver[0] : 0);
    png_voidp memptr = png_get_mem_ptr((png_const_structrp)png_ptr);
    (void)memptr;

    // step 4: Cleanup
    png_free((png_const_structrp)png_ptr, buffer);

    // API sequence test completed successfully
    (void)ver;
    (void)sig_result;
    return 66;
}