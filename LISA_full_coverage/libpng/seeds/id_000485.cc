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
//<ID> 485
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Create a png struct suitable for write operations
    png_structp png_ptr = png_create_write_struct_2((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL);

    // step 2: Allocate a zeroed block to act as an info struct placeholder
    png_voidp info_mem = png_malloc_warn((png_const_structrp)png_ptr, (png_alloc_size_t)256);
    memset(info_mem, 0, (size_t)256);
    png_inforp info_ptr = (png_inforp)info_mem;

    // step 3: Configure the png struct with user transform info and strip alpha as an extra transform
    png_set_user_transform_info((png_structrp)png_ptr, (png_voidp)info_ptr, 8, 4);
    png_set_strip_alpha((png_structrp)png_ptr);

    // step 4: Query pCAL chunk information (will safely return 0 / leave outputs NULL/zero for our zeroed info)
    png_charp purpose = (png_charp)NULL;
    png_int_32 X0 = 0;
    png_int_32 X1 = 0;
    int type = 0;
    int nparams = 0;
    png_charp units = (png_charp)NULL;
    png_charpp params = (png_charpp)NULL;
    png_uint_32 pcal_presence = png_get_pCAL((png_const_structrp)png_ptr, info_ptr, &purpose, &X0, &X1, &type, &nparams, &units, &params);
    (void)pcal_presence;
    (void)purpose;
    (void)X0;
    (void)X1;
    (void)type;
    (void)nparams;
    (void)units;
    (void)params;

    // step 5: Free the info placeholder allocated with png_malloc_warn, then destroy the write struct
    png_free((png_const_structrp)png_ptr, info_mem);
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)NULL);

    return 66;
}