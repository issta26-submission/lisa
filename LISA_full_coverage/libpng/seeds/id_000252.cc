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
//<ID> 252
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
    png_byte trans_buf[4];
    png_byte exif_buf[8];
    png_color_16 trans_color;
    png_uint_32 parsed_uint32 = 0;
    png_byte color_type = 0;

    // step 2: Configure
    png_save_int_32(trans_buf, (png_int_32)0xA1B2C3D4);
    memset(exif_buf, 0x5A, sizeof(exif_buf));
    trans_color.index = 1;
    trans_color.red = (png_uint_16)0x0100;
    trans_color.green = (png_uint_16)0x0200;
    trans_color.blue = (png_uint_16)0x0300;
    trans_color.gray = (png_uint_16)0x0000;
    png_set_tRNS((png_structrp)png_ptr, info_ptr, (png_const_bytep)trans_buf, 4, (png_const_color_16p)&trans_color);
    png_set_eXIf_1((png_const_structrp)png_ptr, info_ptr, (png_uint_32)1, (png_bytep)exif_buf);

    // step 3: Operate / Validate
    parsed_uint32 = png_get_uint_32((png_const_bytep)trans_buf);
    trans_color.gray = (png_uint_16)(parsed_uint32 & 0xFFFFu);
    color_type = png_get_color_type((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    (void)color_type;
    (void)trans_color;

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}