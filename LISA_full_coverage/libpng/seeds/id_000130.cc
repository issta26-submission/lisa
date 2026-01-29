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
//<ID> 130
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

static void user_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void user_flush(png_structp png_ptr) {
    (void)png_ptr;
}

int test_libpng_api_sequence() {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);

    png_set_compression_level(png_ptr, 6);
    png_set_write_fn(png_ptr, NULL, user_write_data, user_flush);

    png_uint_32 initial_row = (png_uint_32)png_access_version_number();

    png_byte chunk_name[5];
    memset(chunk_name, 0, (size_t)5);
    chunk_name[0] = (png_byte)'t';
    chunk_name[1] = (png_byte)'E';
    chunk_name[2] = (png_byte)'S';
    chunk_name[3] = (png_byte)'T';

    png_uint_32 length = (png_uint_32)(initial_row + 8u);
    png_bytep buffer = (png_bytep)malloc((size_t)length);
    memset(buffer, 0xAB, (size_t)length);

    png_write_chunk(png_ptr, (png_const_bytep)chunk_name, (png_const_bytep)buffer, (size_t)length);

    png_uint_32 post_row = (png_uint_32)png_access_version_number();
    png_uint_32 mix = length ^ initial_row ^ post_row;
    (void)mix;

    free(buffer);

    (void)info_ptr;
    (void)png_ptr;

    return 66;
}