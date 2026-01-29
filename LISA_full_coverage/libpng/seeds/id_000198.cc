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
//<ID> 198
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structrp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);
    png_alloc_size_t chunk_max = (png_alloc_size_t)65536;
    png_byte * trans_alpha = (png_byte *)malloc((size_t)4);
    trans_alpha[0] = 0xFF;
    trans_alpha[1] = 0x80;
    trans_alpha[2] = 0x40;
    trans_alpha[3] = 0x00;
    png_color_16 trans_color_storage;
    trans_color_storage.index = 1;
    trans_color_storage.red = 0x1234;
    trans_color_storage.green = 0x2345;
    trans_color_storage.blue = 0x3456;
    trans_color_storage.gray = 0x0400;

    // step 2: Configure
    png_set_chunk_malloc_max((png_structrp)png_ptr, chunk_max);
    png_set_tRNS((png_structrp)png_ptr, (png_inforp)info_ptr, (png_const_bytep)trans_alpha, 3, (png_const_color_16p)&trans_color_storage);

    // step 3: Operate / Validate
    int unit = 0;
    char * swidth_buf = (char *)malloc((size_t)16);
    char * sheight_buf = (char *)malloc((size_t)16);
    memset(swidth_buf, 0, (size_t)16);
    memset(sheight_buf, 0, (size_t)16);
    /* provide initial string values that could plausibly be returned or used */
    memcpy(swidth_buf, "1.0\0", 4);
    memcpy(sheight_buf, "1.0\0", 4);
    char * swidth_p = swidth_buf;
    char * sheight_p = sheight_buf;
    png_uint_32 scal_result = png_get_sCAL_s((png_const_structrp)png_ptr, (png_const_inforp)info_ptr, &unit, (png_charpp)&swidth_p, (png_charpp)&sheight_p);
    /* mix returned values into stored state to create meaningful data flow */
    png_uint_32 mixed = (png_uint_32)((scal_result ^ (png_uint_32)(chunk_max & 0xFFFFFFFFu)) + (png_uint_32)unit);
    trans_alpha[0] = (png_byte)(mixed & 0xFFu);
    trans_color_storage.gray = (png_uint_16)((mixed >> 8) & 0xFFFFu);
    png_chunk_benign_error((png_const_structrp)png_ptr, (png_const_charp)"benign: sCAL and tRNS sequence checked");

    // step 4: Cleanup
    free(trans_alpha);
    free(swidth_buf);
    free(sheight_buf);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}