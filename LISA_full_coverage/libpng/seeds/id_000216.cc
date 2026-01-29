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
//<ID> 216
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_const_charp copyr = png_get_copyright((png_const_structrp)png_ptr);
    png_color_16 background;
    background.index = (png_byte)0;
    background.red = (png_uint_16)0x1234u;
    background.green = (png_uint_16)0x2345u;
    background.blue = (png_uint_16)0x3456u;
    background.gray = (png_uint_16)0x0077u;
    char purpose[] = "Calibration";
    char units[] = "units";
    char param0[] = "param1";
    char *params[1];
    params[0] = param0;

    // step 2: Configure
    png_set_bKGD((png_const_structrp)png_ptr, (png_inforp)NULL, (png_const_color_16p)&background);
    png_set_alpha_mode((png_structrp)png_ptr, 2, 1.0);
    png_set_pCAL((png_const_structrp)png_ptr, (png_inforp)NULL, (png_const_charp)purpose, (png_int_32)0, (png_int_32)100, (int)1, (int)1, (png_const_charp)units, (png_charpp)params);

    // step 3: Operate / Validate
    unsigned char buf[8];
    memset(buf, 0xAB, sizeof(buf));
    buf[0] = (unsigned char)((background.red) & 0xFFu);
    buf[1] = (unsigned char)((background.green) & 0xFFu);
    buf[2] = (unsigned char)copyr[0];
    png_color_16 bg_out;
    png_color_16 *bg_out_ptr = &bg_out;
    png_uint_32 got_bkgd = png_get_bKGD((png_const_structrp)png_ptr, (png_inforp)NULL, &bg_out_ptr);
    buf[3] = (unsigned char)((bg_out_ptr->blue) & 0xFFu);
    (void)got_bkgd;
    (void)buf;

    // step 4: Cleanup
    // API sequence test completed successfully
    return 66;
}