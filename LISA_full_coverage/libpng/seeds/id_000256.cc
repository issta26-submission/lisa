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
//<ID> 256
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
    png_byte sig_buf[4] = {0x00, 0x00, 0x00, 0x04};
    png_uint_32 exif_len = png_get_uint_32((png_const_bytep)sig_buf);
    png_bytep exif_buf = (png_bytep)NULL;
    png_byte trans_alpha[2] = {0xFF, 0x00};
    int num_trans = 2;
    png_color_16 trans_color = {0, 0x0100, 0x0200, 0x0300, 0x0000};

    // step 2: Setup / Configure
    unsigned char local_exif[8];
    memset(local_exif, 0x5A, sizeof(local_exif));
    exif_buf = (png_bytep)local_exif;
    png_set_eXIf_1((png_const_structrp)png_ptr, info_ptr, exif_len, exif_buf);
    png_set_tRNS((png_structrp)png_ptr, info_ptr, (png_const_bytep)trans_alpha, num_trans, (png_const_color_16p)&trans_color);

    // step 3: Operate / Validate
    png_byte color_type = png_get_color_type((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    (void)color_type;
    png_uint_32 got_len = png_get_uint_32((png_const_bytep)sig_buf);
    (void)got_len;

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}