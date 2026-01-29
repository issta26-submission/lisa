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
//<ID> 358
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    void *user_mem = malloc(128);
    memset(user_mem, 0xA5, 128);
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = (png_inforp)NULL;

    png_alloc_size_t chunk_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);
    png_alloc_size_t exif_size = chunk_max + (png_alloc_size_t)16;
    png_bytep exif = (png_bytep)malloc((size_t)exif_size);
    memset(exif, 0x5E, (size_t)exif_size);

    png_fixed_point output_gamma = (png_fixed_point)22000;
    png_set_alpha_mode_fixed((png_structrp)png_ptr, 0, output_gamma);

    int unit = 0;
    char *s_w = (char *)NULL;
    char *s_h = (char *)NULL;
    png_charpp swidth = &s_w;
    png_charpp sheight = &s_h;
    png_uint_32 scal_ret = png_get_sCAL_s((png_const_structrp)png_ptr, (png_const_inforp)info_ptr, &unit, swidth, sheight);

    exif[0] = (png_byte)(scal_ret & 0xFFu);
    exif[1] = (png_byte)(unit & 0xFF);

    png_set_eXIf((png_const_structrp)png_ptr, (png_inforp)info_ptr, exif);

    free(exif);
    free(user_mem);

    return 66;
}