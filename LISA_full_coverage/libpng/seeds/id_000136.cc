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
//<ID> 136
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // Create a write struct with default (NULL) callbacks and allocators
    png_structp png_ptr = png_create_write_struct_2(
        (png_const_charp)"1.6.37",
        (png_voidp)NULL,
        (png_error_ptr)NULL,
        (png_error_ptr)NULL,
        (png_voidp)NULL,
        (png_malloc_ptr)NULL,
        (png_free_ptr)NULL
    );

    // Configure basic write-state and signature handling
    png_set_write_status_fn((png_structrp)png_ptr, (png_write_status_ptr)NULL);
    png_set_sig_bytes((png_structrp)png_ptr, 8);
    png_write_sig((png_structrp)png_ptr);

    // Tune compression-related parameters
    png_set_compression_buffer_size((png_structrp)png_ptr, (size_t)16384);
    png_set_compression_window_bits((png_structrp)png_ptr, 15);
    png_set_compression_mem_level((png_structrp)png_ptr, 8);
    png_set_text_compression_level((png_structrp)png_ptr, 6);

    // Allocate and free a small buffer through libpng helpers
    png_voidp buffer = png_malloc((png_const_structrp)png_ptr, (png_alloc_size_t)256);
    png_free_default((png_const_structrp)png_ptr, buffer);

    // Clean up the write struct
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)NULL);

    return 66;
}