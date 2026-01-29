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
//<ID> 460
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
    png_const_charp initial_ver = png_get_libpng_ver((png_const_structrp)png_ptr);
    unsigned char signature[8];
    memset(signature, 0x00, sizeof(signature));

    // step 2: Configure
    png_set_alpha_mode((png_structrp)png_ptr, 1, 2.2);
    png_set_strip_alpha((png_structrp)png_ptr);

    // step 3: Operate / Validate
    int sigcmp_result = png_sig_cmp((png_const_bytep)signature, 0, (size_t)sizeof(signature));
    png_set_sig_bytes((png_structrp)png_ptr, sigcmp_result);
    png_const_charp ver_after_cfg = png_get_libpng_ver((png_const_structrp)png_ptr);
    (void)initial_ver;
    (void)ver_after_cfg;
    (void)sigcmp_result;

    // step 4: Cleanup
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);
    // API sequence test completed successfully
    return 66;
}