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
//<ID> 197
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_alloc_size_t requested_chunk_max = (png_alloc_size_t)65536;
    png_set_chunk_malloc_max((png_structrp)png_ptr, requested_chunk_max);

    // step 2: Configure
    png_byte trans_alpha_buf[4];
    trans_alpha_buf[0] = 0;
    trans_alpha_buf[1] = 128;
    trans_alpha_buf[2] = 255;
    trans_alpha_buf[3] = 64;
    png_color_16 trans_color;
    trans_color.index = (png_byte)2;
    trans_color.red = (png_uint_16)0x1234;
    trans_color.green = (png_uint_16)0x2345;
    trans_color.blue = (png_uint_16)0x3456;
    trans_color.gray = (png_uint_16)0x0;
    png_set_tRNS((png_structrp)png_ptr, (png_inforp)info_ptr, (png_const_bytep)trans_alpha_buf, 4, (png_const_color_16p)&trans_color);

    // step 3: Operate / Validate
    int unit = 0;
    png_charpp sw_pp = (png_charpp)malloc(sizeof(char *));
    png_charpp sh_pp = (png_charpp)malloc(sizeof(char *));
    *sw_pp = (char *)malloc((size_t)8);
    *sh_pp = (char *)malloc((size_t)8);
    memset(*sw_pp, 0, (size_t)8);
    memset(*sh_pp, 0, (size_t)8);
    (*sw_pp)[0] = '1'; (*sw_pp)[1] = '.'; (*sw_pp)[2] = '0'; (*sw_pp)[3] = '\0';
    (*sh_pp)[0] = '1'; (*sh_pp)[1] = '.'; (*sh_pp)[2] = '0'; (*sh_pp)[3] = '\0';
    png_uint_32 sCALset = png_get_sCAL_s((png_const_structrp)png_ptr, (png_const_inforp)info_ptr, &unit, sw_pp, sh_pp);
    unsigned int computed = (unsigned int)(sCALset ^ (png_uint_32)unit ^ (png_uint_32)trans_alpha_buf[1]);
    (void)computed;
    png_chunk_benign_error((png_const_structrp)png_ptr, (png_const_charp)"benign: sCAL/tRNS sequence");

    // step 4: Cleanup
    free(*sw_pp);
    free(*sh_pp);
    free(sw_pp);
    free(sh_pp);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}