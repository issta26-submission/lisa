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
//<ID> 154
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

    // step 2: Configure
    png_set_strip_16((png_structrp)png_ptr);
    png_voidp buffer = png_malloc_warn((png_const_structrp)png_ptr, (png_alloc_size_t)128);
    memset(buffer, 0xAA, (size_t)128);

    // step 3: Operate / Validate
    png_charp purpose = (png_charp)NULL;
    png_int_32 X0 = 0;
    png_int_32 X1 = 0;
    int pcal_type = 0;
    int pcal_nparams = 0;
    png_charp units = (png_charp)NULL;
    png_charpp params = (png_charpp)NULL;
    png_uint_32 pcal_present = png_get_pCAL((png_const_structrp)png_ptr, info_ptr, &purpose, &X0, &X1, &pcal_type, &pcal_nparams, &units, &params);
    png_const_charp cp = png_get_copyright((png_const_structrp)png_ptr);
    unsigned int mix = (unsigned int)(pcal_present ^ (png_uint_32)X0 ^ (png_uint_32)X1 ^ (png_uint_32)pcal_type ^ (png_uint_32)pcal_nparams ^ (unsigned int)(unsigned char)cp[0] ^ (unsigned int)((unsigned char *)buffer)[0]);
    (void)mix;

    // step 4: Cleanup
    free(buffer);
    png_destroy_info_struct((png_const_structrp)png_ptr, &info_ptr);

    // API sequence test completed successfully
    return 66;
}