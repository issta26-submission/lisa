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
//<ID> 52
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
static void my_write_data(png_structp png_ptr, png_bytep data, png_size_t length) { (void)png_ptr; (void)data; (void)length; }
static void my_flush(png_structp png_ptr) { (void)png_ptr; }

int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structrp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_const_charp header_ver = png_get_header_version((png_const_structrp)png_ptr);
    png_uint_32 lib_ver = png_access_version_number();
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);

    // step 2: Configure
    png_set_user_limits((png_structrp)png_ptr, (png_uint_32)lib_ver, (png_uint_32)lib_ver);
    png_set_chunk_malloc_max((png_structrp)png_ptr, (png_alloc_size_t)chunk_max);
    png_set_read_fn((png_structrp)png_ptr, (png_voidp)header_ver, (png_rw_ptr)NULL);
    png_set_IHDR((png_const_structrp)png_ptr, (png_inforp)info_ptr, (png_uint_32)32, (png_uint_32)16, 8, 2, 0, 0, 0);

    // provide a real (no-op) write and flush function so write operations don't error
    png_set_write_fn((png_structrp)png_ptr, (png_voidp)header_ver, (png_rw_ptr)my_write_data, (png_flush_ptr)my_flush);

    // step 3: Operate
    size_t rowbytes = png_get_rowbytes((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    size_t height = (size_t)16;
    unsigned char *buffer = (unsigned char *)malloc(rowbytes * height);
    memset(buffer, 0x42, rowbytes * height);
    png_set_pHYs((png_const_structrp)png_ptr, (png_inforp)info_ptr, (png_uint_32)300, (png_uint_32)300, 1);
    png_write_chunk_start((png_structrp)png_ptr, (png_const_bytep)"fUTR", (png_uint_32)4);
    png_write_chunk_data((png_structrp)png_ptr, (png_const_bytep)buffer, (size_t)4);
    png_write_chunk_end((png_structrp)png_ptr);

    // step 4: Validate / Cleanup
    png_bytepp rows = png_get_rows((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = (png_info *)info_ptr;
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);
    free(buffer);
    (void)rows;
    (void)lib_ver;
    (void)chunk_max;
    (void)header_ver;

    // API sequence test completed successfully
    return 66;
}