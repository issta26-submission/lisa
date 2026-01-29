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
//<ID> 110
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_const_charp verstr = png_get_header_ver((png_const_structrp)png_ptr);
    png_uint_32 vernum = png_get_uint_32((png_const_bytep)verstr);

    // step 2: Configure
    int reset_rc = png_reset_zstream((png_structrp)png_ptr);
    char chunk_name[] = "TEST";
    size_t data_len = (size_t)((vernum & 0xFFu) + (unsigned)reset_rc + 4u);
    unsigned char * data = (unsigned char *)malloc(data_len);
    memset(data, 0, data_len);
    data[0] = (unsigned char)(vernum & 0xFFu);
    data[1] = (unsigned char)((vernum >> 8) & 0xFFu);
    data[2] = (unsigned char)(reset_rc & 0xFF);
    data[3] = (unsigned char)((vernum ^ (png_uint_32)reset_rc) & 0xFFu);

    // create an info struct to use with a setter API (avoids calling any write callback)
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);

    // step 3: Operate / Validate
    // use png_set_iCCP to pass our prepared data into libpng (does not require a write function)
    png_set_iCCP((png_const_structrp)png_ptr, (png_inforp)info_ptr, (png_const_charp)chunk_name, 0, (png_const_bytep)data, (png_uint_32)data_len);
    png_uint_32 checksum = vernum ^ (png_uint_32)data[0] ^ (png_uint_32)reset_rc;
    (void)checksum;

    // step 4: Cleanup
    free(data);
    png_destroy_info_struct((png_const_structrp)png_ptr, (png_infopp)&info_ptr);
    png_destroy_read_struct(&png_ptr_nonrestrict, (png_infopp)NULL, (png_infopp)NULL);

    return 66;
}