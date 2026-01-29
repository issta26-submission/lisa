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
//<ID> 255
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
    png_byte trans_alpha_arr[3];
    trans_alpha_arr[0] = 0x11;
    trans_alpha_arr[1] = 0x22;
    trans_alpha_arr[2] = 0x33;
    png_color_16 trans_color;
    trans_color.index = 0;
    trans_color.red = 0;
    trans_color.green = 0;
    trans_color.blue = 0;
    trans_color.gray = 0;

    // step 2: Configure
    png_set_tRNS((png_structrp)png_ptr, info_ptr, (png_const_bytep)trans_alpha_arr, 3, (png_const_color_16p)&trans_color);
    png_byte exif_buf[8];
    memset(exif_buf, 0xAB, sizeof(exif_buf));
    png_set_eXIf_1((png_const_structrp)png_ptr, info_ptr, (png_uint_32)sizeof(exif_buf), (png_bytep)exif_buf);

    // step 3: Operate / Validate
    png_byte int_buf[4];
    png_save_int_32(int_buf, (png_int_32)0x01020304);
    png_uint_32 got = png_get_uint_32((png_const_bytep)int_buf);
    png_byte color_type = png_get_color_type((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    unsigned char work[16];
    memset(work, (int)(got ^ (png_uint_32)color_type), sizeof(work));

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}