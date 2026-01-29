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
//<ID> 285
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* Minimal write/flush stubs to satisfy png_set_write_fn.
   These are defined outside the test function so the function body
   remains a straight-line sequence of API calls as requested. */
static void write_stub(png_structp png_ptr, png_voidp data, png_uint_32 length)
{
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void flush_stub(png_structp png_ptr)
{
    (void)png_ptr;
}

int test_libpng_api_sequence() {
    /* step 1: Declarations / Initialize */
    void *user_err = malloc(128);
    memset(user_err, 0xAB, 128);
    /* Create a write struct (not a read struct) so write callbacks are allowed. */
    png_structp png_ptr = png_create_write_struct_2(
        (png_const_charp)"1.6.37",
        (png_voidp)user_err,
        (png_error_ptr)NULL,
        (png_error_ptr)NULL,
        (png_voidp)NULL,
        (png_malloc_ptr)NULL,
        (png_free_ptr)NULL
    );

    /* step 2: Configure */
    png_set_benign_errors((png_structrp)png_ptr, 1);
    /* Ensure a non-NULL write function is installed so subsequent write operations
       (eg. png_write_chunk_start) do not fail with "Call to NULL write function". */
    png_set_write_fn((png_structrp)png_ptr,
                     (png_voidp)user_err,
                     (png_rw_ptr)write_stub,
                     (png_flush_ptr)flush_stub);

    png_byte chunk_name[4] = { 't', 'E', 'S', 'T' };
    png_write_chunk_start((png_structrp)png_ptr, (png_const_bytep)chunk_name, (png_uint_32)4);

    /* step 3: Operate / Validate */
    png_const_inforp info_ptr = (png_const_inforp)NULL;
    png_uint_32 res_x = 0;
    png_uint_32 res_y = 0;
    int unit_type = 0;
    png_get_pHYs((png_const_structrp)png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    png_uint_32 pixels_per_meter = res_x;

    unsigned char *buf = (unsigned char *)user_err;
    buf[0] = (unsigned char)(pixels_per_meter & 0xFFu);
    buf[1] = (unsigned char)((pixels_per_meter >> 8) & 0xFFu);
    buf[2] = (unsigned char)((pixels_per_meter >> 16) & 0xFFu);
    buf[3] = (unsigned char)((pixels_per_meter >> 24) & 0xFFu);

    /* finish the chunk we started */
    png_write_chunk_end((png_structrp)png_ptr);

    /* step 4: Cleanup */
    free(user_err);

    /* API sequence test completed successfully */
    return 66;
}