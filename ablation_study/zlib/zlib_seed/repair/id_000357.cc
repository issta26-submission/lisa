#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 357
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const Bytef write_payload[] = "Zlib API sequence payload for gz file write/read, using gzflush, gzoffset64, gzfread and gzgetc_.";
    const uLong write_payload_len = (uLong)(sizeof(write_payload) - 1);
    Bytef read_buf[512];
    memset((void *)read_buf, 0, sizeof(read_buf));
    gzFile gzw;
    gzFile gzr;
    int ret_gzwrite;
    int ret_gzflush;
    int ret_gzclose_w;
    int ret_gzclose_r;
    off64_t offset_after_write;
    off64_t offset_after_read;
    z_size_t items_read;
    int ch_after_read;

    // step 2: Setup - open gz file for writing and prepare data
    gzw = gzopen("test_zlib_api_sequence_fuzz.gz", "wb+");

    // step 3: Operate - write, flush, get offset and close writer
    ret_gzwrite = gzwrite(gzw, (voidpc)write_payload, (unsigned int)write_payload_len);
    ret_gzflush = gzflush(gzw, 0);
    offset_after_write = gzoffset64(gzw);
    ret_gzclose_w = gzclose(gzw);

    // step 4: Validate / Cleanup - open for read, read via gzfread, obtain offset and a subsequent char, then close
    gzr = gzopen("test_zlib_api_sequence_fuzz.gz", "rb");
    items_read = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)sizeof(read_buf), gzr);
    offset_after_read = gzoffset64(gzr);
    ch_after_read = gzgetc_(gzr);
    ret_gzclose_r = gzclose(gzr);

    (void)ret_gzwrite;
    (void)ret_gzflush;
    (void)offset_after_write;
    (void)items_read;
    (void)offset_after_read;
    (void)ch_after_read;
    (void)ret_gzclose_w;
    (void)ret_gzclose_r;
    // API sequence test completed successfully
    return 66;
}