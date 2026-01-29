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
//<ID> 1125
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source, z_stream and deflate
    const Bytef source[] = "zlib API sequence fuzz driver payload for deflate and gz I/O validation.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *version = zlibVersion();
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 2: Compute bound, allocate output buffer and perform deflate
    uLong bound = deflateBound(&strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)source_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    int def_ret = deflate(&strm, 4);
    uLong comp_size = strm.total_out;
    int def_end_ret = deflateEnd(&strm);

    // step 3: Write compressed data to a .gz file, then reopen and read
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gw, comp_buf, (unsigned int)comp_size);
    int gzclose_w_ret = gzclose(gw);
    gzFile gr = gzopen("tmp_zlib_api_seq.gz", "rb");
    uLong read_alloc = comp_size + 64;
    Bytef *read_buf = (Bytef *)malloc((size_t)read_alloc);
    memset(read_buf, 0, (size_t)read_alloc);
    int gzread_ret = gzread(gr, read_buf, (unsigned int)read_alloc);
    char linebuf[256];
    memset(linebuf, 0, (int)sizeof(linebuf));
    char *gets_ret = gzgets(gr, linebuf, (int)sizeof(linebuf));
    int eof_flag = gzeof(gr);
    int gzclose_r_ret = gzclose(gr);

    // step 4: Validate checksum, cleanup and finalize
    uLong checksum = crc32(0UL, read_buf, (uInt)(gzread_ret > 0 ? gzread_ret : 0));
    free(comp_buf);
    free(read_buf);
    (void)init_ret;
    (void)def_ret;
    (void)comp_size;
    (void)def_end_ret;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)gzread_ret;
    (void)gets_ret;
    (void)eof_flag;
    (void)gzclose_r_ret;
    (void)checksum;
    (void)version;
    (void)bound;
    (void)source_len;
    return 66;
    // API sequence test completed successfully
}