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
//<ID> 53
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

    // step 2: Configure
    png_set_IHDR((png_const_structrp)png_ptr, (png_inforp)info_ptr, (png_uint_32)64, (png_uint_32)32, 8, 2, 0, 0, 0);
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);
    png_set_chunk_malloc_max((png_structrp)png_ptr, chunk_max);
    png_const_charp header_ver = png_get_header_version((png_const_structrp)png_ptr);
    png_set_read_user_chunk_fn((png_structrp)png_ptr, (png_voidp)header_ver, (png_user_chunk_ptr)NULL);

    // step 3: Operate
    png_uint_32 ver_num = png_access_version_number();
    size_t rowbytes = png_get_rowbytes((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_alloc_size_t alloc_size = (png_alloc_size_t)rowbytes * (png_alloc_size_t)32;
    unsigned char *buffer = (unsigned char *)malloc((size_t)alloc_size);
    memset(buffer, (int)(ver_num & 0xFF), (size_t)alloc_size);

    // step 4: Cleanup / Validate
    free(buffer);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = (png_info *)info_ptr;
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}