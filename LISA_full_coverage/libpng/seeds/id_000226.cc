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
//<ID> 226
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Create write struct and info struct
    png_structp write_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop write_info = png_create_info_struct((png_const_structrp)write_ptr);

    // step 2: Prepare a valid PNG signature and check it
    unsigned char signature[8];
    signature[0] = 0x89u;
    signature[1] = 'P';
    signature[2] = 'N';
    signature[3] = 'G';
    signature[4] = '\r';
    signature[5] = '\n';
    signature[6] = 0x1Au;
    signature[7] = '\n';
    int sig_check = png_sig_cmp((png_const_bytep)signature, (size_t)0, (size_t)8);

    // step 3: Set the number of signature bytes (use the checked value, which will be 0 for a valid signature)
    png_set_sig_bytes((png_structrp)write_ptr, (int)(sig_check & 0xFF));

    // step 4: Configure IHDR in the info struct
    png_set_IHDR((png_const_structrp)write_ptr, write_info, (png_uint_32)16, (png_uint_32)16, 8, 2, 0, 0, 0);

    // step 5: Query some info fields to exercise get APIs
    png_uint_32 img_h = png_get_image_height((png_const_structrp)write_ptr, (png_const_inforp)write_info);
    png_byte bit_depth = png_get_bit_depth((png_const_structrp)write_ptr, (png_const_inforp)write_info);

    // step 6: Cleanup
    png_destroy_info_struct((png_const_structrp)write_ptr, (png_infopp)&write_info);
    png_free((png_const_structrp)write_ptr, (png_voidp)write_ptr);

    (void)signature;
    (void)sig_check;
    (void)img_h;
    (void)bit_depth;
    return 66;
}