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
//<ID> 386
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* Minimal write/flush callbacks to satisfy png_set_write_fn.
   Signatures use common libpng types: png_structp, png_bytep, size_t.
   These are simple no-ops that avoid the "NULL write function" error. */
static void test_write_fn(png_structp png_ptr, png_bytep data, size_t length)
{
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void test_flush_fn(png_structp png_ptr)
{
    (void)png_ptr;
}

int test_libpng_api_sequence() {
    /* step 1: Declarations / Initialize */
    png_structp png_ptr = png_create_write_struct_2(
        (png_const_charp)"1.6.37", (png_voidp)NULL,
        (png_error_ptr)NULL, (png_error_ptr)NULL,
        (png_voidp)NULL, (png_malloc_ptr)NULL, (png_free_ptr)NULL
    );
    png_info *info_ptr = png_create_info_struct((png_const_structrp)png_ptr);

    unsigned char intbuf[4] = { 0x00, 0x00, 0x00, 0x04 };
    png_bytep buf = (png_bytep)intbuf;

    /* step 2: Configure - set non-NULL write/flush callbacks */
    png_set_write_fn((png_structrp)png_ptr, (png_voidp)NULL, (png_rw_ptr)test_write_fn, (png_flush_ptr)test_flush_fn);

    /* step 3: Operate / Validate */
    png_int_32 length = png_get_int_32((png_const_bytep)buf);
    png_byte chunk_name[4] = { (png_byte)'t', (png_byte)'E', (png_byte)'S', (png_byte)'T' };
    unsigned char data[4] = { 0xAB, 0xAB, 0xAB, 0xAB };

    png_write_chunk((png_structrp)png_ptr, (png_const_bytep)chunk_name, (png_const_bytep)data, (size_t)length);
    png_write_chunk_end((png_structrp)png_ptr);

    /* step 4: Cleanup */
    png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    /* API sequence test completed successfully */
    return 66;
}