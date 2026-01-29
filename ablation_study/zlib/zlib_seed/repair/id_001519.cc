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
//<ID> 1519
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and compression buffer)
    const Bytef src[] = "zlib API sequence test: compress, write gzip, inflate and inspect codes used";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong comp_bound = compressBound(src_len);
    Bytef * comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress((Bytef *)comp_buf, &comp_len, (const Bytef *)src, src_len);

    // step 2: Configure (open a gzip file for writing and write compressed data)
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    unsigned int write_len = (unsigned int)((comp_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)comp_len);
    int gw_write_ret = gzwrite(gz, (voidpc)comp_buf, write_len);

    // step 3: Operate (inflate the compressed buffer in-memory and query codes used)
    z_stream inf;
    memset(&inf, 0, (int)sizeof(inf));
    const char * ver = zlibVersion();
    int inf_init_ret = inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    Bytef * out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    inf.next_out = out_buf;
    inf.avail_out = (uInt)src_len;
    int inf_ret = inflate(&inf, 0);
    unsigned long codes_used = inflateCodesUsed(&inf);
    int inf_end_ret = inflateEnd(&inf);

    // step 4: Validate & Cleanup (close file, free buffers, silence unused results)
    int gz_close_ret = gzclose(gz);
    free(comp_buf);
    free(out_buf);
    (void)comp_ret;
    (void)gw_write_ret;
    (void)inf_init_ret;
    (void)ver;
    (void)inf_ret;
    (void)codes_used;
    (void)inf_end_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}