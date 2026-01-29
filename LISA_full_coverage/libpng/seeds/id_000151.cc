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
//<ID> 151
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    FILE *fp = fopen("out.png", "wb");
    png_init_io((png_structrp)png_ptr, fp);

    // step 2: Configure
    png_uint_32 width = 2;
    png_uint_32 height = 2;
    int bit_depth = 16;
    int color_type = 2; /* RGB */
    int interlace_method = 0;
    int compression_method = 0;
    int filter_method = 0;
    png_set_IHDR((png_const_structrp)png_ptr, info_ptr, width, height, bit_depth, color_type, interlace_method, compression_method, filter_method);
    png_set_strip_16((png_structrp)png_ptr);

    size_t rowbytes = (size_t)width * 3 * (size_t)(bit_depth / 8);
    png_bytepp rows = (png_bytepp)malloc((size_t)height * sizeof(png_bytep));
    png_bytep row0 = (png_bytep)malloc(rowbytes);
    png_bytep row1 = (png_bytep)malloc(rowbytes);
    memset(row0, 0x11, rowbytes);
    memset(row1, 0x22, rowbytes);
    rows[0] = row0;
    rows[1] = row1;
    png_set_rows((png_const_structrp)png_ptr, info_ptr, rows);

    // step 3: Operate / Validate
    png_charp purpose = (png_charp)NULL;
    png_int_32 X0 = 0;
    png_int_32 X1 = 0;
    int pcal_type = 0;
    int pcal_nparams = 0;
    png_charp units = (png_charp)NULL;
    png_charpp params = (png_charpp)NULL;
    png_uint_32 pcal_ret = png_get_pCAL((png_const_structrp)png_ptr, info_ptr, &purpose, &X0, &X1, &pcal_type, &pcal_nparams, &units, &params);

    png_const_charp copy = png_get_copyright((png_const_structrp)png_ptr);
    (void)copy;
    (void)pcal_ret;
    (void)purpose;
    (void)units;
    (void)params;

    png_voidp warnbuf = png_malloc_warn((png_const_structrp)png_ptr, (png_alloc_size_t)64);
    if (warnbuf) {
        memset(warnbuf, 0x41, 64);
    }
    unsigned char chunk_name[5] = {'t','E','X','t', 0};
    png_write_chunk((png_structrp)png_ptr, (png_const_bytep)chunk_name, (png_const_bytep)warnbuf, (size_t)16);
    png_write_image((png_structrp)png_ptr, rows);
    png_write_flush((png_structrp)png_ptr);

    // step 4: Cleanup
    free(row1);
    free(row0);
    free(rows);
    if (warnbuf) free(warnbuf);
    if (fp) fclose(fp);
    png_destroy_info_struct((png_const_structrp)png_ptr, &info_ptr);

    // API sequence test completed successfully
    return 66;
}