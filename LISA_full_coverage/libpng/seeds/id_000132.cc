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
//<ID> 132
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpng_api_sequence() {
    png_uint_32 ver = png_access_version_number();
    size_t chunk_len = (size_t)((ver & 0xFFu) + 16u);

    png_byte *chunk_name = (png_byte *)malloc(5);
    memset(chunk_name, 0, (size_t)5);

    png_byte *chunk_data = (png_byte *)malloc(chunk_len);
    memset(chunk_data, 0, chunk_len);

    chunk_name[0] = (png_byte)'T';
    chunk_name[1] = (png_byte)'E';
    chunk_name[2] = (png_byte)'S';
    chunk_name[3] = (png_byte)'T';

    chunk_data[0] = (png_byte)(ver & 0xFFu);

    png_byte buf[4];
    png_save_uint_32(buf, ver);

    png_uint_32 mix = ver ^ (png_uint_32)buf[0] ^ (png_uint_32)chunk_name[0];
    (void)mix;

    free(chunk_name);
    free(chunk_data);

    return 66;
}