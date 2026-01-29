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
//<ID> 1121
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source and deflate stream
    const Bytef source[] = "zlib API sequence fuzz driver payload demonstrating deflateBound, gzwrite/gzread and gzgets usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *version = zlibVersion();
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure stream, compute bound, compress into buffer
    uLong bound = deflateBound(&strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)source_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    int def_ret = deflate(&strm, 4);
    uLong comp_size = strm.total_out;
    int end_ret = deflateEnd(&strm);

    // step 3: Persist compressed buffer to a .gz file, then reopen and read via gzread and gzgets
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gw_ret = gzwrite(gw, comp_buf, (unsigned int)comp_size);
    int gzclose_w = gzclose(gw);
    gzFile gr = gzopen("tmp_zlib_api_seq.gz", "rb");
    uInt read_buf_size = (uInt)(comp_size + 64);
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (size_t)read_buf_size);
    int gzread_ret = gzread(gr, read_buf, read_buf_size);
    char line_buf[128];
    memset(line_buf, 0, (int)sizeof(line_buf));
    char *line_ptr = gzgets(gr, line_buf, (int)sizeof(line_buf));
    int eof_flag = gzeof(gr);

    // step 4: Validate with crc32, cleanup and finalize
    uLong checksum = crc32(0UL, read_buf, (uInt)gzread_ret);
    int gzclose_r = gzclose(gr);
    free(comp_buf);
    free(read_buf);
    (void)init_ret;
    (void)def_ret;
    (void)end_ret;
    (void)gw_ret;
    (void)gzclose_w;
    (void)gzread_ret;
    (void)line_ptr;
    (void)eof_flag;
    (void)checksum;
    (void)gzclose_r;
    (void)version;
    (void)bound;
    (void)source_len;
    return 66;
    // API sequence test completed successfully
}