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
//<ID> 210
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // Step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = (png_inforp)NULL;
    png_color_16 bg;
    bg.index = 0;
    bg.red = 0;
    bg.green = 0;
    bg.blue = 0;
    bg.gray = 0;
    png_const_charp purpose = (png_const_charp)"Calibration";
    png_const_charp units = (png_const_charp)"mm";
    png_int_32 X0 = 0;
    png_int_32 X1 = 100;
    int pcal_type = 1;
    int pcal_nparams = 1;
    char p0[8];
    p0[0] = '1'; p0[1] = '.'; p0[2] = '0'; p0[3] = '\0';
    char * pcal_params_arr[1];
    pcal_params_arr[0] = p0;
    png_charpp pcal_params = (png_charpp)pcal_params_arr;
    png_voidp scratch = png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)16);
    memset(scratch, 0, 16);

    // Step 2: Configure
    png_set_pCAL((png_const_structrp)png_ptr, info_ptr, purpose, X0, X1, pcal_type, pcal_nparams, units, pcal_params);
    png_set_bKGD((png_const_structrp)png_ptr, info_ptr, (png_const_color_16p)&bg);
    png_set_alpha_mode((png_structrp)png_ptr, 2, 0.45455);

    // Step 3: Operate / Validate
    png_const_charp cr = png_get_copyright((png_const_structrp)png_ptr);
    unsigned char mix = (unsigned char)cr[0];
    mix ^= (unsigned char)cr[1];
    bg.gray = (png_uint_16)mix;
    (void)bg;
    (void)pcal_params;
    (void)purpose;

    // Step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, scratch);
    (void)cr;
    // API sequence test completed successfully
    return 66;
}