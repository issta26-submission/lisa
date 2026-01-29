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
//<ID> 1517
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compress data, write with gzopen64/gzwrite, then inflate and inspect codes used";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(src_len);
    uLongf dest_len = (uLongf)bound;
    Bytef * comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    int comp_ret = compress(comp_buf, &dest_len, src, src_len);

    // step 2: Configure (write compressed data to a gzip file)
    gzFile gw = gzopen64("test_zlib_api_sequence.gz", "wb");
    unsigned int write_len = (unsigned int)((dest_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)dest_len);
    int gw_write_ret = gzwrite(gw, (voidpc)comp_buf, write_len);
    int gw_close_ret = gzclose(gw);

    // step 3: Operate (inflate the compressed buffer and query inflateCodesUsed)
    z_stream instrm;
    memset(&instrm, 0, (int)sizeof(instrm));
    const char * ver = zlibVersion();
    int inf_init_ret = inflateInit_(&instrm, ver, (int)sizeof(z_stream));
    instrm.next_in = comp_buf;
    instrm.avail_in = (uInt)dest_len;
    uLong out_size = src_len + 64;
    Bytef * out_buf = (Bytef *)malloc((size_t)out_size);
    memset(out_buf, 0, (int)out_size);
    instrm.next_out = out_buf;
    instrm.avail_out = (uInt)out_size;
    int inf_ret = inflate(&instrm, 0);
    unsigned long codes_used = inflateCodesUsed(&instrm);

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&instrm);
    free(comp_buf);
    free(out_buf);
    (void)comp_ret;
    (void)dest_len;
    (void)write_len;
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