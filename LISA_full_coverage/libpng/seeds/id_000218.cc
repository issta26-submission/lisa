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
//<ID> 218
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = (png_inforp)malloc(1);
    memset(info_ptr, 0, (size_t)1);

    // step 2: Configure
    char *param0 = (char*)malloc(8);
    char *param1 = (char*)malloc(8);
    memcpy(param0, "paramA", 7);
    memcpy(param1, "paramB", 7);
    char *params_arr[2];
    params_arr[0] = param0;
    params_arr[1] = param1;
    png_const_charp purpose = (png_const_charp)"Calibration";
    png_const_charp units = (png_const_charp)"units";
    png_set_pCAL((png_const_structrp)png_ptr, info_ptr, purpose, (png_int_32)0, (png_int_32)1024, 1, 2, units, (png_charpp)params_arr);

    // step 3: Operate / Validate
    png_const_charp copy_right = png_get_copyright((png_const_structrp)png_ptr);
    png_color_16 bg;
    memset(&bg, 0, sizeof(bg));
    bg.index = (png_byte)copy_right[0];
    bg.red = (png_uint_16)100u;
    bg.green = (png_uint_16)150u;
    bg.blue = (png_uint_16)200u;
    bg.gray = (png_uint_16)50u;
    png_set_bKGD((png_const_structrp)png_ptr, info_ptr, (png_const_color_16p)&bg);
    png_set_alpha_mode((png_structrp)png_ptr, 1, 0.45455);

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, (png_voidp)param0);
    png_free_default((png_const_structrp)png_ptr, (png_voidp)param1);
    png_free_default((png_const_structrp)png_ptr, (png_voidp)info_ptr);
    // API sequence test completed successfully
    return 66;
}