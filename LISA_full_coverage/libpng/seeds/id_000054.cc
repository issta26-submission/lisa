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
//<ID> 54
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpng_api_sequence() {
    /* step 1: Declarations / Initialize (use a write struct so write callbacks exist) */
    png_structp png_ptr = png_create_write_struct_2(
        (png_const_charp)"1.6.37",
        (png_voidp)NULL,
        (png_error_ptr)NULL,
        (png_error_ptr)NULL,
        (png_voidp)NULL,
        (png_malloc_ptr)NULL,
        (png_free_ptr)NULL
    );
    png_infop info_ptr = png_create_info_struct(png_ptr);
    png_uint_32 access_ver = png_access_version_number();
    png_const_charp header_ver = png_get_header_version((png_const_structrp)png_ptr);

    /* ensure png has a valid write function by attaching a FILE* */
    FILE *fp = tmpfile();
    png_init_io((png_structrp)png_ptr, fp);

    /* step 2: Configure */
    png_set_IHDR((png_const_structrp)png_ptr, info_ptr, (png_uint_32)4, (png_uint_32)3, 8, 2, 0, 0, 0);
    png_set_chunk_malloc_max((png_structrp)png_ptr, (png_alloc_size_t)1024);
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);

    /* step 3: Operate */
    png_size_t rowbytes = png_get_rowbytes((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    size_t total_bytes = (size_t)rowbytes * (size_t)3;
    unsigned char *buffer = (unsigned char *)malloc(total_bytes);
    memset(buffer, 0xAB, total_bytes);
    unsigned char **rows = (unsigned char **)malloc(sizeof(unsigned char *) * 3);
    rows[0] = buffer;
    rows[1] = buffer + rowbytes;
    rows[2] = buffer + 2 * rowbytes;
    const unsigned char chunk_name[4] = { 'f', 'U', 'Z', 'Z' };
    png_write_chunk_start((png_structrp)png_ptr, (png_const_bytep)chunk_name, (png_uint_32)total_bytes);
    png_write_chunk_data((png_structrp)png_ptr, (png_const_bytep)buffer, total_bytes);
    png_write_chunk((png_structrp)png_ptr, (png_const_bytep)chunk_name, (png_const_bytep)buffer, total_bytes);
    png_write_image((png_structrp)png_ptr, (png_bytepp)rows);

    /* step 4: Validate / Cleanup */
    (void)access_ver;
    (void)header_ver;
    (void)chunk_max;
    free(rows);
    free(buffer);
    fclose(fp);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = (png_info *)info_ptr;
    png_destroy_write_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict);

    /* API sequence test completed successfully */
    return 66;
}