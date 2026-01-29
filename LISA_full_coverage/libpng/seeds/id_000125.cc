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
//<ID> 125
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

    // step 2: Configure
    png_color_16 background;
    memset(&background, 0, sizeof(background));
    background.red = (png_uint_16)300u;
    background.green = (png_uint_16)200u;
    background.blue = (png_uint_16)100u;
    png_set_bKGD((png_const_structrp)png_ptr, info_ptr, (png_const_color_16p)&background);

    png_color_8 true_bits;
    memset(&true_bits, 0, sizeof(true_bits));
    true_bits.red = (png_byte)8;
    true_bits.green = (png_byte)8;
    true_bits.blue = (png_byte)8;
    true_bits.gray = (png_byte)8;
    true_bits.alpha = (png_byte)8;
    png_set_shift((png_structrp)png_ptr, (png_const_color_8p)&true_bits);

    png_const_charp purpose = png_get_copyright((png_const_structrp)png_ptr);
    char *units_buf = (char *)malloc((size_t)8);
    memset(units_buf, 0, (size_t)8);
    units_buf[0] = 'u';
    units_buf[1] = 'n';
    units_buf[2] = 'i';
    units_buf[3] = 't';
    units_buf[4] = 's';
    png_const_charp units = (png_const_charp)units_buf;

    png_charpp params = (png_charpp)malloc(sizeof(char *));
    params[0] = (char *)malloc((size_t)16);
    memset(params[0], 0, (size_t)16);
    params[0][0] = purpose[0];
    params[0][1] = '\0';

    png_int_32 X0 = 0;
    png_int_32 X1 = 1023;
    int pcal_type = 1;
    int pcal_nparams = 1;
    png_set_pCAL((png_const_structrp)png_ptr, info_ptr, purpose, X0, X1, pcal_type, pcal_nparams, units, params);

    // step 3: Operate / Validate
    png_charp ret_purpose = (png_charp)NULL;
    png_int_32 ret_X0 = 0;
    png_int_32 ret_X1 = 0;
    int ret_type = 0;
    int ret_nparams = 0;
    png_charp ret_units = (png_charp)NULL;
    png_charpp ret_params = (png_charpp)NULL;
    png_uint_32 pcal_mask = png_get_pCAL((png_const_structrp)png_ptr, info_ptr, &ret_purpose, &ret_X0, &ret_X1, &ret_type, &ret_nparams, &ret_units, &ret_params);
    (void)pcal_mask;
    (void)ret_X0;
    (void)ret_X1;
    (void)ret_type;
    (void)ret_nparams;
    (void)ret_purpose;
    (void)ret_units;
    (void)ret_params;

    // step 4: Cleanup
    free(params[0]);
    free(params);
    free(units_buf);
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}