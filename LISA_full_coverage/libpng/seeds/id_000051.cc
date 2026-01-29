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
//<ID> 51
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structrp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = (png_info *)info_ptr;
    png_uint_32 access_version = png_access_version_number();
    png_const_charp header_version = png_get_header_version((png_const_structrp)png_ptr);
    png_alloc_size_t chunk_malloc_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);

    // step 2: Configure
    png_uint_32 width = (png_uint_32)64;
    png_uint_32 height = (png_uint_32)32;
    png_set_IHDR((png_const_structrp)png_ptr, (png_inforp)info_ptr, width, height, 8, 2, 0, 0, 0);

    // step 3: Operate
    size_t rowbytes = png_get_rowbytes((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    size_t total_size = rowbytes * (size_t)height;
    unsigned char *buffer = (unsigned char *)malloc(total_size);
    memset(buffer, 0x5A, total_size);
    (void)access_version;
    (void)header_version;
    (void)chunk_malloc_max;

    // step 4: Validate / Cleanup
    free(buffer);
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);
    // API sequence test completed successfully
    return 66;
}