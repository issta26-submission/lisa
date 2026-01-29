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
//<ID> 1515
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (source data, compute compression bound, allocate buffers, compress)
    const Bytef src[] = "zlib API sequence test: compress to memory, write with gzopen64/gzwrite, then inflate and inspect codes used";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(src_len);
    Bytef * comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLong dest_len = bound;
    int comp_ret = compress(comp_buf, (uLongf *)&dest_len, src, src_len);

    // step 2: Configure (write compressed data to a gzip file using gzopen64 and gzwrite)
    gzFile gzf = gzopen64("test_zlib_api_sequence.gz", "wb");
    unsigned int write_len = (unsigned int)((dest_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)dest_len);
    int gw_ret = gzwrite(gzf, (voidpc)comp_buf, write_len);
    int gclose_ret = gzclose(gzf);

    // step 3: Operate (inflate the in-memory compressed data and query inflateCodesUsed)
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char * ver = zlibVersion();
    int inf_init_ret = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    strm.next_in = comp_buf;
    strm.avail_in = (uInt)dest_len;
    uLong out_max = src_len + 128;
    Bytef * out_buf = (Bytef *)malloc((size_t)out_max);
    memset(out_buf, 0, (int)out_max);
    strm.next_out = out_buf;
    strm.avail_out = (uInt)out_max;
    int inf_ret = inflate(&strm, 0);
    unsigned long codes_used = inflateCodesUsed(&strm);

    // step 4: Validate & Cleanup (end inflate, free buffers, close file handles)
    int inf_end_ret = inflateEnd(&strm);
    free(comp_buf);
    free(out_buf);
    (void)comp_ret;
    (void)bound;
    (void)write_len;
    (void)gw_ret;
    (void)gclose_ret;
    (void)ver;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}