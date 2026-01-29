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
//<ID> 258
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
    png_byte trans_alpha[3];
    png_byte exif_buf[16];
    png_byte uintbuf[4];
    png_color_16 trans_color = {0, 0, 0, 0, 0};

    memset(trans_alpha, 0x11, sizeof(trans_alpha));
    memset(exif_buf, 0xAB, sizeof(exif_buf));
    uintbuf[0] = 0x00; uintbuf[1] = 0x00; uintbuf[2] = 0x01; uintbuf[3] = 0x02;

    // step 2: Configure
    png_set_tRNS((png_structrp)png_ptr, info_ptr, (png_const_bytep)trans_alpha, 3, (png_const_color_16p)&trans_color);
    png_set_eXIf_1((png_const_structrp)png_ptr, info_ptr, (png_uint_32)2, (png_bytep)exif_buf);

    // step 3: Operate / Validate
    png_byte color_type = png_get_color_type((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_uint_32 parsed = png_get_uint_32((png_const_bytep)uintbuf);
    trans_alpha[0] = (png_byte)((png_uint_32)color_type ^ (parsed & 0xFF));

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}