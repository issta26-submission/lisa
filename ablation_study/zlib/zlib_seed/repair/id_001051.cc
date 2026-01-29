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
//<ID> 1051
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence test payload: compress, reset, recompress, write gz, read gz, validate.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    (void)init_ret;
    uLong bound = deflateBound(&strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);

    // step 2: Configure and operate deflate (do one deflate, reset, then deflate again)
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)source_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    int def_ret1 = deflate(&strm, 0);
    uLong comp_size1 = strm.total_out;
    int reset_ret = deflateReset(&strm);
    (void)def_ret1;
    (void)comp_size1;
    (void)reset_ret;

    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)source_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    int def_ret2 = deflate(&strm, 4);
    uLong comp_size2 = strm.total_out;
    int def_end_ret = deflateEnd(&strm);
    (void)def_ret2;
    (void)def_end_ret;

    // step 3: Write compressed data to a gz file and then read it back with gzfread, also inspect offsets
    gzFile gzw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gzw, comp_buf, (unsigned int)comp_size2);
    off_t write_off = gzoffset(gzw);
    int gzclose_ret_w = gzclose(gzw);

    gzFile gzr = gzopen("tmp_zlib_api_seq.gz", "rb");
    off_t read_off_before = gzoffset(gzr);
    z_size_t read_buf_len = (z_size_t)(source_len + 64);
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_len);
    memset(read_buf, 0, (size_t)read_buf_len);
    z_size_t read_items = gzfread(read_buf, 1, read_buf_len, gzr);
    off_t read_off_after = gzoffset(gzr);
    int gzclose_ret_r = gzclose(gzr);

    // step 4: Validate / Cleanup
    uLong checksum = adler32_z(0UL, read_buf, read_items);
    free(comp_buf);
    free(read_buf);
    (void)version;
    (void)bound;
    (void)gzwrite_ret;
    (void)gzclose_ret_w;
    (void)gzclose_ret_r;
    (void)write_off;
    (void)read_off_before;
    (void)read_off_after;
    (void)checksum;
    // API sequence test completed successfully
    return 66;
}