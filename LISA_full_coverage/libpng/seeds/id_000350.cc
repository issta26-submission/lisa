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
//<ID> 350
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    void *user_mem = malloc((size_t)64);
    memset(user_mem, 0xA5, (size_t)64);
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_inforp info_ptr = (png_inforp)NULL;
    png_inforp end_info_ptr = (png_inforp)NULL;

    // step 2: Configure
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);
    png_alloc_size_t exif_size = chunk_max + (png_alloc_size_t)8;
    png_bytep exif_buf = (png_bytep)png_malloc((png_const_structrp)png_ptr, exif_size);
    memset(exif_buf, 0xAB, (size_t)exif_size);
    png_charpp swidth_pp = (png_charpp)malloc(sizeof(png_charp));
    png_charpp sheight_pp = (png_charpp)malloc(sizeof(png_charp));
    *swidth_pp = (png_charp)NULL;
    *sheight_pp = (png_charp)NULL;
    int scunit = 0;

    // step 3: Operate / Validate
    png_set_eXIf_1((png_const_structrp)png_ptr, info_ptr, (png_uint_32)exif_size, (png_bytep)exif_buf);
    png_set_gamma_fixed((png_structrp)png_ptr, (png_fixed_point)45455, (png_fixed_point)30000);
    png_uint_32 scalc_ret = png_get_sCAL_s((png_const_structrp)png_ptr, (png_const_inforp)info_ptr, &scunit, swidth_pp, sheight_pp);
    (void)scalc_ret;
    (void)scunit;

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, (png_voidp)exif_buf);
    free(swidth_pp);
    free(sheight_pp);
    free(user_mem);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)&end_info_ptr);

    // API sequence test completed successfully
    return 66;
}