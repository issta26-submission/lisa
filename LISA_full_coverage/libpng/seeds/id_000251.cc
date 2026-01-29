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
//<ID> 251
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
    png_byte trans_alpha[2];
    png_color_16 trans_color;
    png_byte exif_buf[16];
    png_byte test_buf[4];

    // step 2: Configure
    memset(trans_alpha, 0x7F, sizeof(trans_alpha));
    trans_color.index = 0;
    trans_color.red = 0x0100;
    trans_color.green = 0x0200;
    trans_color.blue = 0x0300;
    trans_color.gray = 0x0040;
    png_set_tRNS((png_structrp)png_ptr, info_ptr, (png_const_bytep)trans_alpha, (int)(sizeof(trans_alpha) / sizeof(trans_alpha[0])), (png_const_color_16p)&trans_color);
    memset(exif_buf, 0xAA, sizeof(exif_buf));
    png_set_eXIf_1((png_const_structrp)png_ptr, info_ptr, (png_uint_32)1, (png_bytep)exif_buf);

    // step 3: Operate / Validate
    png_byte color_type = png_get_color_type((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    test_buf[0] = (png_byte)(color_type & 0xFF);
    test_buf[1] = (png_byte)((color_type + 1) & 0xFF);
    test_buf[2] = (png_byte)((color_type + 2) & 0xFF);
    test_buf[3] = (png_byte)((color_type + 3) & 0xFF);
    png_uint_32 val = png_get_uint_32((png_const_bytep)test_buf);
    (void)val; // use value to show data flow

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}