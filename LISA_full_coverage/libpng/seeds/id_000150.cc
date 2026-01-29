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
//<ID> 150
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

    // step 3: Operate / Validate
    png_voidp buffer = png_malloc_warn((png_const_structrp)png_ptr, (png_alloc_size_t)128);
    memset(buffer, 0xAB, 128);

    png_charp purpose = (png_charp)NULL;
    png_int_32 X0 = 0;
    png_int_32 X1 = 0;
    int type = 0;
    int nparams = 0;
    png_charp units = (png_charp)NULL;
    png_charpp params = (png_charpp)NULL;
    png_uint_32 pcal_len = png_get_pCAL((png_const_structrp)png_ptr, (png_inforp)info_ptr, &purpose, &X0, &X1, &type, &nparams, &units, &params);

    png_const_charp copy = png_get_copyright((png_const_structrp)png_ptr);

    int mix = (int)pcal_len ^ (int)X0 ^ type ^ nparams ^ (int)(copy ? copy[0] : 0) ^ (int)*((unsigned char*)buffer);
    (void)mix;

    // step 4: Cleanup
    free(buffer);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = (png_info *)info_ptr;
    png_destroy_write_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict);

    // API sequence test completed successfully
    return 66;
}