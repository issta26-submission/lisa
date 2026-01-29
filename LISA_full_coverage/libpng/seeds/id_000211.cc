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
//<ID> 211
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = (png_inforp)NULL;
    png_const_charp copyright_str = (png_const_charp)NULL;
    png_color_16 background = { 0, 0, 0, 0, 0 };
    png_int_32 pcal_X0 = 0;
    png_int_32 pcal_X1 = 100;
    int pcal_type = 1;
    int pcal_nparams = 1;
    png_const_charp pcal_units = (png_const_charp)"units";

    char p0[] = "42";
    char * p_arr[2];
    p_arr[0] = p0;
    p_arr[1] = (char *)NULL;
    png_charpp pcal_params = (png_charpp)p_arr;

    // step 2: Configure
    copyright_str = png_get_copyright((png_const_structrp)png_ptr);
    png_set_alpha_mode((png_structrp)png_ptr, 2, 1.0);
    png_set_pCAL((png_const_structrp)png_ptr, info_ptr, copyright_str, pcal_X0, pcal_X1, pcal_type, pcal_nparams, pcal_units, pcal_params);
    png_set_bKGD((png_const_structrp)png_ptr, info_ptr, (png_const_color_16p)&background);

    // step 3: Operate / Validate
    (void)copyright_str;
    (void)background;
    (void)pcal_params;
    (void)pcal_units;
    (void)pcal_X0;
    (void)pcal_X1;
    (void)pcal_type;
    (void)pcal_nparams;

    // step 4: Cleanup
    png_destroy_info_struct((png_const_structrp)png_ptr, (png_infopp)&info_ptr);

    // API sequence test completed successfully
    return 66;
}