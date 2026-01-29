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
//<ID> 473
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = (png_inforp)NULL;
    png_bytep exif = (png_bytep)png_calloc((png_const_structrp)png_ptr, (png_alloc_size_t)32);
    memset((void *)exif, 0, (size_t)32);
    exif[0] = (png_byte)'E';
    exif[1] = (png_byte)'X';
    exif[2] = (png_byte)'I';
    exif[3] = (png_byte)'F';

    // step 2: Configure
    png_set_eXIf((png_const_structrp)png_ptr, info_ptr, exif);
    png_set_text_compression_level((png_structrp)png_ptr, 5);
    png_set_compression_level((png_structrp)png_ptr, 6);

    // step 3: Operate / Validate
    png_uint_32 chunk_type = png_get_io_chunk_type((png_const_structrp)png_ptr);
    exif[0] = (png_byte)(chunk_type & 0xFF);

    // step 4: Cleanup
    png_free((png_const_structrp)png_ptr, (png_voidp)exif);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)NULL, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}