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
//<ID> 102
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_inforp info_ptr = (png_inforp)png_create_info_struct((png_const_structrp)png_ptr);

    // step 2: Configure
    (void)png_set_option((png_structrp)png_ptr, 3, 1);
    png_set_compression_level((png_structrp)png_ptr, 6);
    png_uint_32 height_max = png_get_user_height_max((png_const_structrp)png_ptr);
    png_bytep exif = (png_bytep)malloc((size_t)16);
    memset(exif, 0, (size_t)16);
    exif[0] = (png_byte)(height_max & 0xFFu);
    exif[1] = (png_byte)((height_max >> 8) & 0xFFu);
    exif[2] = (png_byte)0xA5u;
    exif[3] = (png_byte)0x5Au;
    png_set_eXIf_1((png_const_structrp)png_ptr, info_ptr, (png_uint_32)1u, exif);

    // step 3: Operate / Validate
    png_uint_32 check = height_max ^ (png_uint_32)exif[0];
    (void)check;
    png_uint_32 height_verify = png_get_user_height_max((png_const_structrp)png_ptr);
    (void)height_verify;

    // step 4: Cleanup
    free(exif);
    png_free_data((png_const_structrp)png_ptr, info_ptr, (png_uint_32)0u, 0);
    png_data_freer((png_const_structrp)png_ptr, info_ptr, 1, (png_uint_32)0xFFFFFFFFu);

    // API sequence test completed successfully
    return 66;
}