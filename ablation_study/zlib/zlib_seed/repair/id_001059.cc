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
//<ID> 1059
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize / Configure
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence test payload: compress, write gz, read back, inspect offsets and checksum.";
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

    // step 2: Operate (compress and finalize)
    int def_ret = deflate(&strm, 4);
    uLong comp_size = strm.total_out;
    int end_ret = deflateEnd(&strm);

    // step 3: Write compressed data to gz, then read it back with gzfread and inspect offsets
    gzFile gzw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gzw, comp_buf, (unsigned int)comp_size);
    off_t write_offset = gzoffset(gzw);
    int gzclose_ret_w = gzclose(gzw);
    gzFile gzr = gzopen("tmp_zlib_api_seq.gz", "rb");
    z_size_t read_buf_len = (z_size_t)(comp_size + 64);
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_len);
    memset(read_buf, 0, (size_t)read_buf_len);
    z_size_t read_items = gzfread(read_buf, 1, read_buf_len, gzr);
    off_t read_offset = gzoffset(gzr);
    int gzclose_ret_r = gzclose(gzr);

    // step 4: Validate / Cleanup
    uLong checksum = crc32(0UL, read_buf, (uInt)read_items);
    free(comp_buf);
    free(read_buf);
    (void)init_ret;
    (void)reset_ret;
    (void)def_ret;
    (void)end_ret;
    (void)gzwrite_ret;
    (void)gzclose_ret_w;
    (void)gzclose_ret_r;
    (void)write_offset;
    (void)read_offset;
    (void)checksum;
    (void)version;
    (void)source_len;
    (void)bound;
    (void)comp_size;
    // API sequence test completed successfully
    return 66;
}