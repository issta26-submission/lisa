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
//<ID> 204
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = (png_structp)NULL;
    unsigned char buf[4];
    buf[0] = 0x7Fu;
    buf[1] = 0x01u;
    buf[2] = 0x02u;
    buf[3] = 0x03u;
    png_uint_32 u31 = png_get_uint_31((png_const_structrp)png_ptr, (png_const_bytep)buf);

    // step 2: Configure
    int opt_ret = png_set_option((png_structrp)png_ptr, 0x1E, 1);
    png_const_charp ver = png_get_header_version((png_const_structrp)png_ptr);

    // step 3: Operate / Validate
    unsigned char work[16];
    memset(work, 0x5Au, sizeof(work));
    work[0] ^= (unsigned char)(u31 & 0xFFu);
    work[1] ^= (unsigned char)(opt_ret & 0xFFu);
    work[2] ^= (unsigned char)(((unsigned long)ver) & 0xFFu);
    png_charp purpose = (png_charp)NULL;
    png_int_32 X0 = (png_int_32)u31;
    png_int_32 X1 = (png_int_32)(u31 >> 1);
    int type = 0;
    int nparams = 0;
    png_charp units = (png_charp)NULL;
    png_charpp params = (png_charpp)NULL;
    png_uint_32 pcal = png_get_pCAL((png_const_structrp)png_ptr, (png_inforp)NULL, &purpose, &X0, &X1, &type, &nparams, &units, &params);
    work[3] ^= (unsigned char)(pcal & 0xFFu);

    // step 4: Cleanup
    memset(work, 0, sizeof(work));
    // API sequence test completed successfully
    return 66;
}