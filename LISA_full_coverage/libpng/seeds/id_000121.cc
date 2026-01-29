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
//<ID> 121
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = (png_info *)info_ptr;
    png_color_8 true_bits;
    memset(&true_bits, 0, sizeof(true_bits));
    true_bits.red = 5;
    true_bits.green = 6;
    true_bits.blue = 5;
    true_bits.gray = 8;
    png_color_16 background;
    memset(&background, 0, sizeof(background));
    background.red = 300;
    background.green = 400;
    background.blue = 500;
    background.gray = 0;
    char *pcal_param0 = (char *)malloc(4);
    pcal_param0[0] = '1';
    pcal_param0[1] = '.';
    pcal_param0[2] = '0';
    pcal_param0[3] = '\0';
    char *params_stack[2];
    params_stack[0] = pcal_param0;
    params_stack[1] = (char *)NULL;

    // step 2: Configure
    png_set_shift((png_structrp)png_ptr, (png_const_color_8p)&true_bits);
    png_set_bKGD((png_const_structrp)png_ptr, info_ptr, (png_const_color_16p)&background);
    png_set_pCAL((png_const_structrp)png_ptr, info_ptr, (png_const_charp)"CalibrationPurpose", (png_int_32)0, (png_int_32)100, 2, 1, (png_const_charp)"units", (png_charpp)params_stack);

    // step 3: Operate / Validate
    png_const_charp copyright = png_get_copyright((png_const_structrp)png_ptr);
    unsigned int checksum = (unsigned int)copyright[0];
    checksum ^= (unsigned int)(background.red & 0xFFu);
    checksum ^= (unsigned int)true_bits.green;
    (void)checksum;

    // step 4: Cleanup
    free(pcal_param0);
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}