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
//<ID> 364
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_const_charp header_ver = png_get_header_version((png_const_structrp)png_ptr);

    // step 2: Configure
    png_set_text_compression_level((png_structrp)png_ptr, 6);
    png_unknown_chunk unknown;
    memset(&unknown, 0, sizeof(unknown));
    unknown.name[0] = (png_byte)'u';
    unknown.name[1] = (png_byte)'n';
    unknown.name[2] = (png_byte)'k';
    unknown.name[3] = (png_byte)'1';
    unknown.name[4] = 0;
    unknown.size = (size_t)16;
    unknown.data = (png_byte *)png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)unknown.size);
    memset(unknown.data, 0xA5, unknown.size);
    // png_set_unknown_chunks requires a non-zero valid location; set to 1 to satisfy the expectation.
    unknown.location = (png_byte)1;
    png_set_unknown_chunks((png_const_structrp)png_ptr, info_ptr, (png_const_unknown_chunkp)&unknown, 1);

    // step 3: Operate / Validate
    ((unsigned char *)unknown.data)[0] = (unsigned char)header_ver[0];
    (void)header_ver;

    // step 4: Cleanup
    png_free((png_const_structrp)png_ptr, (png_voidp)unknown.data);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}