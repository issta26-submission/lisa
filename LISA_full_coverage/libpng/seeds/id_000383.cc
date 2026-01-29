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
//<ID> 383
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_struct *png_ptr = (png_struct*)0;
    png_structpp png_ptr_ptr = &png_ptr;
    png_info *info_ptr = (png_info*)0;
    png_infopp info_ptr_ptr = &info_ptr;
    png_byte buf[4];
    png_int_32 initial_value = (png_int_32)0x01020304;
    png_save_int_32(buf, initial_value);

    // step 2: Configure
    png_set_write_status_fn(png_ptr, (png_write_status_ptr)NULL);
    png_alloc_size_t chunk_cache = (png_alloc_size_t)png_get_int_32(buf);
    png_set_chunk_malloc_max(png_ptr, chunk_cache);

    // step 3: Operate / Validate
    png_int_32 roundtrip = png_get_int_32(buf);
    (void)roundtrip;

    // step 4: Cleanup
    png_destroy_write_struct(png_ptr_ptr, info_ptr_ptr);

    // API sequence test completed successfully
    return 66;
}