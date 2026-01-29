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
//<ID> 353
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
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, user_mem, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_inforp info_ptr = (png_inforp)NULL;

    // step 2: Configure
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);
    png_alloc_size_t exif_size = chunk_max + (png_alloc_size_t)24;
    void * exif_buf = png_malloc((png_const_structrp)png_ptr, exif_size);
    memset(exif_buf, 0xE1, (size_t)exif_size);
    png_fixed_point screen_gamma = (png_fixed_point)22000;
    png_fixed_point override_file_gamma = (png_fixed_point)10000;
    png_set_gamma_fixed((png_structrp)png_ptr, screen_gamma, override_file_gamma);

    // step 3: Operate / Validate
    int scal_unit = 0;
    char * scal_swidth = (char *)NULL;
    char * scal_sheight = (char *)NULL;
    png_uint_32 scal_result = png_get_sCAL_s((png_const_structrp)png_ptr, (png_const_inforp)info_ptr, &scal_unit, (png_charpp)&scal_swidth, (png_charpp)&scal_sheight);
    ((unsigned char *)exif_buf)[0] = (unsigned char)(scal_result & 0xFFu);
    png_set_eXIf_1((png_const_structrp)png_ptr, info_ptr, (png_uint_32)exif_size, (png_bytep)exif_buf);

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, exif_buf);
    free(user_mem);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}