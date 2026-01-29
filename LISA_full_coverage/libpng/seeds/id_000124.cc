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
//<ID> 124
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

    png_color_16 background16;
    background16.index = 0;
    background16.red = (png_uint_16)300;
    background16.green = (png_uint_16)400;
    background16.blue = (png_uint_16)500;
    background16.gray = (png_uint_16)0;

    png_color_8 true_bits8;
    true_bits8.red = (png_byte)8;
    true_bits8.green = (png_byte)8;
    true_bits8.blue = (png_byte)8;
    true_bits8.gray = (png_byte)8;
    true_bits8.alpha = (png_byte)0;

    const char * purpose = "CalibrationPurpose";
    const char * units = "units";
    int nparams = 2;
    png_charpp params = (png_charpp)malloc((size_t)nparams * sizeof(char *));
    memset(params, 0, (size_t)nparams * sizeof(char *));
    params[0] = (char *)malloc((size_t)3);
    memset(params[0], 0, (size_t)3);
    params[0][0] = '1';
    params[1] = (char *)malloc((size_t)3);
    memset(params[1], 0, (size_t)3);
    params[1][0] = '2';

    png_int_32 X0 = 0;
    png_int_32 X1 = 100;
    int pcal_type = 1;

    // step 2: Configure
    png_set_pCAL((png_const_structrp)png_ptr, info_ptr, (png_const_charp)purpose, X0, X1, pcal_type, nparams, (png_const_charp)units, params);
    png_set_bKGD((png_const_structrp)png_ptr, info_ptr, (png_const_color_16p)&background16);
    png_set_shift((png_structrp)png_ptr, (png_const_color_8p)&true_bits8);

    // step 3: Operate / Validate
    png_const_charp cpr = png_get_copyright((png_const_structrp)png_ptr);
    png_uint_32 derived = (png_uint_32)((unsigned char)cpr[0]) ^ (png_uint_32)background16.red;
    params[0][0] = (char)(derived & 0xFFu);

    // step 4: Cleanup
    free(params[0]);
    free(params[1]);
    free(params);
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}