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
//<ID> 128
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_color_8 true_bits;
    png_color_16 bg;
    memset(&true_bits, 0, sizeof(true_bits));
    memset(&bg, 0, sizeof(bg));
    true_bits.red = (png_byte)8;
    true_bits.green = (png_byte)8;
    true_bits.blue = (png_byte)8;
    bg.red = (png_uint_16)0x0100;
    bg.green = (png_uint_16)0x0200;
    bg.blue = (png_uint_16)0x0300;

    // step 2: Configure
    png_set_shift((png_structrp)png_ptr, (png_const_color_8p)&true_bits);
    png_set_bKGD((png_const_structrp)png_ptr, (png_inforp)NULL, (png_const_color_16p)&bg);
    png_const_charp purpose = (png_const_charp)"Calibration";
    png_int_32 X0 = (png_int_32)0;
    png_int_32 X1 = (png_int_32)1023;
    int type = 1;
    int nparams = 2;
    png_const_charp units = (png_const_charp)"units";
    png_charpp params = (png_charpp)malloc((size_t)sizeof(char *) * (size_t)nparams);
    params[0] = (char *)malloc((size_t)8);
    params[1] = (char *)malloc((size_t)8);
    memset(params[0], 0, (size_t)8);
    memset(params[1], 0, (size_t)8);
    params[0][0] = 'A';
    params[1][0] = 'B';
    png_set_pCAL((png_const_structrp)png_ptr, (png_inforp)NULL, purpose, X0, X1, type, nparams, units, params);

    // step 3: Operate / Validate
    png_const_charp copy = png_get_copyright((png_const_structrp)png_ptr);
    bg.gray = (png_uint_16)((unsigned char)copy[0]);
    png_set_bKGD((png_const_structrp)png_ptr, (png_inforp)NULL, (png_const_color_16p)&bg);

    // step 4: Cleanup
    free(params[0]);
    free(params[1]);
    free(params);
    png_destroy_read_struct(&png_ptr_nonrestrict, (png_infopp)NULL, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}