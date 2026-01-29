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
//<ID> 120
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = (png_info *)info_ptr;

    png_color_16 *bg = (png_color_16 *)malloc(sizeof(png_color_16));
    memset(bg, 0, sizeof(png_color_16));
    bg->red = (png_uint_16)300u;
    bg->green = (png_uint_16)400u;
    bg->blue = (png_uint_16)500u;
    bg->gray = (png_uint_16)0u;
    bg->index = (png_byte)0u;

    png_color_8 true_bits;
    memset(&true_bits, 0, sizeof(true_bits));
    true_bits.red = (png_byte)8u;
    true_bits.green = (png_byte)8u;
    true_bits.blue = (png_byte)8u;
    true_bits.gray = (png_byte)8u;
    true_bits.alpha = (png_byte)8u;

    int nparams = 2;
    char **params = (char **)malloc((size_t)nparams * sizeof(char *));
    params[0] = (char *)malloc(4);
    params[1] = (char *)malloc(4);
    memset(params[0], 0, 4);
    memset(params[1], 0, 4);
    params[0][0] = '1'; params[0][1] = '.'; params[0][2] = '0'; params[0][3] = '\0';
    params[1][0] = '2'; params[1][1] = '.'; params[1][2] = '0'; params[1][3] = '\0';

    png_const_charp purpose = (png_const_charp)"Calibration";
    png_const_charp units = (png_const_charp)"units";

    // step 2: Configure
    png_set_pCAL((png_const_structrp)png_ptr, info_ptr, purpose, (png_int_32)0, (png_int_32)1024, 1, nparams, units, (png_charpp)params);
    png_set_bKGD((png_const_structrp)png_ptr, info_ptr, (png_const_color_16p)bg);
    png_set_shift((png_structrp)png_ptr, (png_const_color_8p)&true_bits);

    png_const_charp copyr = png_get_copyright((png_const_structrp)png_ptr);

    // step 3: Operate / Validate
    unsigned int v0 = (unsigned int)(copyr ? (unsigned char)copyr[0] : 0u);
    unsigned int v1 = (unsigned int)(copyr ? (unsigned char)copyr[1] : 0u);
    unsigned int checksum = ((unsigned int)bg->red) ^ ((unsigned int)true_bits.red) ^ (v0 | (v1 << 8));
    (void)checksum;

    // step 4: Cleanup
    free(params[0]);
    free(params[1]);
    free(params);
    free(bg);
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}