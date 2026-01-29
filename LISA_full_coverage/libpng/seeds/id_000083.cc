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
//<ID> 83
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structrp png_ptr = (png_structrp)png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr = (png_info *)png_create_info_struct((png_structrp)png_ptr);

    // step 2: Configure
    (void)png_set_option((png_structrp)png_ptr, 1, 1);

    // step 3: Operate / Validate
    png_const_charp cp = png_get_copyright((png_const_structrp)png_ptr);
    png_uint_32 ver = png_access_version_number();
    png_bytep buf = (png_bytep)png_malloc_warn((png_const_structrp)png_ptr, (png_alloc_size_t)4);
    memset((void *)buf, 0, (size_t)4);
    png_save_uint_32(buf, ver);
    unsigned int checksum = (unsigned int)buf[0] | ((unsigned int)buf[1] << 8) | ((unsigned int)buf[2] << 16) | ((unsigned int)buf[3] << 24);
    (void)cp;
    (void)checksum;

    // step 4: Cleanup
    png_destroy_read_struct(&png_ptr_nonrestrict, (png_infopp)&info_ptr, (png_infopp)NULL);
    // API sequence test completed successfully
    return 66;
}