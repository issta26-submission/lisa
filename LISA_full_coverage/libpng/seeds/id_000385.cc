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
//<ID> 385
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37",
                                                   (png_voidp)NULL,
                                                   (png_error_ptr)NULL,
                                                   (png_error_ptr)NULL,
                                                   (png_voidp)NULL,
                                                   (png_malloc_ptr)NULL,
                                                   (png_free_ptr)NULL);
    png_info *info_ptr = (png_info *)NULL;
    png_byte int_buf[4];
    memset(int_buf, 0, sizeof(int_buf));

    // step 2: Configure
    png_set_chunk_malloc_max((png_structrp)png_ptr, (png_alloc_size_t)4096);
    png_save_uint_32((png_bytep)int_buf, (png_uint_32)0x12345678u);

    // step 3: Operate / Validate
    png_const_charp header_ver = png_get_header_version((png_const_structrp)png_ptr);
    ((unsigned char *)int_buf)[0] = (unsigned char)header_ver[0];

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}