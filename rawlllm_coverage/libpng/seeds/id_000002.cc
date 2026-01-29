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
//<ID> 26
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // Prepare a small buffer to read a 31-bit unsigned integer from
    png_byte buf[4] = { 0x7F, 0xFF, 0xFF, 0xFF }; // sample big-endian bytes

    // Create a write struct. Use null error/warning handlers and error_ptr.
    png_structp png_ptr = png_create_write_struct("1.6.37", nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        return 1; // failure to create
    }

    // Query the libpng version from the created struct (const char* result).
    png_const_charp ver = png_get_libpng_ver((png_const_structrp)png_ptr);
    (void)ver; // silence unused-variable warnings

    // Read a 31-bit unsigned integer from the buffer using the png struct.
    png_uint_32 value = png_get_uint_31((png_const_structrp)png_ptr, (png_const_bytep)buf);
    (void)value; // we don't need to do anything further with it here

    // Prepare a small EXIF blob and set it on the png struct/info (info_ptr passed as nullptr).
    png_bytep exif = new png_byte[8];
    for (int i = 0; i < 8; ++i) {
        exif[i] = (png_byte)(i + 1);
    }
    // Set the EXIF data. Pass nullptr for the info pointer (allowed in this test sequence).
    png_set_eXIf((png_const_structrp)png_ptr, (png_inforp)nullptr, exif);

    // Clean up allocated EXIF buffer.
    delete [] exif;

    // Destroy the write struct. We pass the address of png_ptr; info pointer argument is NULL.
    png_destroy_write_struct(&png_ptr, nullptr);

    return 66;
}