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
//<ID> 253
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
    memset(trans_alpha, 0x7F, sizeof(trans_alpha));
    png_color_16 trans_color;
    trans_color.index = 0;
    trans_color.red = 0;
    trans_color.green = 0;
    trans_color.blue = 0;
    trans_color.gray = 0;

    // step 2: Configure
    png_set_tRNS((png_structrp)png_ptr, info_ptr, (png_const_bytep)trans_alpha, 2, (png_const_color_16p)&trans_color);
    png_byte exif_data[8];
    memset(exif_data, 0xAB, sizeof(exif_data));
    png_set_eXIf_1((png_const_structrp)png_ptr, info_ptr, (png_uint_32)1, (png_bytep)exif_data);

    // step 3: Operate / Validate
    png_byte buf4[4];
    png_save_int_32(buf4, (png_int_32)0x11223344);
    png_uint_32 read_val = png_get_uint_32((png_const_bytep)buf4);
    png_byte color_type = png_get_color_type((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_save_int_32(buf4, (png_int_32)(read_val ^ (png_uint_32)color_type));

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}