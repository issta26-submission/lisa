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
//<ID> 1075
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize / Configure
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence test payload: compress, write gz, inspect pending and offsets.";
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

    // step 2: Operate (compress and inspect pending)
    int def_ret = deflate(&strm, 4);
    unsigned int pending = 0;
    int bits = 0;
    int pend_ret = deflatePending(&strm, &pending, &bits);
    uLong comp_size = strm.total_out;
    int reset_ret = deflateReset(&strm);

    // step 3: Write compressed data to gz and inspect offset
    gzFile gzf = gzopen64("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_size);
    off64_t write_offset = gzoffset64(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate / Cleanup
    uLong crc_src = crc32(0UL, source, (uInt)source_len);
    uLong crc_comp = crc32(0UL, comp_buf, (uInt)comp_size);
    uLong combined_crc = crc32_combine64(crc_src, crc_comp, (off64_t)comp_size);
    int end_ret = deflateEnd(&strm);
    free(comp_buf);
    (void)init_ret;
    (void)bound;
    (void)def_ret;
    (void)pend_ret;
    (void)pending;
    (void)bits;
    (void)reset_ret;
    (void)gzwrite_ret;
    (void)write_offset;
    (void)gzclose_ret;
    (void)crc_src;
    (void)crc_comp;
    (void)combined_crc;
    (void)end_ret;
    (void)version;
    (void)source_len;
    (void)comp_size;
    // API sequence test completed successfully
    return 66;
}