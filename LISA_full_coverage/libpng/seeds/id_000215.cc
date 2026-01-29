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
//<ID> 215
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // Step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)0, (png_error_ptr)0, (png_error_ptr)0);
    png_const_charp cprt = png_get_copyright((png_const_structrp)png_ptr);
    char first_char = cprt[0];

    png_color_16 bg;
    memset(&bg, 0, sizeof(bg));
    bg.red = (png_uint_16)0x1234u;
    bg.green = (png_uint_16)0x2345u;
    bg.blue = (png_uint_16)0x3456u;
    bg.gray = (png_uint_16)0x4567u;
    bg.index = (png_byte)5u;

    // Step 2: Configure
    png_set_alpha_mode(png_ptr, 1, 0.45455);
    png_const_charp purpose = "Calibration";
    png_int_32 X0 = 0;
    png_int_32 X1 = 100;
    int pcal_type = 1;
    int pcal_nparams = 2;
    png_const_charp units = "units";
    png_charpp params = (png_charpp)png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)(2 * sizeof(char *)));
    params[0] = (png_charp)"1.23";
    params[1] = (png_charp)"4.56";
    png_set_pCAL((png_const_structrp)png_ptr, (png_inforp)0, purpose, X0, X1, pcal_type, pcal_nparams, units, params);

    // Step 3: Operate / Validate
    png_set_bKGD((png_const_structrp)png_ptr, (png_inforp)0, (png_const_color_16p)&bg);
    int mix = ((int)first_char) ^ (int)(X1 & 0xFF);
    (void)mix;

    // Step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, (png_voidp)params);
    // API sequence test completed successfully
    return 66;
}