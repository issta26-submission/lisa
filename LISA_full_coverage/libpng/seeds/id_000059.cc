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
//<ID> 59
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Initialize
    png_structrp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = (png_info *)info_ptr;

    // step 2: Configure
    png_set_IHDR((png_const_structrp)png_ptr, (png_inforp)info_ptr, (png_uint_32)16, (png_uint_32)8, 8, 2, 0, 0, 0);

    // step 3: Operate / Validate
    png_uint_32 libver = png_access_version_number();
    png_const_charp header_ver = png_get_header_version((png_const_structrp)png_ptr);
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);
    size_t rowbytes = png_get_rowbytes((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_uint_32 width = (png_uint_32)16;
    png_uint_32 height = (png_uint_32)8;
    size_t buffer_size = rowbytes * (size_t)height;
    png_bytep buffer = (png_bytep)malloc(buffer_size);
    memset(buffer, 0x7F, buffer_size);
    png_bytepp rows = (png_bytepp)malloc(sizeof(png_bytep) * (size_t)height);
    rows[0] = buffer;
    rows[1] = buffer + rowbytes;
    rows[2] = buffer + rowbytes * 2;
    rows[3] = buffer + rowbytes * 3;
    rows[4] = buffer + rowbytes * 4;
    rows[5] = buffer + rowbytes * 5;
    rows[6] = buffer + rowbytes * 6;
    rows[7] = buffer + rowbytes * 7;

    // step 4: Cleanup
    free(rows);
    free(buffer);
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    (void)libver;
    (void)header_ver;
    (void)chunk_max;
    (void)rowbytes;
    (void)width;
    (void)height;
    return 66;
}