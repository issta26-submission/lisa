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
//<ID> 381
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // Step 1: Declarations / Initialize
    png_struct *png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_info *info_ptr = (png_info *)NULL;
    unsigned char intbuf[4];
    png_bytep buf = (png_bytep)intbuf;
    memset(intbuf, 0, sizeof(intbuf));

    // Step 2: Configure
    png_alloc_size_t chunk_max = (png_alloc_size_t)4096;
    png_set_chunk_malloc_max((png_structrp)png_ptr, chunk_max);
    png_set_write_status_fn((png_structrp)png_ptr, (png_write_status_ptr)NULL);

    // Step 3: Operate / Validate
    png_int_32 save_val = (png_int_32)chunk_max;
    png_save_int_32(buf, save_val);
    (void)buf;
    (void)save_val;

    // Step 4: Cleanup
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);

    // API sequence test completed successfully
    return 66;
}