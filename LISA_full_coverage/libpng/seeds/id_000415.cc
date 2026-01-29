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
//<ID> 415
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structrp png_ptr = (png_structrp)NULL;
    png_info *info_ptr = (png_info *)NULL;

    // step 2: Configure
    // Query the compiled libpng version number (no png_ptr required)
    png_uint_32 version = png_access_version_number();

    // step 3: Operate / Validate
    // Save the version and an example signed integer into a small buffer
    png_byte buffer[8];
    png_save_uint_32((png_bytep)buffer, version);
    png_save_int_32((png_bytep)(buffer + 4), (png_int_32)12345);

    // step 4: Cleanup
    // Nothing allocated that requires libpng cleanup in this sequence.
    (void)png_ptr;
    (void)info_ptr;
    (void)buffer;
    (void)version;
    return 66;
}