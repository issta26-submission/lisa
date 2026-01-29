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
//<ID> 89
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
    png_info *info_ptr = (png_info *)NULL;

    // step 2: Configure
    (void)png_set_option((png_structrp)png_ptr, 3, 1);
    png_const_charp copyright = png_get_copyright((png_const_structrp)png_ptr);
    png_bytep buf = (png_bytep)malloc((size_t)4);
    memset(buf, 0, (size_t)4);
    png_uint_32 save_val = (png_uint_32)0xDEADBEEFu;
    png_save_uint_32(buf, save_val);

    // step 3: Operate / Validate
    png_uint_32 first_byte = (png_uint_32)buf[0];
    png_uint_32 combined = save_val ^ first_byte;
    (void)combined;
    (void)copyright;

    // step 4: Cleanup
    free(buf);
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}