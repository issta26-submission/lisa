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
//<ID> 351
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    void *user_mem = malloc(128);
    memset(user_mem, 0x5A, 128);
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL, NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_inforp info_ptr = (png_inforp)NULL;
    png_inforp end_info_ptr = (png_inforp)NULL;

    // step 2: Configure
    if (png_ptr) {
        info_ptr = (png_inforp)png_create_info_struct((png_const_structrp)png_ptr);
    }
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);
    png_alloc_size_t buf_size = chunk_max + (png_alloc_size_t)32;
    png_voidp buffer = png_calloc((png_const_structrp)png_ptr, buf_size);
    memset(buffer, 0xA5, (size_t)buf_size);
    png_alloc_size_t exif_size = (png_alloc_size_t)64;
    png_bytep exif = (png_bytep)png_calloc((png_const_structrp)png_ptr, exif_size);
    memset(exif, 0xE7, (size_t)exif_size);
    png_uint_32 num_exif = (png_uint_32)1u;
    png_set_eXIf_1((png_const_structrp)png_ptr, info_ptr, num_exif, (png_bytep)exif);
    png_fixed_point screen_gamma = (png_fixed_point) (1 << 16);     // 1.0 in fixed-point
    png_fixed_point override_gamma = (png_fixed_point) (int)(1.6 * (1 << 16)); // ~1.6
    png_set_gamma_fixed((png_structrp)png_ptr, screen_gamma, override_gamma);

    // step 3: Operate / Validate
    int unit = 0;
    char *swidth = NULL;
    char *sheight = NULL;
    png_uint_32 sCAL_res = png_get_sCAL_s((png_const_structrp)png_ptr, (png_const_inforp)info_ptr, &unit, (png_charpp)&swidth, (png_charpp)&sheight);
    (void)sCAL_res;
    ((unsigned char *)buffer)[0] = (unsigned char)(chunk_max & 0xFF);
    ((unsigned char *)exif)[0] = (unsigned char)((chunk_max >> 8) & 0xFF);

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, buffer);
    png_free_default((png_const_structrp)png_ptr, exif);
    free(user_mem);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)&end_info_ptr);

    // API sequence test completed successfully
    return 66;
}