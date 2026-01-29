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
//<ID> 363
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpng_api_sequence() {
    // step 1: Declarations / Initialize
    png_structrp png_ptr = (png_structrp)NULL;
    png_inforp info_ptr = (png_inforp)NULL;
    png_alloc_size_t chunk_size = (png_alloc_size_t)256;
    png_voidp chunk_data = malloc((size_t)chunk_size);
    memset(chunk_data, 0xA5, (size_t)chunk_size);

    // step 2: Configure
    png_set_text_compression_strategy(png_ptr, 9);
    png_set_text_compression_window_bits(png_ptr, 15);
    png_set_text_compression_method(png_ptr, 8);

    // step 3: Operate / Validate
    png_const_charp ver = png_get_libpng_ver(png_ptr);
    ((char *)chunk_data)[0] = ver[0];
    (void)ver;

    // step 4: Cleanup
    png_free_default(png_ptr, chunk_data);
    free(info_ptr);
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // API sequence test completed successfully
    return 66;
}