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
//<ID> 1123
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source data and deflate stream, compute bound
    const Bytef source[] = "zlib API sequence fuzz driver payload for gzgets/gzread and deflateBound test.\n";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&strm, source_len);

    // step 2: Allocate compression buffer and compress source, compute checksum
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLong comp_len = bound;
    int comp_ret = compress2(comp_buf, &comp_len, source, source_len, 6);
    uLong comp_crc = crc32(0UL, comp_buf, (uInt)comp_len);

    // step 3: Persist original source to a .gz file, reopen and read via gzgets and gzread, check EOF
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gw, source, (unsigned int)source_len);
    int gzclose_w_ret = gzclose(gw);
    gzFile gr = gzopen("tmp_zlib_api_seq.gz", "rb");
    char *line_buf = (char *)malloc(256);
    memset(line_buf, 0, 256);
    char *got_line = gzgets(gr, line_buf, 256);
    Bytef *read_buf = (Bytef *)malloc(128);
    memset(read_buf, 0, 128);
    int gzread_ret = gzread(gr, read_buf, (unsigned int)128);
    int eof_flag = gzeof(gr);
    int gzclose_r_ret = gzclose(gr);

    // step 4: Cleanup, finalize deflate stream and free resources
    int def_end_ret = deflateEnd(&strm);
    free(comp_buf);
    free(line_buf);
    free(read_buf);
    (void)def_init_ret;
    (void)bound;
    (void)comp_ret;
    (void)comp_len;
    (void)comp_crc;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)got_line;
    (void)gzread_ret;
    (void)eof_flag;
    (void)gzclose_r_ret;
    (void)def_end_ret;
    (void)version;
    (void)source_len;
    return 66;
    // API sequence test completed successfully
}