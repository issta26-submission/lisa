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
//<ID> 474
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    png_structp png_ptr = png_create_write_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_bytep exif = (png_bytep)png_malloc_warn((png_const_structrp)png_ptr, (png_alloc_size_t)64);
    exif[0] = (png_byte)'E'; exif[1] = (png_byte)'X'; exif[2] = (png_byte)'I'; exif[3] = (png_byte)'F';
    png_set_eXIf_1((png_const_structrp)png_ptr, (png_inforp)info_ptr, (png_uint_32)4, (png_bytep)exif);
    png_color_16 bg;
    png_color_16p bgp = &bg;
    png_uint_32 bg_flags = png_get_bKGD((png_const_structrp)png_ptr, (png_inforp)info_ptr, &bgp);
    (void)bg_flags;
    (void)bg.index;
    (void)bg.red;
    (void)bg.green;
    (void)bg.blue;
    (void)bg.gray;
    (void)bgp;
    png_destroy_info_struct((png_const_structrp)png_ptr, (png_infopp)&info_ptr);
    png_free_default((png_const_structrp)png_ptr, (png_voidp)exif);
    png_free_default((png_const_structrp)png_ptr, (png_voidp)png_ptr);
    return 66;
}