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
//<ID> 104
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
    png_bytep exif_buf = (png_bytep)malloc((size_t)16);
    memset(exif_buf, 0, (size_t)16);
    png_uint_32 initial_user_max = png_get_user_height_max((png_const_structrp)png_ptr);
    exif_buf[0] = (png_byte)0x45;
    exif_buf[1] = (png_byte)0x58;
    exif_buf[2] = (png_byte)(initial_user_max & 0xFF);

    // step 2: Configure
    (void)png_set_option((png_structrp)png_ptr, 3, 1);
    int compression_level = (int)((initial_user_max & 7u) + 1u);
    png_set_compression_level((png_structrp)png_ptr, compression_level);
    png_set_eXIf_1((png_const_structrp)png_ptr, info_ptr, (png_uint_32)4u, exif_buf);

    // step 3: Operate / Validate
    png_uint_32 observed_user_max = png_get_user_height_max((png_const_structrp)png_ptr);
    png_uint_32 combined_value = observed_user_max ^ (png_uint_32)exif_buf[2];
    (void)combined_value;
    (void)compression_level;

    // step 4: Cleanup
    free(exif_buf);
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}