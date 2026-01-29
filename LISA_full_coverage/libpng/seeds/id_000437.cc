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
//<ID> 437
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_info *info_ptr = (png_info*)png_malloc_default((png_const_structrp)png_ptr, (size_t)128);

    // step 2: Configure
    png_set_text_compression_mem_level((png_structrp)png_ptr, 8);
    png_set_strip_alpha((png_structrp)png_ptr);
    png_set_strip_16((png_structrp)png_ptr);

    // step 3: Operate / Validate
    png_fixed_point y_offset_fp = png_get_y_offset_inches_fixed((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_uint_32 width = png_get_image_width((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_uint_32 combined = (png_uint_32)(width + (png_uint_32)((png_uint_32)((png_uint_32)y_offset_fp) & 0xFFFFFFFFU));
    png_byte buffer[8];
    memset(buffer, 0, sizeof(buffer));
    png_save_uint_32((png_bytep)buffer, combined);

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, (png_voidp)info_ptr);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)NULL, (png_infopp)NULL);

    // API sequence test completed successfully
    (void)buffer;
    (void)y_offset_fp;
    (void)combined;
    (void)width;
    return 66;
}