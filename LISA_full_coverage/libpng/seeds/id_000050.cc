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
//<ID> 50
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
    png_uint_32 lib_access_version = png_access_version_number();
    png_const_charp header_version = png_get_header_version((png_const_structrp)png_ptr);
    png_alloc_size_t initial_chunk_malloc_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);

    // step 2: Configure
    png_set_chunk_malloc_max((png_structrp)png_ptr, initial_chunk_malloc_max);
    png_set_user_limits((png_structrp)png_ptr, lib_access_version, lib_access_version);
    png_set_IHDR((png_const_structrp)png_ptr, (png_inforp)info_ptr, (png_uint_32)128, (png_uint_32)64, 8, 2, 0, 0, 0);
    png_set_oFFs((png_const_structrp)png_ptr, (png_inforp)info_ptr, (png_int_32)0, (png_int_32)0, 1);

    // step 3: Operate
    size_t rowbytes = png_get_rowbytes((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    size_t total_bytes = rowbytes * (size_t)64;
    unsigned char * buffer = (unsigned char *)malloc(total_bytes ? total_bytes : 1);
    memset(buffer, 0xAA, total_bytes ? total_bytes : 1);
    png_set_sRGB_gAMA_and_cHRM((png_const_structrp)png_ptr, (png_inforp)info_ptr, 0);

    // step 4: Validate / Cleanup
    png_struct * png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info * info_ptr_nonrestrict = (png_info *)info_ptr;
    free(buffer);
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);
    (void)header_version;
    (void)initial_chunk_malloc_max;
    // API sequence test completed successfully
    return 66;
}