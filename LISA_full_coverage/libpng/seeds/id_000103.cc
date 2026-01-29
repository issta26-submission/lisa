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
//<ID> 103
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = (png_info *)info_ptr;

    // step 2: Configure
    (void)png_set_option((png_structrp)png_ptr, 3, 1);
    png_uint_32 user_h_max = png_get_user_height_max((png_const_structrp)png_ptr);
    png_set_compression_level((png_structrp)png_ptr, 6);

    // step 3: Operate / Validate
    png_uint_32 exif_count = (png_uint_32)1;
    size_t exif_size = (size_t)((user_h_max & 0xFFu) + 16u);
    png_bytep exif_buf = (png_bytep)malloc(exif_size);
    memset(exif_buf, 0, exif_size);
    exif_buf[0] = (png_byte)(user_h_max & 0xFFu);
    png_set_eXIf_1((png_const_structrp)png_ptr, info_ptr, exif_count, exif_buf);
    png_uint_32 verify_h = png_get_user_height_max((png_const_structrp)png_ptr);
    png_uint_32 combined = verify_h ^ (png_uint_32)exif_buf[0];
    (void)combined;
    (void)user_h_max;

    // step 4: Cleanup
    free(exif_buf);
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}