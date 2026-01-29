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
//<ID> 196
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structrp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_infop info_ptr = png_create_info_struct((png_const_structrp)png_ptr);

    // step 2: Configure
    png_alloc_size_t user_chunk_max = (png_alloc_size_t)4096;
    png_set_chunk_malloc_max((png_structrp)png_ptr, user_chunk_max);
    png_byte * trans_alpha = (png_byte *)malloc((size_t)4);
    memset((void *)trans_alpha, 0x7F, (size_t)4);
    png_color_16 trans_color;
    trans_color.index = 1;
    trans_color.red = (png_uint_16)0x0ABC;
    trans_color.green = (png_uint_16)0x0123;
    trans_color.blue = (png_uint_16)0x0456;
    trans_color.gray = (png_uint_16)0x00FF;
    png_set_tRNS((png_structrp)png_ptr, (png_inforp)info_ptr, (png_const_bytep)trans_alpha, 4, (png_const_color_16p)&trans_color);

    // step 3: Operate / Validate
    int unit = 0;
    png_charpp swidth = (png_charpp)malloc(sizeof(char *));
    png_charpp sheight = (png_charpp)malloc(sizeof(char *));
    *swidth = (char *)malloc((size_t)32);
    *sheight = (char *)malloc((size_t)32);
    memset((void *)*swidth, 'W', (size_t)32);
    memset((void *)*sheight, 'H', (size_t)32);
    png_uint_32 sres = png_get_sCAL_s((png_const_structrp)png_ptr, (png_const_inforp)info_ptr, &unit, swidth, sheight);
    unsigned int mix = ((unsigned int)sres) ^ ((unsigned int)unit) ^ ((unsigned int)trans_alpha[0]);
    ((unsigned char *)(*swidth))[0] = (unsigned char)(mix & 0xFFu);
    png_chunk_benign_error((png_const_structrp)png_ptr, (png_const_charp)"benign: adjusted chunk malloc max and tRNS set");

    // step 4: Cleanup
    free((void *)*swidth);
    free((void *)*sheight);
    free((void *)swidth);
    free((void *)sheight);
    free((void *)trans_alpha);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}