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
//<ID> 356
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
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL, user_mem, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_inforp info_ptr = (png_inforp)NULL;
    png_inforp end_info = (png_inforp)NULL;

    // step 2: Configure
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);
    png_alloc_size_t buf_size = chunk_max + (png_alloc_size_t)16;
    png_voidp exif_buf = png_calloc((png_const_structrp)png_ptr, buf_size);
    memset(exif_buf, 0xAA, (size_t)buf_size);
    png_set_gamma_fixed((png_structrp)png_ptr, (png_fixed_point)22000, (png_fixed_point)10000);

    // step 3: Operate / Validate
    png_set_eXIf_1((png_const_structrp)png_ptr, info_ptr, (png_uint_32)1u, (png_bytep)exif_buf);
    int unit = 0;
    char *swidth = (char *)NULL;
    char *sheight = (char *)NULL;
    png_charpp swidth_pp = &swidth;
    png_charpp sheight_pp = &sheight;
    png_uint_32 scals = png_get_sCAL_s((png_const_structrp)png_ptr, (png_const_inforp)info_ptr, &unit, swidth_pp, sheight_pp);
    ((unsigned char *)exif_buf)[0] = (unsigned char)(scals & 0xFF);

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, exif_buf);
    free(user_mem);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)&end_info);

    // API sequence test completed successfully
    return 66;
}