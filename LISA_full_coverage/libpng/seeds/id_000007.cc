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
//<ID> 7
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations
    png_structrp png_ptr = nullptr;
    png_inforp info_ptr = nullptr;
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = png_access_version_number();
    image.width = 16;
    image.height = 16;
    image.format = 0;
    image.flags = 0;
    image.colormap_entries = 0;
    // sPLT setup
    png_sPLT_t splt_entry;
    memset(&splt_entry, 0, sizeof(splt_entry));
    splt_entry.name = (png_charp)"example-sPLT";
    splt_entry.depth = 8;
    splt_entry.entries = nullptr;
    splt_entry.nentries = 0;
    png_const_sPLT_tp splt_ptr = &splt_entry;
    // buffers
    png_alloc_size_t mem_bytes = 0;
    void * out_memory = nullptr;
    size_t row_stride = (size_t)image.width * 4;
    png_int_32 row_stride_i = (png_int_32)row_stride;
    size_t pixel_buffer_size = (size_t)image.width * (size_t)image.height * 4;
    unsigned char * pixel_buffer = (unsigned char*)malloc(pixel_buffer_size);
    memset(pixel_buffer, 0xFF, pixel_buffer_size);
    // step 2: Setup
    png_set_read_status_fn(png_ptr, (png_read_status_ptr)nullptr);
    png_set_filler(png_ptr, (png_uint_32)0xFFFFFFFFU, 0);
    png_set_sPLT(png_ptr, info_ptr, splt_ptr, 1);
    // step 3: Operate and Validate
    png_image_write_to_memory(&image, &out_memory, &mem_bytes, 1, pixel_buffer, row_stride_i, nullptr);
    // step 4: Cleanup
    free(pixel_buffer);
    free(out_memory);
    // API sequence test completed successfully
    return 66;
}