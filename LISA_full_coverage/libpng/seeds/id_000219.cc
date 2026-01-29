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
//<ID> 219
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_const_charp copyright = png_get_copyright((png_const_structrp)png_ptr);
    png_int_32 X0 = (png_int_32)0;
    png_int_32 X1 = (png_int_32)1000;
    int pcal_type = 1;
    int pcal_nparams = 2;
    png_const_charp pcal_units = (png_const_charp)"units";
    static char p0[] = "1.234";
    static char p1[] = "5.678";
    char * pcal_params_arr[3];
    pcal_params_arr[0] = p0;
    pcal_params_arr[1] = p1;
    pcal_params_arr[2] = (char *)NULL;
    png_charpp pcal_params = (png_charpp)pcal_params_arr;
    png_color_16 bg;
    bg.index = (png_byte)(copyright ? (unsigned char)copyright[0] : 0u);
    bg.red = (png_uint_16)(X0 & 0xFFFF);
    bg.green = (png_uint_16)(X1 & 0xFFFF);
    bg.blue = (png_uint_16)((X0 + X1) & 0xFFFF);
    bg.gray = (png_uint_16)0;

    // step 2: Configure
    png_set_alpha_mode(png_ptr, 1, 2.2);
    png_set_pCAL((png_const_structrp)png_ptr, (png_inforp)NULL, copyright ? copyright : (png_const_charp)"purpose", X0, X1, pcal_type, pcal_nparams, pcal_units, pcal_params);

    // step 3: Operate / Validate
    png_set_bKGD((png_const_structrp)png_ptr, (png_inforp)NULL, (png_const_color_16p)&bg);
    png_voidp scratch = png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)64);
    memset(scratch, 0xA5, 64);

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, scratch);
    (void)copyright;
    (void)pcal_params;
    // API sequence test completed successfully
    return 66;
}