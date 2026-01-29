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
//<ID> 127
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
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_info *info_ptr_nonrestrict = (png_info *)info_ptr;

    png_color_16 background;
    background.index = 0;
    background.red = (png_uint_16)300;
    background.green = (png_uint_16)400;
    background.blue = (png_uint_16)500;
    background.gray = (png_uint_16)0;

    png_color_8 true_bits;
    true_bits.red = (png_byte)8;
    true_bits.green = (png_byte)8;
    true_bits.blue = (png_byte)8;
    true_bits.gray = (png_byte)4;
    true_bits.alpha = (png_byte)0;

    const char *purpose = "TestPurpose";
    const char *units = "units";
    int nparams = 2;
    char *param0 = (char *)malloc((size_t)8);
    memset(param0, 0, (size_t)8);
    param0[0] = '1';
    char *param1 = (char *)malloc((size_t)8);
    memset(param1, 0, (size_t)8);
    param1[0] = '2';
    char **params = (char **)malloc((size_t)(sizeof(char *) * nparams));
    params[0] = param0;
    params[1] = param1;
    png_charpp params_pp = (png_charpp)params;

    // step 2: Configure
    png_const_charp cp = png_get_copyright((png_const_structrp)png_ptr);
    background.red = (png_uint_16)((png_uint_16)background.red ^ (unsigned char)cp[0]);
    true_bits.gray = (png_byte)((png_byte)true_bits.gray ^ (unsigned char)cp[1]);
    png_set_pCAL((png_const_structrp)png_ptr, info_ptr, (png_const_charp)purpose, (png_int_32)0, (png_int_32)1024, 1, nparams, (png_const_charp)units, params_pp);
    png_set_bKGD((png_const_structrp)png_ptr, info_ptr, (png_const_color_16p)&background);
    png_set_shift((png_structrp)png_ptr, (png_const_color_8p)&true_bits);

    // step 3: Operate / Validate
    png_uint_32 checksum = (png_uint_32)background.red ^ (png_uint_32)background.green ^ (png_uint_32)true_bits.gray ^ (png_uint_32)(unsigned char)cp[0];
    (void)checksum;

    // step 4: Cleanup
    free(param0);
    free(param1);
    free(params);
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}