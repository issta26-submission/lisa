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
//<ID> 1127
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize zlib deflate stream and compute bound
    const Bytef source[] = "zlib API sequence fuzz driver payload for gz* and deflateBound usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *version = zlibVersion();
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);

    // step 2: Configure stream buffers and perform deflate
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)source_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    int def_ret = deflate(&strm, 4);
    uLong comp_size = strm.total_out;
    int def_end_ret = deflateEnd(&strm);

    // step 3: Write compressed bytes to a .gz file, reopen and read back using gzread and gzgets
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gw_ret = gzwrite(gw, comp_buf, (unsigned int)comp_size);
    int gzclose_write_ret = gzclose(gw);
    gzFile gr = gzopen("tmp_zlib_api_seq.gz", "rb");
    size_t read_alloc = (size_t)(source_len + 256);
    Bytef *read_buf = (Bytef *)malloc(read_alloc);
    memset(read_buf, 0, read_alloc);
    int gzread_ret = gzread(gr, read_buf, (unsigned int)read_alloc);
    char line_buf[256];
    memset(line_buf, 0, sizeof(line_buf));
    char *gzgets_ret = gzgets(gr, line_buf, (int)sizeof(line_buf));
    int eof_flag = gzeof(gr);
    int gzclose_read_ret = gzclose(gr);

    // step 4: Validate checksum, cleanup and finalize
    uLong checksum = crc32(0UL, read_buf, (uInt)gzread_ret);
    free(comp_buf);
    free(read_buf);
    (void)init_ret;
    (void)def_ret;
    (void)comp_size;
    (void)def_end_ret;
    (void)gw_ret;
    (void)gzclose_write_ret;
    (void)gzread_ret;
    (void)gzgets_ret;
    (void)eof_flag;
    (void)gzclose_read_ret;
    (void)checksum;
    (void)version;
    (void)bound;
    (void)source_len;
    return 66;
    // API sequence test completed successfully
}