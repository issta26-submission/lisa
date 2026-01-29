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
//<ID> 200
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
    png_const_charp header_ver = png_get_header_version((png_const_structrp)png_ptr);
    unsigned char buf[4];
    buf[0] = (unsigned char)header_ver[0];
    buf[1] = (unsigned char)header_ver[1];
    buf[2] = (unsigned char)header_ver[2];
    buf[3] = (unsigned char)header_ver[3];

    // step 2: Configure
    int opt_ret1 = png_set_option((png_structrp)png_ptr, 0, 1);

    // step 3: Operate / Validate
    png_uint_32 v31 = png_get_uint_31((png_const_structrp)png_ptr, (png_const_bytep)buf);
    png_charp purpose = (png_charp)NULL;
    png_int_32 X0 = 0;
    png_int_32 X1 = 0;
    int type = 0;
    int nparams = 0;
    png_charp units = (png_charp)NULL;
    png_charpp params = (png_charpp)NULL;
    png_uint_32 pcal_ret = png_get_pCAL((png_const_structrp)png_ptr, info_ptr, &purpose, &X0, &X1, &type, &nparams, &units, &params);
    int opt_ret2 = png_set_option((png_structrp)png_ptr, 0, 0);
    png_uint_32 combined = v31 ^ pcal_ret ^ (png_uint_32)opt_ret1 ^ (png_uint_32)opt_ret2;
    buf[0] = (unsigned char)(combined & 0xFFu);

    // step 4: Cleanup
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);

    // API sequence test completed successfully
    return 66;
}