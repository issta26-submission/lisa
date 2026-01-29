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
//<ID> 384
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // Create a read struct
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);

    // Query version string from the created struct
    png_const_charp header_ver = png_get_header_version((png_const_structrp)png_ptr);

    // Set custom memory functions (NULL here, just exercising the API)
    png_set_mem_fn((png_structrp)png_ptr, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);

    // Retrieve the memory pointer set on png_ptr
    png_voidp mem_ptr = png_get_mem_ptr((png_const_structrp)png_ptr);

    // Get chunk malloc maximum and adjust the chunk cache maximum
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);
    png_set_chunk_cache_max((png_structrp)png_ptr, (png_uint_32)1024);

    // Query IO state and error pointer
    png_uint_32 io_state = png_get_io_state((png_const_structrp)png_ptr);
    png_voidp err_ptr = png_get_error_ptr((png_const_structrp)png_ptr);

    // Call a benign warning function (no longjmp)
    png_benign_error((png_const_structrp)png_ptr, (png_const_charp)"benign test warning");

    // Get user transform pointer and current pass number
    png_voidp user_transform = png_get_user_transform_ptr((png_const_structrp)png_ptr);
    png_byte pass_number = png_get_current_pass_number((png_const_structrp)png_ptr);

    // Toggle a few transformation/settings APIs (no-op of real behavior here)
    png_set_alpha_mode((png_structrp)png_ptr, 1, 2.2);
    png_set_check_for_invalid_index((png_structrp)png_ptr, 1);
    png_set_strip_alpha((png_structrp)png_ptr);

    // Use the gathered values in trivial way to avoid "unused variable" warnings
    (void)header_ver;
    (void)mem_ptr;
    (void)chunk_max;
    (void)io_state;
    (void)err_ptr;
    (void)user_transform;
    (void)pass_number;

    return 66;
}