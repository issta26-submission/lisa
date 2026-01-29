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
//<ID> 1512
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and allocate buffers)
    const Bytef src[] = "zlib API sequence test: compress to buffer, write gzip via gzopen64/gzwrite, then inflate from buffer and query inflateCodesUsed";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(src_len);
    Bytef * comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLong comp_len = bound;
    (void)bound;
    const char * ver = zlibVersion();
    int comp_ret = compress2(comp_buf, (uLongf *)&comp_len, src, src_len, 6);

    // step 2: Configure & Operate (write compressed payload to a gzip file)
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    unsigned int write_len = (unsigned int)((comp_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)comp_len);
    int gz_write_ret = gzwrite(gz, (voidpc)comp_buf, write_len);
    int gz_close_ret = gzclose(gz);

    // step 3: Operate (inflate the compressed buffer in-memory and query codes used)
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int inf_init_ret = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    strm.next_in = comp_buf;
    strm.avail_in = (uInt)((comp_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)comp_len);
    uLong out_size = (src_len > 4096UL) ? (src_len + 4096UL) : (src_len * 2UL + 64UL);
    Bytef * out_buf = (Bytef *)malloc((size_t)out_size);
    memset(out_buf, 0, (int)out_size);
    strm.next_out = out_buf;
    strm.avail_out = (uInt)((out_size > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)out_size);
    int inf_ret = inflate(&strm, 0);
    unsigned long codes_used = inflateCodesUsed(&strm);
    int inf_end_ret = inflateEnd(&strm);

    // step 4: Validate & Cleanup
    free(comp_buf);
    free(out_buf);
    (void)comp_ret;
    (void)ver;
    (void)gz_write_ret;
    (void)gz_close_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}