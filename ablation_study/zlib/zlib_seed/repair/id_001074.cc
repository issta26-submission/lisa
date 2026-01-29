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
//<ID> 1074
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize / Prepare source and deflate stream
    const Bytef source[] = "zlib API sequence test payload for deflate and gz offset inspection.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *version = zlibVersion();
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)source_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    unsigned int pending_before;
    int bits_before;
    int pend_ret_before = deflatePending(&strm, &pending_before, &bits_before);

    // step 2: Operate (perform compression)
    int def_ret = deflate(&strm, 4);
    uLong comp_size = strm.total_out;
    unsigned int pending_after;
    int bits_after;
    int pend_ret_after = deflatePending(&strm, &pending_after, &bits_after);

    // step 3: Reset stream and write compressed buffer into a .gz file, inspect offset with gzoffset64
    int reset_ret = deflateReset(&strm);
    gzFile gzf = gzopen64("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gzf, (voidpc)comp_buf, (unsigned int)comp_size);
    off64_t write_offset = gzoffset64(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Cleanup and lightweight validation
    int end_ret = deflateEnd(&strm);
    uLong checksum = crc32(0UL, comp_buf, (uInt)comp_size);
    free(comp_buf);
    (void)init_ret;
    (void)bound;
    (void)pend_ret_before;
    (void)bits_before;
    (void)pending_before;
    (void)def_ret;
    (void)pending_after;
    (void)bits_after;
    (void)pend_ret_after;
    (void)reset_ret;
    (void)gzwrite_ret;
    (void)write_offset;
    (void)gzclose_ret;
    (void)end_ret;
    (void)checksum;
    (void)version;
    (void)source_len;
    (void)comp_size;
    // API sequence test completed successfully
    return 66;
}