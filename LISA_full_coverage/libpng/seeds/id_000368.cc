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
//<ID> 368
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structp png_ptr = png_create_write_struct((png_const_charp)"1.6.37", (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
    png_inforp info_ptr = (png_inforp)png_malloc((png_const_structrp)png_ptr, (png_alloc_size_t)256);
    png_info_init_3((png_infopp)&info_ptr, (size_t)0);

    // step 2: Configure
    png_const_charp header_ver = png_get_libpng_ver((png_const_structrp)png_ptr);
    png_alloc_size_t chunk_data_size = (png_alloc_size_t)64;
    png_voidp chunk_data = png_malloc((png_const_structrp)png_ptr, chunk_data_size);
    memset(chunk_data, 0x5A, (size_t)chunk_data_size);
    png_unknown_chunk unknown_local;
    memset((void *)&unknown_local, 0, sizeof(unknown_local));
    unknown_local.name[0] = (png_byte)'u';
    unknown_local.name[1] = (png_byte)'n';
    unknown_local.name[2] = (png_byte)'k';
    unknown_local.name[3] = (png_byte)'\0';
    unknown_local.name[4] = (png_byte)'\0';
    unknown_local.data = (png_byte *)chunk_data;
    unknown_local.size = (size_t)chunk_data_size;
    unknown_local.location = (png_byte)1;
    png_set_unknown_chunks((png_const_structrp)png_ptr, info_ptr, (png_const_unknown_chunkp)&unknown_local, 1);
    png_set_text_compression_level((png_structrp)png_ptr, 6);

    // step 3: Operate / Validate
    ((unsigned char *)chunk_data)[0] = (unsigned char)header_ver[0];
    png_set_text_compression_level((png_structrp)png_ptr, 7);

    // step 4: Cleanup
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}