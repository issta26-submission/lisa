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
//<ID> 123
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
    png_color_8 true_bits;
    memset(&true_bits, 0, sizeof(true_bits));
    png_charpp pcal_params = (png_charpp)malloc(sizeof(char *));
    char *pcal_param0 = (char *)malloc(16);
    memset(pcal_param0, 0, 16);
    pcal_param0[0] = '1';
    pcal_param0[1] = '.';
    pcal_param0[2] = '0';
    pcal_param0[3] = '0';
    pcal_params[0] = pcal_param0;

    // step 2: Configure
    true_bits.red = 8;
    true_bits.green = 8;
    true_bits.blue = 8;
    true_bits.gray = 8;
    true_bits.alpha = 8;
    png_set_shift((png_structrp)png_ptr, (png_const_color_8p)&true_bits);
    png_set_pCAL((png_const_structrp)png_ptr, info_ptr, (png_const_charp)"Calibration", (png_int_32)0, (png_int_32)1023, 1, 1, (png_const_charp)"units", pcal_params);
    png_const_charp notice = png_get_copyright((png_const_structrp)png_ptr);
    background.index = (png_byte)0;
    background.red = (png_uint_16)((unsigned char)notice[0]);
    background.green = (png_uint_16)((unsigned char)notice[1]);
    background.blue = (png_uint_16)((unsigned char)notice[2]);
    background.gray = (png_uint_16)0;
    png_set_bKGD((png_const_structrp)png_ptr, info_ptr, (png_const_color_16p)&background);

    // step 3: Operate / Validate
    png_uint_32 io_state = png_get_io_state((png_const_structrp)png_ptr);
    png_uint_32 checksum = io_state ^ ((png_uint_32)background.red) ^ ((png_uint_32)background.green);
    (void)checksum;

    // step 4: Cleanup
    free(pcal_param0);
    free(pcal_params);
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}