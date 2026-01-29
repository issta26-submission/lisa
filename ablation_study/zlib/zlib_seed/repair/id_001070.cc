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
//<ID> 1070
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize / Configure
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence test payload for deflate, pending inspection, reset and gz offset.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);

    // step 2: Prepare stream and reset
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)source_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    int reset_ret = deflateReset(&strm);

    // step 3: Operate (deflate, inspect pending bits)
    int def_ret = deflate(&strm, 4);
    unsigned int pending_bytes = 0;
    int pending_bits = 0;
    int pending_ret = deflatePending(&strm, &pending_bytes, &pending_bits);
    uLong comp_size = strm.total_out;
    int end_ret = deflateEnd(&strm);

    // step 4: Write compressed data to .gz, inspect 64-bit offset, validate and cleanup
    gzFile gzf = gzopen64("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_size);
    off64_t offset64 = gzoffset64(gzf);
    int gzclose_ret = gzclose(gzf);
    uLong checksum = crc32(0UL, comp_buf, (uInt)comp_size);
    free(comp_buf);

    (void)version;
    (void)init_ret;
    (void)bound;
    (void)reset_ret;
    (void)def_ret;
    (void)pending_ret;
    (void)pending_bytes;
    (void)pending_bits;
    (void)comp_size;
    (void)end_ret;
    (void)gzwrite_ret;
    (void)offset64;
    (void)gzclose_ret;
    (void)checksum;
    // API sequence test completed successfully
    return 66;
}