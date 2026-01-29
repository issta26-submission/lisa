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
//<ID> 209
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    unsigned char header_buf[8];
    memset(header_buf, 0, sizeof(header_buf));
    png_const_charp header_ver = png_get_header_version((png_const_structrp)png_ptr);
    header_buf[0] = (unsigned char)header_ver[0];

    // step 2: Configure
    int opt_initial = png_set_option((png_structrp)png_ptr, 3, 1);
    int opt_secondary = png_set_option((png_structrp)png_ptr, 7, 0);

    // step 3: Operate / Validate
    unsigned char raw31[4];
    raw31[0] = 0x00u;
    raw31[1] = 0x01u;
    raw31[2] = 0x02u;
    raw31[3] = 0x03u;
    png_uint_32 got31 = png_get_uint_31((png_const_structrp)png_ptr, (png_const_bytep)raw31);
    png_charp purpose = (png_charp)NULL;
    png_int_32 X0 = 0;
    png_int_32 X1 = 0;
    int ptype = 0;
    int pnparams = 0;
    png_charp units = (png_charp)NULL;
    png_charpp params = (png_charpp)NULL;
    png_uint_32 pcal_ret = png_get_pCAL((png_const_structrp)png_ptr, (png_inforp)NULL, &purpose, &X0, &X1, &ptype, &pnparams, &units, &params);
    unsigned char mix_buf[12];
    memset(mix_buf, 0xA5, sizeof(mix_buf));
    mix_buf[0] = (unsigned char)((got31 & 0xFFu) ^ (png_uint_32)(opt_initial & 0xFF) ^ (png_uint_32)(pcal_ret & 0xFFu));

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, (png_voidp)purpose);
    png_free_default((png_const_structrp)png_ptr, (png_voidp)units);
    png_free_default((png_const_structrp)png_ptr, (png_voidp)params);
    (void)opt_secondary;
    (void)mix_buf;
    // API sequence test completed successfully
    return 66;
}