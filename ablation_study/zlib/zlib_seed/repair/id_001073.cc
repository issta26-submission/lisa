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
//<ID> 1073
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize / configure deflate stream and buffers
    const char *version = zlibVersion();
    const Bytef source[] = "zlib api sequence: compress then write to .gz and inspect offsets and pending bits";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)source_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    int reset_ret = deflateReset(&strm);

    // step 2: Operate - perform compression and inspect pending output
    int def_ret = deflate(&strm, 4);
    unsigned int pending_bytes = 0;
    int pending_bits = 0;
    int pend_ret = deflatePending(&strm, &pending_bytes, &pending_bits);
    uLong comp_size = strm.total_out;
    int end_ret = deflateEnd(&strm);

    // step 3: Write compressed data to a .gz file and inspect 64-bit offset
    gzFile gzf = gzopen64("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gzf, (voidpc)comp_buf, (unsigned int)comp_size);
    off64_t file_offset = gzoffset64(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate (simple checksum) and cleanup
    uLong src_crc = crc32(0UL, source, (uInt)source_len);
    free(comp_buf);
    (void)version;
    (void)init_ret;
    (void)bound;
    (void)reset_ret;
    (void)def_ret;
    (void)pend_ret;
    (void)pending_bytes;
    (void)pending_bits;
    (void)end_ret;
    (void)gzwrite_ret;
    (void)file_offset;
    (void)gzclose_ret;
    (void)src_crc;
    // API sequence test completed successfully
    return 66;
}