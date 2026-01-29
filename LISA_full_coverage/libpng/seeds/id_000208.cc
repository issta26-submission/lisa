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
//<ID> 208
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    const png_const_charp version_string = (png_const_charp)"1.6.37";
    png_structp png_ptr = png_create_read_struct(version_string, (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    unsigned char buf[4];
    memset(buf, 0, sizeof(buf));

    // step 2: Configure
    int option_result = png_set_option((png_structrp)png_ptr, 0x1, 1);
    png_const_charp header_ver = png_get_header_version((png_const_structrp)png_ptr);

    // step 3: Operate / Validate
    buf[0] = 0x12u;
    buf[1] = 0x34u;
    buf[2] = 0x56u;
    buf[3] = 0x78u;
    png_uint_32 uint31 = png_get_uint_31((png_const_structrp)png_ptr, (png_const_bytep)buf);

    png_charp purpose = (png_charp)NULL;
    png_int_32 X0 = 0;
    png_int_32 X1 = 0;
    int pcal_type = 0;
    int pcal_nparams = 0;
    png_charp units = (png_charp)NULL;
    png_charpp params = (png_charpp)NULL;
    png_uint_32 pcal_ret = png_get_pCAL((png_const_structrp)png_ptr, (png_inforp)NULL, &purpose, &X0, &X1, &pcal_type, &pcal_nparams, &units, &params);

    unsigned int mix = (unsigned int)(uint31 ^ pcal_ret ^ (png_uint_32)option_result);
    if (header_ver) { /* no-op to use header_ver in a straight-line flow */ }
    buf[0] ^= (unsigned char)(mix & 0xFFu);

    // step 4: Cleanup
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}