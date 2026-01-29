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
//<ID> 85
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structrp png_ptr = (png_structrp)png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = (png_inforp)png_create_info_struct((png_const_structrp)png_ptr);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = (png_info *)info_ptr;

    // step 2: Configure
    png_set_option((png_structrp)png_ptr, 2, 1);
    png_const_charp copyright = png_get_copyright((png_const_structrp)png_ptr);

    // step 3: Operate / Validate
    const unsigned char *c = (const unsigned char *)copyright;
    png_uint_32 combined = ((png_uint_32)c[0] << 24) | ((png_uint_32)c[1] << 16) | ((png_uint_32)c[2] << 8) | ((png_uint_32)c[3]);
    unsigned char buf[4];
    png_save_uint_32((png_bytep)buf, combined);
    unsigned int checksum = (unsigned int)buf[0] + (unsigned int)buf[1] + (unsigned int)buf[2] + (unsigned int)buf[3];
    (void)checksum;

    // step 4: Cleanup
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}