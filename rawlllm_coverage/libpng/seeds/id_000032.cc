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
//<ID> 173
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = (png_inforp)NULL;

    // step 2: Configure
    png_uint_32 width = 320;
    png_uint_32 height = 240;
    (void)width;
    (void)height;
    size_t iccp_len = 128;
    unsigned char * iccp_profile = (unsigned char *)malloc(iccp_len);
    memset(iccp_profile, 0x5A, iccp_len);
    png_set_compression_buffer_size((png_structrp)png_ptr, (size_t)4096);
    png_set_text_compression_level((png_structrp)png_ptr, 2);

    // step 3: Operate / Validate
    png_uint_32 user_hmax = png_get_user_height_max((png_const_structrp)png_ptr);
    png_uint_32 image_h = png_get_current_row_number((png_const_structrp)png_ptr);
    unsigned int mix = ((unsigned int)user_hmax) ^ ((unsigned int)image_h) ^ ((unsigned int)iccp_profile[0]);
    iccp_profile[0] = (unsigned char)(mix & 0xFF);
    (void)mix;

    // step 4: Cleanup
    png_destroy_write_struct((png_structpp)(&png_ptr), (png_infopp)(&info_ptr));
    free(iccp_profile);

    // API sequence test completed successfully
    return 66;
}