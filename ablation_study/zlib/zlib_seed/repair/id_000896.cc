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
//<ID> 896
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence test payload for deflate, copy, and gzip offset.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc = crc_tbl[0];
    z_stream s_orig;
    z_stream s_copy;
    memset(&s_orig, 0, sizeof(s_orig));
    memset(&s_copy, 0, sizeof(s_copy));
    int init_ret = deflateInit_(&s_orig, 6, version, (int)sizeof(z_stream));
    s_orig.adler = (uLong)first_crc;

    // step 2: Configure
    s_orig.next_in = (Bytef *)source;
    s_orig.avail_in = (uInt)src_len;
    uLong out_bound = deflateBound(&s_orig, src_len);
    Bytef *outbuf = (Bytef *)malloc((size_t)out_bound);
    memset(outbuf, 0, (size_t)out_bound);
    s_orig.next_out = outbuf;
    s_orig.avail_out = (uInt)out_bound;

    // step 3: Operate
    int def_once = deflate(&s_orig, 4);
    uLong produced = s_orig.total_out;
    int copy_ret = deflateCopy(&s_copy, &s_orig);
    s_copy.next_out = outbuf + (size_t)produced;
    s_copy.avail_out = (uInt)(out_bound - produced);
    int def_copy_once = deflate(&s_copy, 4);
    uLong produced_copy = s_copy.total_out;

    // step 4: Validate / Cleanup
    gzFile gzf = gzopen64("zlib_api_sequence_tmp.gz", "wb");
    int gz_put_ret = gzputs(gzf, "zlib header line\n");
    off64_t g_offset = gzoffset64(gzf);
    int gz_close_ret = gzclose(gzf);
    int end_copy = deflateEnd(&s_copy);
    int end_orig = deflateEnd(&s_orig);
    free(outbuf);
    (void)version;
    (void)init_ret;
    (void)first_crc;
    (void)out_bound;
    (void)def_once;
    (void)produced;
    (void)copy_ret;
    (void)def_copy_once;
    (void)produced_copy;
    (void)gz_put_ret;
    (void)g_offset;
    (void)gz_close_ret;
    (void)end_copy;
    (void)end_orig;
    // API sequence test completed successfully
    return 66;
}