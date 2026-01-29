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
//<ID> 486
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Create png read struct and info struct
    png_structp png_ptr = png_create_read_struct_2((png_const_charp)"1.6.37",
                                                   (png_voidp)NULL,
                                                   (png_error_ptr)NULL,
                                                   (png_error_ptr)NULL,
                                                   (png_voidp)NULL,
                                                   (png_malloc_ptr)NULL,
                                                   (png_free_ptr)NULL);
    png_inforp info_ptr = png_create_info_struct((png_const_structrp)png_ptr);

    // step 2: Allocate a small scratch buffer via png_calloc and configure user transform info
    png_voidp scratch = png_calloc((png_const_structrp)png_ptr, (png_alloc_size_t)16);
    png_set_user_transform_info((png_structrp)png_ptr, scratch, 8, 3);

    // step 3: Query pCAL chunk data
    png_charp purpose = (png_charp)NULL;
    png_int_32 X0 = 0;
    png_int_32 X1 = 0;
    int pcal_type = 0;
    int nparams = 0;
    png_charp units = (png_charp)NULL;
    png_charpp params = (png_charpp)NULL;
    png_uint_32 pcal_flags = png_get_pCAL((png_const_structrp)png_ptr, info_ptr,
                                          &purpose, &X0, &X1, &pcal_type, &nparams, &units, &params);

    // store the 32-bit returned flags into the scratch buffer (little-endian byte order)
    unsigned char *s = (unsigned char *)scratch;
    s[0] = (unsigned char)(pcal_flags & 0xFFu);
    s[1] = (unsigned char)((pcal_flags >> 8) & 0xFFu);
    s[2] = (unsigned char)((pcal_flags >> 16) & 0xFFu);
    s[3] = (unsigned char)((pcal_flags >> 24) & 0xFFu);

    // silence unused-variable warnings
    (void)purpose;
    (void)X0;
    (void)X1;
    (void)pcal_type;
    (void)nparams;
    (void)units;
    (void)params;

    // step 4: Free scratch buffer (use deprecated API if present)
    png_free_default((png_const_structrp)png_ptr, scratch);

    // Note: intentionally do not call destroy/free functions for png_ptr/info_ptr
    // to keep the sequence straight-line as requested.

    return 66;
}