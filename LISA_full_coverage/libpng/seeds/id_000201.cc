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
//<ID> 201
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

    // step 2: Configure
    int prev_option = png_set_option((png_structrp)png_ptr, 0x1, 1);

    // step 3: Operate / Validate
    png_const_charp ver = png_get_header_version((png_const_structrp)png_ptr);
    unsigned char buf[4];
    buf[0] = (unsigned char)ver[0];
    buf[1] = (unsigned char)ver[1];
    buf[2] = (unsigned char)ver[2];
    buf[3] = (unsigned char)ver[3];
    png_uint_32 u31 = png_get_uint_31((png_const_structrp)png_ptr, (png_const_bytep)buf);

    png_charp purpose = (png_charp)NULL;
    png_int_32 X0 = 0;
    png_int_32 X1 = 0;
    int pcal_type = 0;
    int pcal_nparams = 0;
    png_charp units = (png_charp)NULL;
    png_charpp params = (png_charpp)NULL;
    png_uint_32 pcal_ret = png_get_pCAL((png_const_structrp)png_ptr, info_ptr, &purpose, &X0, &X1, &pcal_type, &pcal_nparams, &units, &params);

    size_t alloc_size = (size_t)(((u31 ^ pcal_ret) & 0xFFu) + 128u);
    png_voidp buffer = png_malloc((png_const_structrp)png_ptr, (png_alloc_size_t)alloc_size);
    memset(buffer, (int)(u31 & 0xFFu), alloc_size);
    ((unsigned char *)buffer)[0] ^= (unsigned char)(prev_option & 0xFF);

    png_set_option((png_structrp)png_ptr, 0x1, 0);

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, buffer);
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);

    // API sequence test completed successfully
    return 66;
}