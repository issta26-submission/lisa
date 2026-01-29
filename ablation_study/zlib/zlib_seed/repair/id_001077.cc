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
//<ID> 1077
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize / Configure
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence test payload: compress then write to .gz and inspect pending/offset.";
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

    // step 2: Operate (reset, deflate, inspect pending bits)
    int reset_ret = deflateReset(&strm);
    int def_ret = deflate(&strm, 4); /* 4 == Z_FINISH */
    unsigned int pending = 0;
    int bits = 0;
    int pend_ret = deflatePending(&strm, &pending, &bits);
    uLong comp_size = strm.total_out;

    // step 3: Write compressed data to .gz file and get 64-bit offset
    gzFile gzf = gzopen64("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_size);
    off64_t offset64 = gzoffset64(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate / Cleanup
    int end_ret = deflateEnd(&strm);
    uLong checksum = crc32(0UL, comp_buf, (uInt)comp_size);
    free(comp_buf);
    (void)init_ret;
    (void)bound;
    (void)reset_ret;
    (void)def_ret;
    (void)pend_ret;
    (void)pending;
    (void)bits;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)end_ret;
    (void)checksum;
    (void)version;
    (void)source_len;
    (void)comp_size;
    (void)offset64;
    // API sequence test completed successfully
    return 66;
}