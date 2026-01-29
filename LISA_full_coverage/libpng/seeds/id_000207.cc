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
//<ID> 207
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);

    // step 2: Configure
    png_const_charp ver = png_get_header_ver((png_const_structrp)png_ptr);
    png_byte vbuf[4];
    vbuf[0] = ver ? (png_byte)ver[0] : (png_byte)0;
    vbuf[1] = ver ? (png_byte)ver[1] : (png_byte)0;
    vbuf[2] = ver ? (png_byte)ver[2] : (png_byte)0;
    vbuf[3] = ver ? (png_byte)ver[3] : (png_byte)0;
    png_uint_32 max_chunk = (png_uint_32)png_get_chunk_malloc_max((png_const_structrp)png_ptr);
    png_uint_32 u31 = max_chunk & 0x7FFFFFFFu;
    int prev_option = png_set_option((png_structrp)png_ptr, (int)(u31 & 0xFFu), 1);

    // step 3: Operate / Validate
    png_inforp info = (png_inforp)NULL;
    png_charp purpose = (png_charp)NULL;
    png_int_32 X0 = 0;
    png_int_32 X1 = 0;
    int pcal_type = 0;
    int pcal_nparams = 0;
    png_charp units = (png_charp)NULL;
    png_charpp params = (png_charpp)NULL;
    png_uint_32 pcal_ret = png_get_pCAL((png_const_structrp)png_ptr, info, &purpose, &X0, &X1, &pcal_type, &pcal_nparams, &units, &params);
    (void)purpose;
    (void)units;
    (void)params;
    int new_option = png_set_option((png_structrp)png_ptr, (int)((pcal_ret ^ u31 ^ (png_uint_32)prev_option) & 0x7u), 0);
    (void)new_option;

    // step 4: Cleanup
    png_destroy_info_struct((png_const_structrp)png_ptr, (png_infopp)NULL);
    return 66;
    // API sequence test completed successfully
}