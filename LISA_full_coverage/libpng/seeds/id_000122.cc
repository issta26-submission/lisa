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
//<ID> 122
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
    memset(&background, 0, sizeof(background));
    background.index = (png_byte)5;
    background.red = (png_uint_16)300;
    background.green = (png_uint_16)400;
    background.blue = (png_uint_16)500;
    background.gray = (png_uint_16)600;
    png_color_8 true_bits;
    memset(&true_bits, 0, sizeof(true_bits));
    true_bits.red = (png_byte)8;
    true_bits.green = (png_byte)8;
    true_bits.blue = (png_byte)8;
    true_bits.gray = (png_byte)8;
    true_bits.alpha = (png_byte)8;
    png_charpp params = (png_charpp)malloc(sizeof(char*) * 2);
    params[0] = (char*)malloc(8);
    params[1] = (char*)malloc(8);
    memset(params[0], 0, 8);
    memset(params[1], 0, 8);
    params[0][0] = '1';
    params[0][1] = '2';
    params[1][0] = 'A';
    params[1][1] = 'B';

    // step 2: Configure
    png_set_bKGD((png_const_structrp)png_ptr, info_ptr, (png_const_color_16p)&background);
    png_set_pCAL((png_const_structrp)png_ptr, info_ptr, (png_const_charp)"ExamplePurpose", (png_int_32)0, (png_int_32)100, 1, 2, (png_const_charp)"units", params);
    png_set_shift((png_structrp)png_ptr, (png_const_color_8p)&true_bits);

    // step 3: Operate / Validate
    png_const_charp cr = png_get_copyright((png_const_structrp)png_ptr);
    int checksum = (int)(unsigned char)cr[0] ^ (int)(background.red & 0xFFu) ^ (int)(background.green & 0xFFu) ^ (int)true_bits.red;
    (void)checksum;

    // step 4: Cleanup
    free(params[0]);
    free(params[1]);
    free(params);
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}