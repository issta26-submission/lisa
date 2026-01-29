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
//<ID> 311
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37",
                                                  (png_voidp)NULL,
                                                  (png_error_ptr)NULL,
                                                  (png_error_ptr)NULL);

    // step 2: Configure
    png_set_bgr((png_structrp)png_ptr);
    png_set_alpha_mode((png_structrp)png_ptr, 1, 2.2);
    png_const_charp ver = png_get_header_version((png_const_structrp)png_ptr);
    png_alloc_size_t comp_buf_size = (png_alloc_size_t)((size_t)((unsigned char)ver[0]) + 4096);
    png_set_compression_buffer_size((png_structrp)png_ptr, (size_t)comp_buf_size);

    // step 3: Operate / Validate
    // Use a safe value for signature bytes (0..8); avoid previous "Too many bytes" error.
    png_set_sig_bytes((png_structrp)png_ptr, 0);
    png_voidp buffer = png_malloc_warn((png_const_structrp)png_ptr, (png_alloc_size_t)256);
    ((unsigned char *)buffer)[0] = (unsigned char)ver[0];
    ((unsigned char *)buffer)[1] = (unsigned char)ver[1];

    // step 4: Cleanup (no special free available from provided APIs; avoid further libpng state calls)
    (void)comp_buf_size;
    (void)ver;
    (void)buffer;
    return 66;
}