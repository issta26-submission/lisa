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
//<ID> 214
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_const_charp copyright = png_get_copyright((png_const_structrp)png_ptr);
    unsigned char scratch[16];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Configure
    png_color_16 bg;
    bg.index = 0;
    bg.red = 0x0100u;
    bg.green = 0x0200u;
    bg.blue = 0x0300u;
    bg.gray = 0;
    png_set_bKGD((png_const_structrp)png_ptr, (png_inforp)NULL, (png_const_color_16p)&bg);
    png_set_alpha_mode((png_structrp)png_ptr, 1, 2.2);

    // step 3: Operate / Validate
    char p0[] = "paramA";
    char p1[] = "paramB";
    png_charpp params = (png_charpp)NULL;
    png_charp params_arr[2];
    params_arr[0] = p0;
    params_arr[1] = p1;
    params = params_arr;
    png_set_pCAL((png_const_structrp)png_ptr, (png_inforp)NULL, copyright, (png_int_32)0, (png_int_32)1024, 1, 2, (png_const_charp)"units", params);
    unsigned char mix = (unsigned char)copyright[0];
    scratch[0] = (unsigned char)(mix ^ (unsigned char)(bg.red & 0xFFu));
    scratch[1] = (unsigned char)((bg.green >> 8) & 0xFFu);

    // step 4: Cleanup
    (void)scratch;
    (void)params;
    (void)png_ptr;
    // API sequence test completed successfully
    return 66;
}