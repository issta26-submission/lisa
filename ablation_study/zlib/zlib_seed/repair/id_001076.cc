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
//<ID> 1076
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize / Configure deflate stream and buffers
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence fuzz-driver payload: compress to buffer, write .gz, inspect offset and checksum.";
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
    int reset_ret = deflateReset(&strm);

    // step 2: Operate (perform deflate and inspect pending bits)
    int def_ret = deflate(&strm, 4); /* 4 == Z_FINISH */
    unsigned int pending_bits_count = 0U;
    int pending_bits_value = 0;
    int pending_ret = deflatePending(&strm, &pending_bits_count, &pending_bits_value);
    uLong comp_size = strm.total_out;
    int end_ret = deflateEnd(&strm);

    // step 3: Write compressed data to a .gz file and get 64-bit offset
    gzFile gzf = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gzf, (voidpc)comp_buf, (unsigned int)comp_size);
    off64_t out_offset64 = gzoffset64(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate / Cleanup
    uLong comp_crc = crc32(0UL, comp_buf, (uInt)comp_size);
    free(comp_buf);
    (void)version;
    (void)init_ret;
    (void)bound;
    (void)reset_ret;
    (void)def_ret;
    (void)pending_ret;
    (void)pending_bits_count;
    (void)pending_bits_value;
    (void)end_ret;
    (void)gzwrite_ret;
    (void)out_offset64;
    (void)gzclose_ret;
    (void)comp_crc;
    (void)source_len;
    // API sequence test completed successfully
    return 66;
}