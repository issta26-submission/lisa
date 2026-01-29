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
//<ID> 101
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = (png_info *)info_ptr;
    png_uint_32 initial_height_max = png_get_user_height_max((png_const_structrp)png_ptr);
    png_uint_32 num_exif = (png_uint_32)16u;
    png_bytep exif = (png_bytep)malloc((size_t)num_exif);
    memset(exif, 0, (size_t)num_exif);

    // step 2: Configure
    (void)png_set_option((png_structrp)png_ptr, 3, 1);
    png_set_compression_level((png_structrp)png_ptr, 6);
    exif[0] = (png_byte)(initial_height_max & 0xFFu);
    exif[1] = (png_byte)((initial_height_max >> 8) & 0xFFu);

    // step 3: Operate / Validate
    png_set_eXIf_1((png_const_structrp)png_ptr, info_ptr, num_exif, exif);
    png_uint_32 after_height_max = png_get_user_height_max((png_const_structrp)png_ptr);
    png_uint_32 combined = after_height_max ^ num_exif ^ (png_uint_32)exif[0];
    (void)combined;
    (void)initial_height_max;

    // step 4: Cleanup
    free(exif);
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}