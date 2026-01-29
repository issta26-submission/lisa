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
//<ID> 1516
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and buffers)
    const Bytef src[] = "zlib API sequence: compress to memory, inflate, inspect codes, and write original to gzip via gzopen64/gzwrite";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(src_len);
    Bytef * comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len_f = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len_f, src, src_len, 6);
    uLong comp_len = (uLong)comp_len_f;

    // step 2: Configure (prepare inflate stream)
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    strm.next_in = comp_buf;
    strm.avail_in = (uInt)((comp_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)comp_len);
    uLong out_buf_size = src_len + 128;
    Bytef * out_buf = (Bytef *)malloc((size_t)out_buf_size);
    memset(out_buf, 0, (int)out_buf_size);
    strm.next_out = out_buf;
    strm.avail_out = (uInt)((out_buf_size > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)out_buf_size);
    const char * ver = zlibVersion();
    int inf_init_ret = inflateInit_(&strm, ver, (int)sizeof(z_stream));

    // step 3: Operate (inflate and inspect)
    int inf_ret = inflate(&strm, 0);
    unsigned long codes_used = inflateCodesUsed(&strm);
    int inf_end_ret = inflateEnd(&strm);

    // step 4: Validate & Cleanup (write original source to gzip file, close and free)
    gzFile gw = gzopen64("test_zlib_api_sequence.gz", "wb");
    unsigned int write_len = (unsigned int)((src_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)src_len);
    int gw_write_ret = gzwrite(gw, (voidpc)src, write_len);
    int gw_close_ret = gzclose(gw);
    free(comp_buf);
    free(out_buf);
    (void)comp_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)inf_end_ret;
    (void)gw_write_ret;
    (void)gw_close_ret;
    // API sequence test completed successfully
    return 66;
}