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
//<ID> 129
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

    png_color_16 background;
    background.index = (png_byte)1;
    background.red = (png_uint_16)300;
    background.green = (png_uint_16)400;
    background.blue = (png_uint_16)500;
    background.gray = (png_uint_16)128;

    png_color_8 true_bits;
    true_bits.red = (png_byte)5;
    true_bits.green = (png_byte)6;
    true_bits.blue = (png_byte)7;
    true_bits.gray = (png_byte)8;
    true_bits.alpha = (png_byte)0;

    png_charpp pcal_params = (png_charpp)malloc(sizeof(png_charp) * 2);
    char *param0 = (char *)malloc((size_t)8);
    char *param1 = (char *)malloc((size_t)8);
    memset(param0, 0, (size_t)8);
    memset(param1, 0, (size_t)8);
    param0[0] = 'x';
    param0[1] = '\0';
    param1[0] = 'y';
    param1[1] = '\0';
    pcal_params[0] = param0;
    pcal_params[1] = param1;

    // step 2: Configure
    png_set_bKGD((png_const_structrp)png_ptr, info_ptr, (png_const_color_16p)&background);
    png_set_shift((png_structrp)png_ptr, (png_const_color_8p)&true_bits);
    png_set_pCAL((png_const_structrp)png_ptr, info_ptr, (png_const_charp)"CalibrationPurpose", (png_int_32)0, (png_int_32)255, 1, 2, (png_const_charp)"units", pcal_params);

    // step 3: Operate / Validate
    png_const_charp copy = png_get_copyright((png_const_structrp)png_ptr);
    png_byte c0 = (png_byte)(copy ? copy[0] : 0);
    png_uint_32 mix = ((png_uint_32)background.red) ^ ((png_uint_32)true_bits.red) ^ (png_uint_32)c0 ^ (png_uint_32)(pcal_params[0][0]);
    (void)mix;

    // step 4: Cleanup
    free(param1);
    free(param0);
    free(pcal_params);
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}