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
//<ID> 203
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_const_charp header_ver = png_get_header_version((png_const_structrp)png_ptr);
    unsigned char buf[4];
    memset(buf, 0, sizeof(buf));
    buf[0] = (unsigned char)header_ver[0];
    buf[1] = (unsigned char)header_ver[1];
    buf[2] = (unsigned char)header_ver[2];
    buf[3] = (unsigned char)header_ver[3];
    png_uint_32 u31 = png_get_uint_31((png_const_structrp)png_ptr, (png_const_bytep)buf);

    // step 2: Configure
    int opt = 1;
    png_set_option((png_structrp)png_ptr, opt, (int)(u31 & 1u));

    // step 3: Operate / Validate
    png_charp purpose = (png_charp)NULL;
    png_int_32 X0 = 0;
    png_int_32 X1 = 0;
    int pcal_type = 0;
    int pcal_nparams = 0;
    png_charp units = (png_charp)NULL;
    png_charpp params = (png_charpp)NULL;
    png_uint_32 pcal_ret = png_get_pCAL((png_const_structrp)png_ptr, (png_inforp)NULL, &purpose, &X0, &X1, &pcal_type, &pcal_nparams, &units, &params);
    unsigned char buf2[4];
    buf2[0] = (unsigned char)((pcal_ret >> 24) & 0xFFu);
    buf2[1] = (unsigned char)((pcal_ret >> 16) & 0xFFu);
    buf2[2] = (unsigned char)((pcal_ret >> 8) & 0xFFu);
    buf2[3] = (unsigned char)(pcal_ret & 0xFFu);
    png_uint_32 u31_2 = png_get_uint_31((png_const_structrp)png_ptr, (png_const_bytep)buf2);
    (void)u31_2;
    png_const_charp header_ver2 = png_get_header_version((png_const_structrp)png_ptr);
    (void)header_ver2;
    (void)purpose;
    (void)X0;
    (void)X1;
    (void)pcal_type;
    (void)pcal_nparams;
    (void)units;
    (void)params;

    // step 4: Cleanup
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)NULL);
    // API sequence test completed successfully
    return 66;
}