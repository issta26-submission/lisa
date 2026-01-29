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
//<ID> 228
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = (png_infop)NULL;
    unsigned char sig[8] = {0};
    sig[0] = 0x89u;
    int sig_ok = png_sig_cmp((png_const_bytep)sig, (size_t)0, (size_t)8);

    // step 2: Configure (use a read-appropriate configuration API)
    png_set_text_compression_strategy((png_structrp)png_ptr, sig_ok & 3);

    // step 3: Operate / Validate
    png_int_32 x_off = png_get_x_offset_microns((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    int combined = (int)(x_off ^ (png_int_32)sig_ok);
    png_set_chunk_malloc_max((png_structrp)png_ptr, (png_alloc_size_t)combined);

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}