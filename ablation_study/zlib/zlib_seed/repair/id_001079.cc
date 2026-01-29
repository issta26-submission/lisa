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
//<ID> 1079
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize / Configure
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence test payload for deflate, pending, reset, gzip offset inspection.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)source_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;

    // step 2: Operate (compress and inspect pending), then reset stream
    unsigned int pending_before = 0;
    int bits_before = 0;
    int pend_ret1 = deflatePending(&strm, &pending_before, &bits_before);
    int def_ret = deflate(&strm, 4);
    unsigned int pending_after = 0;
    int bits_after = 0;
    int pend_ret2 = deflatePending(&strm, &pending_after, &bits_after);
    uLong comp_size = strm.total_out;
    int reset_ret = deflateReset(&strm);

    // step 3: Write compressed bytes to a .gz file and get 64-bit offset
    gzFile gzf = gzopen64("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_size);
    off64_t offset64 = gzoffset64(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate / Cleanup
    uLong checksum = crc32(0UL, comp_buf, (uInt)comp_size);
    int end_ret = deflateEnd(&strm);
    free(comp_buf);
    (void)init_ret;
    (void)bound;
    (void)pend_ret1;
    (void)bits_before;
    (void)def_ret;
    (void)pend_ret2;
    (void)bits_after;
    (void)comp_size;
    (void)reset_ret;
    (void)gzwrite_ret;
    (void)offset64;
    (void)gzclose_ret;
    (void)checksum;
    (void)end_ret;
    (void)version;
    (void)source_len;
    // API sequence test completed successfully
    return 66;
}