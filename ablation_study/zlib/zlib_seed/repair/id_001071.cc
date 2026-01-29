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
//<ID> 1071
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize / Configure
    const Bytef source[] = "zlib API sequence test payload for deflate/flush/pending and gzoffset64.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong bound = deflateBound(&strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)source_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;

    // step 2: Operate (compress, inspect pending, reset)
    int def_ret = deflate(&strm, 4);
    uLong comp_size = strm.total_out;
    unsigned int pending = 0;
    int bits = 0;
    int pending_ret = deflatePending(&strm, &pending, &bits);
    int reset_ret = deflateReset(&strm);

    // step 3: Persist compressed data to .gz and query 64-bit offset
    gzFile gzf = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_size);
    off64_t file_offset64 = gzoffset64(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate / Cleanup
    uLong computed_crc = crc32(0UL, source, (uInt)source_len);
    int end_ret = deflateEnd(&strm);
    free(comp_buf);
    (void)init_ret;
    (void)bound;
    (void)def_ret;
    (void)pending_ret;
    (void)bits;
    (void)pending;
    (void)reset_ret;
    (void)gzwrite_ret;
    (void)file_offset64;
    (void)gzclose_ret;
    (void)computed_crc;
    (void)end_ret;
    (void)source_len;
    // API sequence test completed successfully
    return 66;
}