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
//<ID> 213
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_const_charp cprt = png_get_copyright((png_const_structrp)png_ptr);
    unsigned char first_char = (unsigned char)cprt[0];

    // step 2: Configure
    png_set_alpha_mode((png_structrp)png_ptr, 1, 2.2);
    png_set_pCAL((png_const_structrp)png_ptr, (png_inforp)NULL, cprt, (png_int_32)0, (png_int_32)100, 1, 0, (png_const_charp)"units", (png_charpp)NULL);

    // step 3: Operate / Validate
    png_color_16 bg;
    memset(&bg, 0, sizeof(bg));
    bg.index = (png_byte)(first_char & 0xFFu);
    bg.red = (png_uint_16)first_char;
    bg.green = (png_uint_16)(first_char ^ 0x55u);
    bg.blue = (png_uint_16)(first_char ^ 0xAAu);
    bg.gray = (png_uint_16)first_char;
    png_set_bKGD((png_const_structrp)png_ptr, (png_inforp)NULL, (png_const_color_16p)&bg);

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, (png_voidp)NULL);
    // API sequence test completed successfully
    return 66;
}