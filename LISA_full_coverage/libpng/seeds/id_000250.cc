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
//<ID> 250
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
    memset(trans_alpha_arr, 0x7F, sizeof(trans_alpha_arr));
    png_color_16 trans_color;
    trans_color.index = 0;
    trans_color.red = 0;
    trans_color.green = 0;
    trans_color.blue = 0;
    trans_color.gray = 0;
    png_byte exif_buffer[8];
    memset(exif_buffer, 0xA5, sizeof(exif_buffer));

    // step 2: Configure
    png_set_tRNS((png_structrp)png_ptr, info_ptr, (png_const_bytep)trans_alpha_arr, 3, (png_const_color_16p)&trans_color);
    png_set_eXIf_1((png_const_structrp)png_ptr, info_ptr, (png_uint_32)sizeof(exif_buffer), (png_bytep)exif_buffer);

    // step 3: Operate / Validate
    png_uint_32 exif_leading = png_get_uint_32((png_const_bytep)exif_buffer);
    png_byte color_type = png_get_color_type((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    (void)exif_leading;
    (void)color_type;

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}