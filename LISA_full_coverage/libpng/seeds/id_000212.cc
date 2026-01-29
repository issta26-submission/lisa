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
//<ID> 212
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_color_16 bg;
    bg.index = 0;
    bg.red = (png_uint_16)0x00FFu;
    bg.green = (png_uint_16)0x0080u;
    bg.blue = (png_uint_16)0x0000u;
    bg.gray = (png_uint_16)0;
    char * units = new char[7];
    units[0] = 'p'; units[1] = 'i'; units[2] = 'x'; units[3] = 'e'; units[4] = 'l'; units[5] = 's'; units[6] = '\0';
    char * p0 = new char[7];
    p0[0] = 'v'; p0[1] = 'a'; p0[2] = 'l'; p0[3] = 'u'; p0[4] = 'e'; p0[5] = '1'; p0[6] = '\0';
    png_charpp params = new char*[2];
    params[0] = p0;
    params[1] = (char*)NULL;

    // step 2: Configure
    png_set_pCAL((png_const_structrp)png_ptr, (png_inforp)NULL, (png_const_charp)"pCAL-purpose", (png_int_32)0, (png_int_32)100, 1, 1, (png_const_charp)units, params);
    png_set_bKGD((png_const_structrp)png_ptr, (png_inforp)NULL, (png_const_color_16p)&bg);
    png_set_alpha_mode((png_structrp)png_ptr, 1, 2.2);

    // step 3: Operate / Validate
    png_const_charp cp = png_get_copyright((png_const_structrp)png_ptr);
    unsigned char buf[8];
    memset(buf, 0, sizeof(buf));
    buf[0] = (unsigned char)(cp != NULL ? cp[0] : 0);
    int checksum = ((int)bg.red) ^ ((int)bg.green) ^ ((int)bg.blue) ^ (int)buf[0];
    buf[1] = (unsigned char)(checksum & 0xFFu);
    (void)checksum;

    // step 4: Cleanup
    png_free_default((png_const_structrp)png_ptr, (png_voidp)units);
    png_free_default((png_const_structrp)png_ptr, (png_voidp)p0);
    png_free_default((png_const_structrp)png_ptr, (png_voidp)params);
    // API sequence test completed successfully
    return 66;
}