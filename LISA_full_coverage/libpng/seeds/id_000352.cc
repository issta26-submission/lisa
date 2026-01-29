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
//<ID> 352
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

    // step 2: Configure
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max((png_const_structrp)png_ptr);
    png_alloc_size_t exif_size = chunk_max + (png_alloc_size_t)16u;
    png_bytep exif_data = (png_bytep)png_calloc((png_const_structrp)png_ptr, exif_size);
    memset(exif_data, 0xA5, (size_t)exif_size);
    exif_data[0] = (png_byte)(exif_size & 0xFFu);

    // step 3: Operate / Validate
    png_set_eXIf_1((png_const_structrp)png_ptr, info_ptr, (png_uint_32)1u, exif_data);
    png_fixed_point screen_gamma = (png_fixed_point)22000;
    png_fixed_point override_gamma = (png_fixed_point)0;
    // Use the gAMA fixed API (expects a fixed-point file gamma) instead of the previous incorrect call
    png_set_gAMA_fixed((png_const_structrp)png_ptr, info_ptr, screen_gamma);
    int sc_unit = 0;
    char *swidth = (char *)NULL;
    char *sheight = (char *)NULL;
    png_uint_32 sc_return = png_get_sCAL_s((png_const_structrp)png_ptr, (png_const_inforp)info_ptr, &sc_unit, (png_charpp)&swidth, (png_charpp)&sheight);
    exif_data[1] = (png_byte)(sc_return & 0xFFu);

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, (png_voidp)exif_data);
    free(user_mem);
    free((void*)swidth);
    free((void*)sheight);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}