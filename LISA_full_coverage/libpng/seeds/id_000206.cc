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
//<ID> 206
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_const_charp ver = png_get_header_version((png_const_structrp)png_ptr);

    // step 2: Configure
    unsigned char buf[4];
    buf[0] = ver[0];
    buf[1] = ver[1];
    buf[2] = ver[2];
    buf[3] = ver[3];
    png_uint_32 uval = png_get_uint_31((png_const_structrp)png_ptr, (png_const_bytep)buf);
    (void)uval;
    png_set_option((png_structrp)png_ptr, (int)(uval & 0xFFu), 1);

    // step 3: Operate / Validate
    png_charp purpose = (png_charp)NULL;
    png_int_32 X0 = 0;
    png_int_32 X1 = 0;
    int type = 0;
    int nparams = 0;
    png_charp units = (png_charp)NULL;
    png_charpp params = (png_charpp)NULL;
    png_uint_32 pcal = png_get_pCAL((png_const_structrp)png_ptr, (png_inforp)NULL, &purpose, &X0, &X1, &type, &nparams, &units, &params);
    png_set_option((png_structrp)png_ptr, (int)(pcal & 0xFFu), (int)((uval ^ pcal) & 1u));
    (void)purpose;
    (void)X0;
    (void)X1;
    (void)type;
    (void)nparams;
    (void)units;
    (void)params;

    // step 4: Cleanup
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}