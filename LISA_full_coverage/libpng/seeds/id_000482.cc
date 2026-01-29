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
//<ID> 482
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Create read struct and info struct
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37",
                                                   (png_voidp)NULL,
                                                   (png_error_ptr)NULL,
                                                   (png_error_ptr)NULL,
                                                   (png_voidp)NULL,
                                                   (png_malloc_ptr)NULL,
                                                   (png_free_ptr)NULL);
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);

    // step 2: Configure
    png_set_invert_mono((png_structrp)png_ptr);

    // step 3: Operate / Validate
    png_uint_32 ppm = png_get_pixels_per_meter((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    unsigned char buf[2];
    memset(buf, 0, 2);
    png_save_uint_16((png_bytep)buf, (unsigned int)(ppm & 0xFFFF));
    png_const_charp ver = png_get_libpng_ver((png_const_structrp)png_ptr);
    (void)ver;
    (void)ppm;
    (void)buf;

    // step 4: Cleanup (use the write-struct destroy API to free structures; cast addresses to expected pointer types)
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr);

    return 66;
}