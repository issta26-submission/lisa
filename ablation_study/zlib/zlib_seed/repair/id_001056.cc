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
//<ID> 1056
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence test payload: compress, write to gz, read back, validate and cleanup.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    int reset_ret = deflateReset(&strm);

    // step 2: Configure
    uLong bound = deflateBound(&strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)source_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&strm, 4); // use 4 (Z_FINISH)
    uLong comp_size = strm.total_out;
    gzFile gzw = gzopen("tmp_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzw, comp_buf, (unsigned int)comp_size);
    off_t write_offset = gzoffset(gzw);
    int gzw_close_ret = gzclose(gzw);
    gzFile gzr = gzopen("tmp_zlib_api_sequence.gz", "rb");
    z_size_t read_buf_len = (z_size_t)(comp_size + 64);
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_len);
    memset(read_buf, 0, (size_t)read_buf_len);
    z_size_t read_items = gzfread(read_buf, 1, read_buf_len, gzr);
    int gzr_close_ret = gzclose(gzr);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&strm);
    uLong checksum = adler32_z(0UL, read_buf, (z_size_t)read_items);
    free(comp_buf);
    free(read_buf);
    (void)init_ret;
    (void)reset_ret;
    (void)def_ret;
    (void)gw_ret;
    (void)write_offset;
    (void)gzw_close_ret;
    (void)gzr_close_ret;
    (void)def_end_ret;
    (void)checksum;
    (void)version;
    (void)source_len;
    (void)bound;
    (void)comp_size;
    // API sequence test completed successfully
    return 66;
}