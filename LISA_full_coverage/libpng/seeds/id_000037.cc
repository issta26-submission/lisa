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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structrp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = (png_uint_32)1;
    image.width = (png_uint_32)2;
    image.height = (png_uint_32)2;
    image.format = (png_uint_32)0;
    image.flags = (png_uint_32)0;
    image.colormap_entries = (png_uint_32)0;
    image.warning_or_error = (png_uint_32)0;

    // step 2: Prepare pixel buffer
    png_int_32 row_stride = (png_int_32)(image.width);
    unsigned char *buffer = (unsigned char *)malloc((size_t)(row_stride * image.height));
    memset(buffer, 0x7F, (size_t)(row_stride * image.height));

    // step 3: Operate - write image to a file using the png_image API
    FILE *f = fopen("out.png", "wb");
    int write_result = png_image_write_to_stdio(&image, f, 1, (const void *)buffer, row_stride, (const void *)NULL);
    fclose(f);

    // step 4: Cleanup
    free(buffer);
    png_struct *png_ptr_nonrestrict = png_ptr;
    png_destroy_read_struct(&png_ptr_nonrestrict, (png_infopp)NULL, (png_infopp)NULL);

    (void)write_result;
    (void)row_stride;
    return 66;
}