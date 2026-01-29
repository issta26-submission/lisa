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
//<ID> 1
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations
    png_structrp png_ptr = (png_structrp)0; // no explicit create function available here; use NULL sentinel
    png_inforp info_ptr = png_create_info_struct(png_ptr);
    png_sPLT_t sPLT;
    memset(&sPLT, 0, sizeof(sPLT));
    sPLT.name = (png_charp)0;
    sPLT.depth = 0;
    sPLT.entries = (png_sPLT_entryp)0;
    sPLT.nentries = 0;

    // step 2: Configuration
    png_set_read_status_fn(png_ptr, (png_read_status_ptr)0);
    png_set_filler(png_ptr, (png_uint_32)0xFFu, 0);
    png_set_sPLT(png_ptr, info_ptr, (png_const_sPLT_tp)&sPLT, 0);

    // step 3: Operate / Validate
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = png_access_version_number();
    image.width = (png_uint_32)16;
    image.height = (png_uint_32)16;
    image.format = 0;
    image.flags = 0;
    image.colormap_entries = 0;
    image.warning_or_error = 0;
    png_int_32 row_stride = (png_int_32)(image.width * 4);

    png_alloc_size_t input_size = (png_alloc_size_t)(image.width * image.height * 4);
    void * input_buf = malloc(input_size);
    memset(input_buf, 0x7F, input_size);

    png_alloc_size_t out_size = input_size + 256;
    void * out_mem = malloc(out_size);
    png_alloc_size_t memory_bytes = out_size;

    int write_result = png_image_write_to_memory(&image, out_mem, &memory_bytes, 1, input_buf, row_stride, (const void *)0);

    // step 4: Cleanup
    free(input_buf);
    free(out_mem);
    png_structp png_ptr_local = (png_structp)0;
    png_infop info_ptr_local = info_ptr;
    png_destroy_read_struct(&png_ptr_local, &info_ptr_local, (png_infopp)0);

    // API sequence test completed successfully
    (void)write_result;
    return 66;
}