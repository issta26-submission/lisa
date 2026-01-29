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
//<ID> 63
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = (png_info *)info_ptr;

    // step 2: Configure
    const png_uint_32 exif_len = (png_uint_32)64;
    png_bytep exif_buffer = (png_bytep)malloc((size_t)exif_len);
    memset(exif_buffer, 0xAB, (size_t)exif_len);
    png_set_eXIf_1((png_const_structrp)png_ptr, (png_inforp)info_ptr, (png_uint_32)exif_len, exif_buffer);
    size_t rowbytes = png_get_rowbytes((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    size_t alloc_rowbytes = rowbytes + (size_t)8;
    png_bytep image_buffer = (png_bytep)malloc(alloc_rowbytes);
    memset(image_buffer, 0x7F, alloc_rowbytes);
    png_bytep *rows = (png_bytep *)malloc(sizeof(png_bytep) * 1);
    rows[0] = image_buffer;
    png_set_rows((png_structrp)png_ptr, (png_inforp)info_ptr, (png_bytepp)rows);

    // step 3: Operate / Validate
    png_uint_32 libver = png_access_version_number();
    png_byte filter_type = png_get_filter_type((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    (void)libver;
    (void)filter_type;
    (void)rowbytes;
    (void)alloc_rowbytes;

    // step 4: Cleanup
    free(rows);
    free(image_buffer);
    free(exif_buffer);
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}