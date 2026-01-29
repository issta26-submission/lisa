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
//<ID> 487
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
    png_info_init_3((png_infopp)&info_ptr, (size_t)0);
    png_charp purpose = (png_charp)png_malloc((png_const_structrp)png_ptr, (png_alloc_size_t)16);
    memset(purpose, 0, (size_t)16);
    purpose[0] = 'p'; purpose[1] = 'C'; purpose[2] = 'A'; purpose[3] = 'L'; purpose[4] = '\0';
    png_charp units = (png_charp)png_malloc((png_const_structrp)png_ptr, (png_alloc_size_t)8);
    memset(units, 0, (size_t)8);
    units[0] = 'u'; units[1] = 'n'; units[2] = '\0';
    int nparams = 2;
    png_charpp params = (png_charpp)png_malloc((png_const_structrp)png_ptr, (png_alloc_size_t)(sizeof(png_charp) * (size_t)nparams));
    params[0] = (png_charp)png_malloc((png_const_structrp)png_ptr, (png_alloc_size_t)8);
    params[1] = (png_charp)png_malloc((png_const_structrp)png_ptr, (png_alloc_size_t)8);
    memset(params[0], 0, (size_t)8);
    memset(params[1], 0, (size_t)8);
    params[0][0] = 'A'; params[0][1] = '\0';
    params[1][0] = 'B'; params[1][1] = '\0';
    png_int_32 X0 = (png_int_32)100;
    png_int_32 X1 = (png_int_32)200;
    int type = 1;

    // step 2: Configure
    png_set_user_transform_info((png_structrp)png_ptr, (png_voidp)params, 8, 3);
    png_set_invert_mono((png_structrp)png_ptr);
    png_set_pCAL((png_const_structrp)png_ptr, info_ptr, (png_const_charp)purpose, X0, X1, type, nparams, (png_const_charp)units, params);

    // step 3: Operate / Validate
    png_charp purpose_out = (png_charp)NULL;
    png_int_32 X0_out = 0;
    png_int_32 X1_out = 0;
    int type_out = 0;
    int nparams_out = 0;
    png_charp units_out = (png_charp)NULL;
    png_charpp params_out = (png_charpp)NULL;
    png_get_pCAL((png_const_structrp)png_ptr, info_ptr, &purpose_out, &X0_out, &X1_out, &type_out, &nparams_out, &units_out, &params_out);
    png_bytep scratch = (png_bytep)png_malloc((png_const_structrp)png_ptr, (png_alloc_size_t)8);
    memset(scratch, 0, (size_t)8);
    unsigned int v0 = (unsigned int)(X0_out & 0xFFFFu);
    scratch[0] = (png_byte)((v0 >> 8) & 0xFF); scratch[1] = (png_byte)(v0 & 0xFF);
    unsigned int v1 = (unsigned int)(X1_out & 0xFFFFu);
    scratch[2] = (png_byte)((v1 >> 8) & 0xFF); scratch[3] = (png_byte)(v1 & 0xFF);
    (void)purpose_out;
    (void)type_out;
    (void)units_out;
    (void)params_out;

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, (png_voidp)params[1]);
    png_free_default((png_const_structrp)png_ptr, (png_voidp)params[0]);
    png_free_default((png_const_structrp)png_ptr, (png_voidp)params);
    png_free_default((png_const_structrp)png_ptr, (png_voidp)purpose);
    png_free_default((png_const_structrp)png_ptr, (png_voidp)units);
    png_free_default((png_const_structrp)png_ptr, (png_voidp)scratch);
    png_free_default((png_const_structrp)png_ptr, (png_voidp)info_ptr);
    png_free_default((png_const_structrp)png_ptr, (png_voidp)png_ptr);

    // API sequence test completed successfully
    return 66;
}