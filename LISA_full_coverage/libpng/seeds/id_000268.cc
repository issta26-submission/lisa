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
//<ID> 268
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_a = png_create_write_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);
    png_voidp mem_a = png_get_mem_ptr((png_const_structrp)png_a);
    png_structp png_b = png_create_write_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, mem_a, (png_malloc_ptr)NULL, (png_free_ptr)NULL);

    // step 2: Configure
    png_set_alpha_mode((png_structrp)png_b, 1, 2.2);

    // step 3: Operate / Validate
    double vals[10];
    memset(vals, 0, sizeof(vals));
    png_uint_32 mdcv_flags = png_get_mDCV((png_const_structrp)png_b, (png_const_inforp)NULL,
                                         &vals[0], &vals[1],
                                         &vals[2], &vals[3],
                                         &vals[4], &vals[5],
                                         &vals[6], &vals[7],
                                         &vals[8], &vals[9]);
    unsigned char outbuf[2];
    png_save_uint_16((png_bytep)outbuf, (unsigned int)(mdcv_flags & 0xFFFF));
    (void)outbuf;
    (void)vals;
    (void)mdcv_flags;

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_b, mem_a);
    png_free_default((png_const_structrp)png_b, (png_voidp)png_b);
    png_free_default((png_const_structrp)png_a, (png_voidp)png_a);

    // API sequence test completed successfully
    return 66;
}