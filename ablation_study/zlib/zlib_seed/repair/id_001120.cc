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
//<ID> 1120
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source and deflate stream
    const Bytef source[] = "zlib API sequence fuzz driver payload to exercise deflateBound, gzgets, gzread and gzeof.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *version = zlibVersion();
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)source_len;

    // step 2: Configure output buffer using deflateBound and run deflate
    uLong bound = deflateBound(&strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    int def_ret = deflate(&strm, 4);
    uLong comp_size = strm.total_out;
    int end_ret = deflateEnd(&strm);

    // step 3: Persist compressed bytes to a .gz file, then reopen and read using gzgets, gzread and gzeof
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gw_ret = gzwrite(gw, comp_buf, (unsigned int)comp_size);
    int gw_close_ret = gzclose(gw);
    gzFile gr = gzopen("tmp_zlib_api_seq.gz", "rb");
    char line_buf[128];
    memset(line_buf, 0, 128);
    char *gets_ret = gzgets(gr, line_buf, 128);
    uLong read_alloc = comp_size + 64;
    Bytef *read_buf = (Bytef *)malloc((size_t)read_alloc);
    memset(read_buf, 0, (size_t)read_alloc);
    int gzread_ret = gzread(gr, (voidp)read_buf, (unsigned int)read_alloc);
    int eof_ret = gzeof(gr);
    int gr_close_ret = gzclose(gr);

    // step 4: Validate a checksum over read buffer and cleanup
    uLong checksum = crc32(0UL, read_buf, (uInt)gzread_ret);
    free(comp_buf);
    free(read_buf);
    (void)init_ret;
    (void)def_ret;
    (void)end_ret;
    (void)version;
    (void)bound;
    (void)comp_size;
    (void)gw_ret;
    (void)gw_close_ret;
    (void)gets_ret;
    (void)gzread_ret;
    (void)eof_ret;
    (void)gr_close_ret;
    (void)checksum;
    (void)source_len;
    return 66;
    // API sequence test completed successfully
}