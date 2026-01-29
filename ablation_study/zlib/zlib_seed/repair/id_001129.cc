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
//<ID> 1129
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source data and deflate stream
    const Bytef source[] = "zlib API sequence fuzz driver payload for gzgets/gzread/deflateBound usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&strm, source_len);

    // step 2: Compress into a buffer using deflate, then finalize deflate
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)source_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    int def_ret = deflate(&strm, 4);
    uLong comp_size = strm.total_out;
    int def_end_ret = deflateEnd(&strm);

    // step 3: Persist original source into a .gz file, then read it back using gzgets and gzread and check EOF
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gw, source, (unsigned int)source_len);
    int gzclose_write_ret = gzclose(gw);
    gzFile gr = gzopen("tmp_zlib_api_seq.gz", "rb");
    char linebuf[256];
    memset(linebuf, 0, (size_t)sizeof(linebuf));
    char *gzgets_ret = gzgets(gr, linebuf, (int)sizeof(linebuf));
    z_size_t read_buf_size = (z_size_t)(source_len + 128);
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (size_t)read_buf_size);
    int gzread_ret = gzread(gr, read_buf, (unsigned int)read_buf_size);
    int gzeof_ret = gzeof(gr);
    int gzclose_read_ret = gzclose(gr);

    // step 4: Validate with crc32, cleanup and finalize
    uLong checksum = crc32(0UL, read_buf, (uInt)(gzread_ret > 0 ? (unsigned int)gzread_ret : 0U));
    free(comp_buf);
    free(read_buf);
    (void)def_init_ret;
    (void)def_ret;
    (void)comp_size;
    (void)def_end_ret;
    (void)bound;
    (void)version;
    (void)gzwrite_ret;
    (void)gzclose_write_ret;
    (void)gzgets_ret;
    (void)gzread_ret;
    (void)gzeof_ret;
    (void)gzclose_read_ret;
    (void)checksum;
    return 66;
    // API sequence test completed successfully
}