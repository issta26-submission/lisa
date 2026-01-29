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
//<ID> 246
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = (png_inforp)NULL;
    png_time ptime;
    time_t tstamp = (time_t)1625072400;

    // step 2: Configure
    png_set_tRNS_to_alpha((png_structrp)png_ptr);
    png_set_unknown_chunk_location((png_const_structrp)png_ptr, (png_inforp)info_ptr, 3, 1);

    // step 3: Operate / Validate
    png_convert_from_time_t((png_timep)&ptime, tstamp);
    png_start_read_image((png_structrp)png_ptr);
    unsigned char buf[4];
    png_uint_32 packed = ((png_uint_32)ptime.year << 16) | ((png_uint_32)ptime.month << 8) | (png_uint_32)ptime.day;
    png_save_uint_32((png_bytep)buf, packed);

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}