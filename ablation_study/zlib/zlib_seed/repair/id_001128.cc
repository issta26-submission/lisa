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
//<ID> 1128
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source data and deflate stream
    const Bytef source[] = "zlib API sequence fuzz driver payload for gzread/gzgets and deflateBound test.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *version = zlibVersion();
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure deflate stream and allocate output buffer using deflateBound
    uLong bound = deflateBound(&strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)source_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;

    // step 3: Operate: perform deflate, then write original source to a .gz file and read it back using gzread and gzgets/gzeof
    int def_ret = deflate(&strm, 4);
    uLong comp_size = strm.total_out;
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gw, source, (unsigned int)source_len);
    int gzclose_write_ret = gzclose(gw);
    gzFile gr = gzopen("tmp_zlib_api_seq.gz", "rb");
    uLong read_alloc = source_len + 16;
    Bytef *read_buf = (Bytef *)malloc((size_t)read_alloc);
    memset(read_buf, 0, (size_t)read_alloc);
    int gzread_ret = gzread(gr, read_buf, (unsigned int)read_alloc);
    char *line_buf = (char *)malloc(128);
    memset(line_buf, 0, 128);
    char *gets_ret = gzgets(gr, line_buf, 128);
    int eof_flag = gzeof(gr);

    // step 4: Cleanup and finalize
    int end_ret = deflateEnd(&strm);
    int gzclose_read_ret = gzclose(gr);
    free(comp_buf);
    free(read_buf);
    free(line_buf);
    (void)init_ret;
    (void)def_ret;
    (void)comp_size;
    (void)gzwrite_ret;
    (void)gzclose_write_ret;
    (void)gzread_ret;
    (void)gets_ret;
    (void)eof_flag;
    (void)end_ret;
    (void)gzclose_read_ret;
    (void)version;
    (void)bound;
    (void)source_len;
    return 66;
    // API sequence test completed successfully
}