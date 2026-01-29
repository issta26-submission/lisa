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
//<ID> 366
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Initialize
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_const_charp header_ver = png_get_header_version((png_const_structrp)png_ptr);
    int num_unknowns = 1;

    // step 2: Configure allocations and unknown chunk
    png_unknown_chunk * unknowns = (png_unknown_chunk *)png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)(sizeof(png_unknown_chunk) * (size_t)num_unknowns));
    memset((void *)unknowns, 0, (size_t)(sizeof(png_unknown_chunk) * (size_t)num_unknowns));
    unknowns[0].name[0] = (png_byte)'t';
    unknowns[0].name[1] = (png_byte)'E';
    unknowns[0].name[2] = (png_byte)'S';
    unknowns[0].name[3] = (png_byte)'T';
    unknowns[0].name[4] = (png_byte)0;
    unknowns[0].size = (size_t)8;
    unknowns[0].data = (png_byte *)png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)unknowns[0].size);
    memset((void *)unknowns[0].data, (int)'A', unknowns[0].size);
    unknowns[0].location = (png_byte)0;

    // step 3: Operate / Configure libpng with unknown chunks and text compression
    png_set_unknown_chunks((png_const_structrp)png_ptr, (png_inforp)NULL, (png_const_unknown_chunkp)unknowns, num_unknowns);
    png_set_text_compression_level((png_structrp)png_ptr, 9);
    png_const_charp verify_ver = png_get_header_version((png_const_structrp)png_ptr);

    // step 4: Validate and Cleanup
    unknowns[0].data[0] = (png_byte)verify_ver[0];
    png_free((png_const_structrp)png_ptr, (png_voidp)unknowns[0].data);
    png_free((png_const_structrp)png_ptr, (png_voidp)unknowns);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)NULL, (png_infopp)NULL);

    // API sequence test completed successfully
    (void)header_ver;
    (void)verify_ver;
    return 66;
}