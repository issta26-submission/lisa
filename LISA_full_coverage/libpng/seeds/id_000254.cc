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
//<ID> 254
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
    png_byte header_buf[4] = { 0x12, 0x34, 0x56, 0x78 };
    png_uint_32 parsed_value = png_get_uint_32((png_const_bytep)header_buf);
    png_byte trans_alpha[3];
    trans_alpha[0] = (png_byte)(parsed_value & 0xFFu);
    trans_alpha[1] = (png_byte)((parsed_value >> 8) & 0xFFu);
    trans_alpha[2] = (png_byte)((parsed_value >> 16) & 0xFFu);
    png_color_16 trans_color = { 0 };
    trans_color.red = (png_uint_16)((parsed_value >> 8) & 0xFFFFu);
    trans_color.green = (png_uint_16)((parsed_value >> 16) & 0xFFFFu);
    trans_color.blue = (png_uint_16)((parsed_value >> 24) & 0xFFFFu);

    // step 2: Configure
    png_set_tRNS((png_structrp)png_ptr, (png_inforp)info_ptr, (png_const_bytep)trans_alpha, 3, (png_const_color_16p)&trans_color);
    png_byte exif_data[16];
    memset(exif_data, 0xA5, sizeof(exif_data));
    png_uint_32 num_exif = (parsed_value & 7u) | 1u;
    png_set_eXIf_1((png_const_structrp)png_ptr, info_ptr, num_exif, (png_bytep)exif_data);

    // step 3: Operate / Validate
    png_byte color_type = png_get_color_type((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    trans_color.gray = (png_uint_16)color_type;
    png_uint_32 verify = png_get_uint_32((png_const_bytep)header_buf);
    (void)verify;

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}