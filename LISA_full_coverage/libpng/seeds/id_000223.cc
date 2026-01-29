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
//<ID> 223
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Query library version (safe, doesn't require an initialized png_struct)
    png_uint_32 ver = png_access_version_number();
    // step 2: Get header version string (safe to pass NULL; function returns static version string)
    png_const_charp verstr = png_get_header_version((png_const_structrp)NULL);
    // silence unused variable warnings
    (void)ver;
    (void)verstr;
    // API sequence test completed successfully
    return 66;
}