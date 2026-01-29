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
//<ID> 355
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
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37",
                                                   (png_voidp)user_mem,
                                                   (png_error_ptr)NULL,
                                                   (png_error_ptr)NULL,
                                                   (png_voidp)NULL,
                                                   (png_malloc_ptr)NULL,
                                                   (png_free_ptr)NULL);
    png_inforp info_ptr = (png_inforp)NULL;
    png_inforp end_info = (png_inforp)NULL;

    // step 2: Configure / allocate buffer using png_malloc to avoid sizeof(incomplete type)
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);
    png_alloc_size_t exif_size = chunk_max + (png_alloc_size_t)32u;
    png_bytep exif_buf = (png_bytep)png_malloc((png_const_structrp)png_ptr, exif_size);
    memset(exif_buf, 0xEE, (size_t)exif_size);

    // step 3: Operate / Validate
    png_set_gamma_fixed((png_structrp)png_ptr, (png_fixed_point)22000, (png_fixed_point)45455);
    int sCAL_unit = 0;
    char *sCAL_width = (char *)NULL;
    char *sCAL_height = (char *)NULL;
    png_uint_32 sCAL_len = png_get_sCAL_s((png_const_structrp)png_ptr, (png_const_inforp)info_ptr,
                                          &sCAL_unit, (png_charpp)&sCAL_width, (png_charpp)&sCAL_height);
    exif_buf[0] = (png_byte)(sCAL_len & 0xFFu);

    // step 4: Cleanup
    free((void *)exif_buf);
    free(user_mem);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)&end_info);

    // API sequence test completed successfully
    return 66;
}