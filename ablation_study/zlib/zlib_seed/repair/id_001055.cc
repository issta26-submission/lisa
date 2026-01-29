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
//<ID> 1055
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence test payload: compress, write to gz, read back, validate checksum.";
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

    // step 2: Configure
    int reset_ret = deflateReset(&strm);

    // step 3: Operate
    int def_ret = deflate(&strm, 4);
    uLong comp_size = strm.total_out;
    gzFile gzw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gzw, comp_buf, (unsigned int)comp_size);
    off_t write_offset = gzoffset(gzw);
    int gzclose_ret_w = gzclose(gzw);
    gzFile gzr = gzopen("tmp_zlib_api_seq.gz", "rb");
    z_size_t read_buf_len = (z_size_t)(source_len + 128);
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_len);
    memset(read_buf, 0, (size_t)read_buf_len);
    z_size_t items_read = gzfread(read_buf, 1, read_buf_len, gzr);
    off_t read_offset = gzoffset(gzr);
    int gzclose_ret_r = gzclose(gzr);

    // step 4: Validate / Cleanup
    uLong checksum = adler32_z(0UL, read_buf, items_read);
    int def_end_ret = deflateEnd(&strm);
    free(comp_buf);
    free(read_buf);
    (void)init_ret;
    (void)reset_ret;
    (void)def_ret;
    (void)comp_size;
    (void)gzwrite_ret;
    (void)write_offset;
    (void)gzclose_ret_w;
    (void)items_read;
    (void)read_offset;
    (void)gzclose_ret_r;
    (void)checksum;
    (void)def_end_ret;
    (void)version;
    /* API sequence test completed successfully */
    return 66;
}