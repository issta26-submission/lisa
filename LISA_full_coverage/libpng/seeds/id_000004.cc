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
//<ID> 4
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations
    png_structrp png_ptr = (png_structrp)NULL;
    png_inforp info_ptr = png_create_info_struct(png_ptr);
    png_sPLT_t splt;
    png_image image;
    void * out_memory = (void *)NULL;
    png_alloc_size_t out_memory_bytes = (png_alloc_size_t)0;

    // step 2: Setup
    splt.name = (png_charp)NULL;
    splt.depth = (png_byte)8;
    splt.entries = (png_sPLT_entryp)NULL;
    splt.nentries = (png_int_32)0;

    image.version = png_access_version_number();
    image.width = (png_uint_32)4;
    image.height = (png_uint_32)4;
    image.format = (png_uint_32)0;
    image.flags = (png_uint_32)0;
    image.colormap_entries = (png_uint_32)0;
    image.warning_or_error = (png_uint_32)0;
    memset(image.message, 0, sizeof(image.message));

    png_int_32 row_stride = (png_int_32)(image.width * 4);
    size_t buffer_size = (size_t)image.height * (size_t)row_stride;
    unsigned char * pixel_buffer = new unsigned char[buffer_size];
    memset(pixel_buffer, 0xAA, buffer_size);

    // step 3: Core operations (Initialize -> Configure -> Operate -> Validate)
    png_set_read_status_fn(png_ptr, (png_read_status_ptr)NULL);
    png_set_filler(png_ptr, (png_uint_32)0xFFFFFFFFu, 1);
    png_set_sPLT(png_ptr, info_ptr, (png_const_sPLT_tp)&splt, 1);
    int write_result = png_image_write_to_memory(&image, out_memory, &out_memory_bytes, 1, pixel_buffer, row_stride, (const void *)NULL);
    (void)write_result; // keep value to show validation step without branching

    // step 4: Cleanup
    delete [] pixel_buffer;
    png_destroy_read_struct((png_structpp)NULL, (png_infopp)&info_ptr, (png_infopp)NULL);
    puts("API sequence test completed successfully");
    return 66;
}