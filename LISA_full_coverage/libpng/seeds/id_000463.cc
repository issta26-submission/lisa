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
//<ID> 463
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    unsigned char signature[8];
    memset(signature, 0, (size_t)sizeof(signature));
    signature[0] = (unsigned char)0x89;
    signature[1] = (unsigned char)'P';
    signature[2] = (unsigned char)'N';
    signature[3] = (unsigned char)'G';
    signature[4] = (unsigned char)'\r';
    signature[5] = (unsigned char)'\n';
    signature[6] = (unsigned char)0x1A;
    signature[7] = (unsigned char)'\n';

    // step 2: Configure
    png_set_alpha_mode((png_structrp)png_ptr, 0, 1.0);
    png_set_strip_alpha((png_structrp)png_ptr);

    // step 3: Operate / Validate
    int sig_cmp = png_sig_cmp((png_const_bytep)signature, (size_t)0, (size_t)8);
    png_const_charp ver = png_get_libpng_ver((png_const_structrp)png_ptr);
    png_set_alpha_mode((png_structrp)png_ptr, (int)(sig_cmp & 1), (double)(sig_cmp + 1));
    (void)ver;
    (void)sig_cmp;

    // step 4: Cleanup
    png_free((png_const_structrp)png_ptr, (png_voidp)png_ptr);
    // API sequence test completed successfully
    return 66;
}