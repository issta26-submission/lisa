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
//<ID> 193
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_alloc_size_t user_chunk_max = (png_alloc_size_t)65536u;
    png_bytep trans_alpha = (png_bytep)png_calloc((png_const_structrp)png_ptr, (png_alloc_size_t)4u);
    memset(trans_alpha, 0x7F, (size_t)4u);
    png_color_16 trans_color;
    trans_color.index = (png_byte)3u;
    trans_color.red = (png_uint_16)0x1234u;
    trans_color.green = (png_uint_16)0x5678u;
    trans_color.blue = (png_uint_16)0x9ABCu;
    trans_color.gray = (png_uint_16)0x00FFu;

    // step 2: Configure
    png_set_chunk_malloc_max((png_structrp)png_ptr, user_chunk_max);
    png_set_tRNS((png_structrp)png_ptr, (png_inforp)info_ptr, (png_const_bytep)trans_alpha, 4, (png_const_color_16p)&trans_color);

    // step 3: Operate / Validate
    int sCAL_unit = 0;
    char * swidth_buf = (char *)png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)16u);
    char * sheight_buf = (char *)png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)16u);
    memset(swidth_buf, 0x00, (size_t)16u);
    memset(sheight_buf, 0x00, (size_t)16u);
    swidth_buf[0] = '1';
    swidth_buf[1] = '.';
    swidth_buf[2] = '0';
    sheight_buf[0] = '1';
    sheight_buf[1] = '.';
    sheight_buf[2] = '0';
    char * swidth_ptr = swidth_buf;
    char * sheight_ptr = sheight_buf;
    png_uint_32 sCAL_ret = png_get_sCAL_s((png_const_structrp)png_ptr, (png_const_inforp)info_ptr, &sCAL_unit, (png_charpp)&swidth_ptr, (png_charpp)&sheight_ptr);
    (void)sCAL_unit;
    (void)sCAL_ret;
    trans_alpha[0] = (png_byte)((sCAL_ret & 0xFFu) ^ (unsigned int)trans_alpha[0]);
    png_chunk_benign_error((png_const_structrp)png_ptr, (png_const_charp)"benign: sCAL and tRNS configured");

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, (png_voidp)swidth_buf);
    png_free_default((png_const_structrp)png_ptr, (png_voidp)sheight_buf);
    png_free_default((png_const_structrp)png_ptr, (png_voidp)trans_alpha);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}