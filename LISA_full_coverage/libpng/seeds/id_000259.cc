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
//<ID> 259
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct("1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    unsigned char exif_data[8];
    unsigned char trans_alpha[3];
    unsigned char header_buf[4];

    // step 2: Configure
    memset(exif_data, 0xAB, sizeof(exif_data));
    trans_alpha[0] = 0x10;
    trans_alpha[1] = 0x20;
    trans_alpha[2] = 0x30;
    png_color_16 trans_color = { 0, 0x0100, 0x0200, 0x0300, 0 };

    png_set_eXIf_1((png_const_structrp)png_ptr, info_ptr, (png_uint_32)1, (png_bytep)exif_data);
    png_set_tRNS((png_structrp)png_ptr, info_ptr, (png_const_bytep)trans_alpha, 3, (png_const_color_16p)&trans_color);

    // step 3: Operate / Validate
    header_buf[0] = 0x00;
    header_buf[1] = 0x00;
    header_buf[2] = 0x01;
    header_buf[3] = 0x02;
    png_uint_32 header_val = png_get_uint_32((png_const_bytep)header_buf);
    png_byte color_type = png_get_color_type((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_uint_32 exif_val = png_get_uint_32((png_const_bytep)exif_data);

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    (void)header_val;
    (void)color_type;
    (void)exif_val;
    return 66;
}