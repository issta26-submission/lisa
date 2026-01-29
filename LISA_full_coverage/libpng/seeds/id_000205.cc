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
//<ID> 205
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = (png_inforp)NULL;

    // step 2: Configure
    png_const_charp ver = png_get_header_version((png_const_structrp)png_ptr);
    png_const_bytep ver_bytes = (png_const_bytep)ver;
    unsigned char buf[4];
    buf[0] = ver_bytes[0];
    buf[1] = ver_bytes[1];
    buf[2] = ver_bytes[2];
    buf[3] = ver_bytes[3];
    png_uint_32 u31 = png_get_uint_31((png_const_structrp)png_ptr, (png_const_bytep)buf);
    int opt_on = (int)(u31 & 1u);
    (void)png_set_option((png_structrp)png_ptr, 1, opt_on);

    // step 3: Operate / Validate
    png_charp purpose = (png_charp)NULL;
    png_int_32 X0 = 0;
    png_int_32 X1 = 0;
    int pcal_type = 0;
    int pcal_nparams = 0;
    png_charp units = (png_charp)NULL;
    png_charpp params = (png_charpp)NULL;
    (void)png_get_pCAL((png_const_structrp)png_ptr, info_ptr, &purpose, &X0, &X1, &pcal_type, &pcal_nparams, &units, &params);
    png_uint_32 check = png_get_uint_31((png_const_structrp)png_ptr, (png_const_bytep)buf);
    (void)png_set_option((png_structrp)png_ptr, 2, (int)((check ^ u31) & 1u));
    png_free_default((png_const_structrp)png_ptr, (png_voidp)purpose);
    png_free_default((png_const_structrp)png_ptr, (png_voidp)units);
    png_free_default((png_const_structrp)png_ptr, (png_voidp)params);

    // step 4: Cleanup
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);

    return 66;
    // API sequence test completed successfully
}