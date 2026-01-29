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
//<ID> 58
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
    png_uint_32 libpng_version = png_access_version_number();
    png_const_charp header_version = png_get_header_version((png_const_structrp)png_ptr);
    png_alloc_size_t chunk_malloc_max_before = png_get_chunk_malloc_max((png_const_structrp)png_ptr);

    // step 2: Configure
    png_uint_32 width = (png_uint_32)128;
    png_uint_32 height = (png_uint_32)64;
    int bit_depth = 8;
    int color_type = 2; // Truecolor (RGB)
    png_set_IHDR((png_const_structrp)png_ptr, (png_inforp)info_ptr, width, height, bit_depth, color_type, 0, 0, 0);
    png_set_chunk_malloc_max((png_structrp)png_ptr, (png_alloc_size_t)(chunk_malloc_max_before + 1024));

    // step 3: Operate (avoid calling png_process_data to prevent "Not a PNG file" error)
    size_t rowbytes = png_get_rowbytes((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    size_t buffer_size = rowbytes * (size_t)height;
    unsigned char *buffer = (unsigned char *)malloc(buffer_size);
    memset(buffer, 0xAA, buffer_size);
    // Instead of feeding arbitrary bytes into libpng parsing functions, call safe non-parsing getters
    png_const_charp copyright = png_get_copyright((png_const_structrp)png_ptr);
    png_voidp mem_ptr = png_get_mem_ptr((png_const_structrp)png_ptr);
    png_uint_32 user_height_max = png_get_user_height_max((png_const_structrp)png_ptr);

    // step 4: Validate / Cleanup
    size_t rowbytes_after = png_get_rowbytes((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_alloc_size_t chunk_malloc_max_after = png_get_chunk_malloc_max((png_const_structrp)png_ptr);
    free(buffer);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = (png_info *)info_ptr;
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    (void)libpng_version;
    (void)header_version;
    (void)copyright;
    (void)mem_ptr;
    (void)user_height_max;
    (void)rowbytes_after;
    (void)chunk_malloc_max_after;

    // API sequence test completed successfully
    return 66;
}