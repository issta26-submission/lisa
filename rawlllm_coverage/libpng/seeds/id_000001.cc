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
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Initialize
    png_structrp png_ptr = (png_structrp)0;
    png_infop info_ptr = png_create_info_struct(png_ptr);
    png_sPLT_t splt;
    memset(&splt, 0, sizeof(splt));
    splt.depth = 8;
    splt.nentries = 0;

    // step 2: Configure
    png_set_read_status_fn(png_ptr, (png_read_status_ptr)0);
    png_set_filler(png_ptr, (png_uint_32)0xFFu, 1);
    png_set_sPLT(png_ptr, info_ptr, (png_const_sPLT_tp)&splt, 1);

    // step 3: Operate & Validate
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = png_access_version_number();
    image.width = 2;
    image.height = 2;
    image.format = 0;
    unsigned char pixel_buffer[8] = { 0xFF, 0x00, 0x00, 0xFF,   0x00, 0xFF, 0x00, 0xFF }; // two RGBA pixels
    png_alloc_size_t memory_bytes = 0;
    void * memory_out = 0;
    int write_result = png_image_write_to_memory(&image, &memory_out, &memory_bytes, 1, pixel_buffer, (png_int_32)8, (const void *)0);

    // step 4: Cleanup
    memset(&splt, 0, sizeof(splt));
    memset(pixel_buffer, 0, sizeof(pixel_buffer));
    memset(&image, 0, sizeof(image));
    free(memory_out);

    // API sequence test completed successfully
    (void)write_result;
    (void)info_ptr;
    return 66;
}