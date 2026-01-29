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
//<ID> 199
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    unsigned char buf[8];
    buf[0] = 0x89u;
    buf[1] = 'P';
    buf[2] = 'N';
    buf[3] = 'G';

    png_uint_32 ver = png_access_version_number();

    buf[4] = (unsigned char)((ver >> 24) & 0xFFu);
    buf[5] = (unsigned char)((ver >> 16) & 0xFFu);
    buf[6] = (unsigned char)((ver >> 8) & 0xFFu);
    buf[7] = (unsigned char)(ver & 0xFFu);

    png_int_32 v32 = png_get_int_32((png_const_bytep)(buf + 4));
    png_uint_16 v16 = png_get_uint_16((png_const_bytep)(buf + 6));
    int sigcmp = png_sig_cmp((png_const_bytep)buf, (size_t)0, (size_t)4);

    size_t mix = ((size_t)ver) ^ ((size_t)v32) ^ ((size_t)v16) ^ ((size_t)sigcmp);
    buf[0] ^= (unsigned char)(mix & 0xFFu);

    (void)v32;
    (void)v16;
    (void)sigcmp;
    (void)mix;

    return 66;
}