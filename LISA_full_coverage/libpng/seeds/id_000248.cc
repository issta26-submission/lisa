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
//<ID> 248
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_infop info_ptr = (png_infop)NULL;
    png_bytep buf = (png_bytep)NULL;
    png_time ptime;
    buf = (png_bytep)malloc((size_t)4);
    memset(buf, 0, 4);
    info_ptr = (png_infop)png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)512);
    png_info_init_3((png_infopp)&info_ptr, (size_t)512);

    // step 2: Configure
    png_set_unknown_chunk_location((png_const_structrp)png_ptr, info_ptr, 0, 1);
    png_set_tRNS_to_alpha((png_structrp)png_ptr);

    // step 3: Operate / Validate
    png_convert_from_time_t((png_timep)&ptime, (time_t)1609459200);
    png_start_read_image((png_structrp)png_ptr);
    png_save_uint_32((png_bytep)buf, (png_uint_32)0xDEADBEEF);
    png_uint_32 read_back = png_get_uint_32((png_const_bytep)buf);
    (void)read_back;

    // step 4: Cleanup
    png_destroy_info_struct((png_const_structrp)png_ptr, (png_infopp)&info_ptr);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    free(buf);

    // API sequence test completed successfully
    return 66;
}