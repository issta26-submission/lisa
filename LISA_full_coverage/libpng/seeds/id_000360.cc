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
//<ID> 360
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_const_charp header_ver = png_get_header_version((png_const_structrp)png_ptr);
    png_alloc_size_t buf_size = (png_alloc_size_t)64;
    png_voidp buf = png_malloc_default((png_const_structrp)png_ptr, buf_size);
    memset(buf, 0xA5, (size_t)buf_size);
    ((unsigned char*)buf)[0] = (unsigned char)header_ver[0];

    // step 2: Configure library behavior
    png_set_text_compression_level((png_structrp)png_ptr, 6);

    // step 3: Operate - prepare unknown chunk data and register it with the info struct pointer (NULL used for demo)
    png_unknown_chunk unknowns[1];
    memset(unknowns, 0, sizeof(unknowns));
    unknowns[0].name[0] = (png_byte)'u';
    unknowns[0].name[1] = (png_byte)'n';
    unknowns[0].name[2] = (png_byte)'k';
    unknowns[0].name[3] = (png_byte)'1';
    unknowns[0].name[4] = (png_byte)0;
    unknowns[0].size = (size_t)16;
    unknowns[0].data = (png_byte*)png_malloc_default((png_const_structrp)png_ptr, (png_alloc_size_t)unknowns[0].size);
    memset(unknowns[0].data, 0x5A, (size_t)unknowns[0].size);
    unknowns[0].location = (png_byte)0;
    png_set_unknown_chunks((png_const_structrp)png_ptr, (png_inforp)NULL, (png_const_unknown_chunkp)unknowns, 1);

    // step 4: Cleanup
    png_free((png_const_structrp)png_ptr, (png_voidp)unknowns[0].data);
    png_free((png_const_structrp)png_ptr, buf);

    // API sequence test completed successfully
    (void)header_ver;
    return 66;
}