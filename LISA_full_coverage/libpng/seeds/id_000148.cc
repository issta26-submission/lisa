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
//<ID> 148
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_struct *png_ptr_nonrestrict = (png_struct *)png_ptr;
    png_info *info_ptr_nonrestrict = (png_info *)info_ptr;

    png_color_16 background;
    background.index = (png_byte)0;
    background.red = (png_uint_16)300;
    background.green = (png_uint_16)400;
    background.blue = (png_uint_16)500;
    background.gray = (png_uint_16)128;

    // step 2: Configure
    png_uint_32 width = 3;
    png_uint_32 height = 2;
    int bit_depth = 8;
    int color_type = 2; /* RGB */
    int interlace_method = 0;
    int compression_method = 0;
    int filter_method = 0;
    png_set_IHDR((png_const_structrp)png_ptr, info_ptr, width, height, bit_depth, color_type, interlace_method, compression_method, filter_method);
    png_set_bKGD((png_const_structrp)png_ptr, info_ptr, (png_const_color_16p)&background);

    size_t rowbytes = (size_t)width * 3;
    png_bytepp rows = (png_bytepp)malloc(sizeof(png_bytep) * (size_t)height);
    png_bytep row0 = (png_bytep)malloc(rowbytes);
    png_bytep row1 = (png_bytep)malloc(rowbytes);
    memset(row0, 0x11, rowbytes);
    memset(row1, 0x22, rowbytes);
    rows[0] = row0;
    rows[1] = row1;
    png_set_rows((png_structrp)png_ptr, info_ptr, rows);

    // step 3: Operate / Validate
    png_uint_32 gwidth = 0;
    png_uint_32 gheight = 0;
    int gbit = 0;
    int gcolor = 0;
    int ginterlace = 0;
    int gcompression = 0;
    int gfilter = 0;
    png_get_IHDR((png_const_structrp)png_ptr, (png_const_inforp)info_ptr, &gwidth, &gheight, &gbit, &gcolor, &ginterlace, &gcompression, &gfilter);
    png_bytepp got_rows = png_get_rows((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    png_byte first_pixel = got_rows[0][0];
    unsigned int mix = ((unsigned int)gwidth) ^ ((unsigned int)gheight) ^ ((unsigned int)gbit) ^ ((unsigned int)background.red) ^ ((unsigned int)first_pixel);
    (void)mix;

    // step 4: Cleanup
    free(row1);
    free(row0);
    free(rows);
    png_destroy_read_struct(&png_ptr_nonrestrict, &info_ptr_nonrestrict, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}