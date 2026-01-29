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
//<ID> 88
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structrp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = (png_info *)NULL;

    // step 2: Configure
    png_uint_32 value_to_store = (png_uint_32)0x11223344;
    png_bytep buf = (png_bytep)malloc((size_t)4);
    memset(buf, 0x00, (size_t)4);
    png_save_uint_32(buf, value_to_store);
    int option_ret = png_set_option((png_structrp)png_ptr, 1, 1);

    // step 3: Operate / Validate
    png_const_charp copyright = png_get_copyright((png_const_structrp)png_ptr);
    png_uint_32 combined = (png_uint_32)buf[0] + (png_uint_32)(option_ret & 0xFF) + (png_uint_32)(unsigned char)copyright[0];
    (void)combined;

    // step 4: Cleanup
    free((void *)buf);
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}