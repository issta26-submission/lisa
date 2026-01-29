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
//<ID> 126
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

    png_charpp params = (png_charpp)malloc(2 * sizeof(char *));
    params[0] = (char *)malloc(16);
    params[1] = (char *)malloc(16);
    memset(params[0], 0, (size_t)16);
    memset(params[1], 0, (size_t)16);
    params[0][0] = '1';
    params[0][1] = '\0';
    params[1][0] = '2';
    params[1][1] = '\0';

    png_color_8 true_bits;
    true_bits.red = 8;
    true_bits.green = 8;
    true_bits.blue = 8;
    true_bits.gray = 8;
    true_bits.alpha = 8;

    png_color_16 background;
    background.index = 0;
    background.red = 0x0100;
    background.green = 0x0080;
    background.blue = 0x0040;
    background.gray = 0x0000;

    // step 2: Configure
    png_set_shift((png_structrp)png_ptr, (png_const_color_8p)&true_bits);
    png_set_bKGD((png_const_structrp)png_ptr, info_ptr, (png_const_color_16p)&background);
    png_const_charp copyright = png_get_copyright((png_const_structrp)png_ptr);
    png_set_pCAL((png_const_structrp)png_ptr, info_ptr, copyright, (png_int_32)0, (png_int_32)100, 1, 2, (png_const_charp)"units", params);

    // step 3: Operate / Validate
    png_const_charp header_ver = png_get_header_version((png_const_structrp)png_ptr);
    png_uint_32 checksum = (png_uint_32)background.red ^ (png_uint_32)background.green ^ (png_uint_32)true_bits.red;
    if (header_ver != (png_const_charp)NULL) checksum ^= (png_uint_32)(unsigned char)header_ver[0];
    (void)checksum;
    (void)copyright;
    (void)header_ver;

    // step 4: Cleanup
    free(params[1]);
    free(params[0]);
    free(params);
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}