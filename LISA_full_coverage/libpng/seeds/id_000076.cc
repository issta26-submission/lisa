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
//<ID> 76
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structrp png_ptr = (png_structrp)png_create_read_struct(
        "1.6.37", /* user_png_ver */
        NULL,     /* error_ptr */
        NULL,     /* error_fn */
        NULL      /* warn_fn */
    );
    png_inforp info_ptr = (png_inforp)malloc(192);
    memset(info_ptr, 0, 192);

    // step 2: Configure
    png_uint_32 width = (png_uint_32)640;
    png_uint_32 height = (png_uint_32)480;
    int bit_depth = 8;
    int color_type = 6; // RGBA
    int interlace_method = 0;
    int compression_method = 0;
    int filter_method = 0;

    // ensure the png struct has user limits large enough to avoid IHDR limit warnings
    png_set_user_limits((png_structrp)png_ptr, width + 1, height + 1);
    png_set_sig_bytes((png_structrp)png_ptr, 0);
    png_set_read_status_fn((png_structrp)png_ptr, (png_read_status_ptr)NULL);
    png_set_benign_errors((png_structrp)png_ptr, 1);

    // step 3: Operate / Validate
    png_byte ct = png_get_color_type((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    unsigned int checksum = (unsigned int)width + (unsigned int)height + (unsigned int)ct;
    (void)bit_depth;
    (void)color_type;
    (void)interlace_method;
    (void)compression_method;
    (void)filter_method;
    (void)checksum;

    // step 4: Cleanup
    free(info_ptr);
    // Note: png_destroy_read_struct isn't present in the provided API list, so avoid calling it.
    // Let the created png_struct leak (test harness will reclaim process memory).

    return 66;
}