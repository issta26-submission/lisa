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
//<ID> 284
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* Provide write and flush callback types (matching libpng's expected signatures). */
typedef void (*png_rw_ptr_local)(png_structp, png_const_bytep, size_t);
typedef void (*png_flush_ptr_local)(png_structp);

/* Simple no-op write callback to satisfy libpng's requirement for a write function. */
static void noop_write_data(png_structp png_ptr, png_const_bytep data, size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

/* Simple no-op flush callback. */
static void noop_flush(png_structp png_ptr) {
    (void)png_ptr;
}

int test_libpng_api_sequence() {
    /* step 1: Declarations / Initialize */
    void *user_mem = malloc(128);
    memset(user_mem, 0x5A, 128);
    png_structp png_ptr = png_create_read_struct((png_const_charp)"1.6.37", (png_voidp)user_mem, (png_error_ptr)NULL, (png_error_ptr)NULL);

    /* step 2: Configure - set a write callback so subsequent write operations will not fail */
    png_set_write_fn((png_structrp)png_ptr, (png_voidp)user_mem,
                     (png_rw_ptr)noop_write_data, (png_flush_ptr)noop_flush);

    /* step 3: Operate / Validate */
    const char chunk_name[5] = "tEXt";
    unsigned char *mem_bytes = (unsigned char *)user_mem;
    /* Use a read/info getter that returns png_uint_32 (similar role to the original ppm usage) */
    png_uint_32 val = png_get_image_width((png_const_structrp)png_ptr, (png_const_inforp)NULL);
    mem_bytes[0] = (unsigned char)(val & 0xFF);
    mem_bytes[1] = (unsigned char)((val >> 8) & 0xFF);
    mem_bytes[2] = (unsigned char)((val >> 16) & 0xFF);
    mem_bytes[3] = (unsigned char)((val >> 24) & 0xFF);

    /* Write a chunk using the no-op write function to avoid the previous NULL write function error */
    png_write_chunk((png_structrp)png_ptr, (png_const_bytep)chunk_name, (png_const_bytep)mem_bytes, (size_t)12);

    /* step 4: Cleanup */
    free(user_mem);
    png_destroy_write_struct((png_structpp)&png_ptr, (png_infopp)NULL);

    /* API sequence test completed successfully */
    return 66;
}