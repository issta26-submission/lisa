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
//<ID> 1511
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and allocate buffers)
    const Bytef src[] = "zlib API sequence test: compress with compress(), write compressed blob to gzip via gzopen64/gzwrite, then inflate and query codes used";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(src_len);
    Bytef * comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress(comp_buf, &comp_len, src, src_len);

    // step 2: Configure (write compressed data to a gzip file)
    gzFile gw = gzopen64("test_zlib_api_sequence.gz", "wb");
    unsigned int write_len = (unsigned int)((comp_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFUL : (unsigned int)comp_len);
    int gw_write_ret = gzwrite(gw, (voidpc)comp_buf, write_len);
    int gw_close_ret = gzclose(gw);

    // step 3: Operate (initialize inflate stream and perform inflation on compressed buffer)
    z_stream inf;
    memset(&inf, 0, (int)sizeof(inf));
    const char * ver = zlibVersion();
    int inf_init_ret = inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)((comp_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFUL : (unsigned int)comp_len);
    uLong out_size = src_len + 64;
    Bytef * out_buf = (Bytef *)malloc((size_t)out_size);
    memset(out_buf, 0, (int)out_size);
    inf.next_out = out_buf;
    inf.avail_out = (uInt)((out_size > 0xFFFFFFFFUL) ? 0xFFFFFFFFUL : (unsigned int)out_size);
    int inf_ret = inflate(&inf, 0);
    unsigned long codes_used = inflateCodesUsed(&inf);

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&inf);
    free(comp_buf);
    free(out_buf);
    (void)comp_ret;
    (void)bound;
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)inf_init_ret;
    (void)ver;
    (void)inf_ret;
    (void)codes_used;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}