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
//<ID> 111
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_uint_32 version_num = png_access_version_number();
    png_const_charp user_ver = (png_const_charp)"1.6.37";
    png_structp png_ptr = png_create_read_struct(user_ver, (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_bytep data = (png_bytep)malloc((size_t)8);
    memset(data, 0, (size_t)8);

    // store the version number into the buffer using a safe png helper
    png_save_uint_32(&data[0], version_num);

    // step 2: Configure (use harmless configuration APIs that do not require IO callbacks)
    png_set_compression_level((png_structrp)png_ptr, 6);
    png_set_text_compression_level((png_structrp)png_ptr, 2);

    // step 3: Operate / Validate (use a warning call rather than a write that requires a write callback)
    png_warning((png_const_structrp)png_ptr, (png_const_charp)"test warning from fuzzer sequence");
    png_uint_32 checksum = version_num ^ (png_uint_32)data[0] ^ (png_uint_32)data[3];
    (void)checksum;

    // step 4: Cleanup
    free(data);
    png_destroy_read_struct(&png_ptr_nonrestrict, (png_infopp)NULL, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}