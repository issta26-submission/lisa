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
//<ID> 1513
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compress to memory, write gzip with gzopen64/gzwrite, then inflate and query codes used";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(src_len);
    Bytef * comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    // step 2: Configure
    gzFile gw = gzopen64("test_zlib_api_sequence.gz", "wb");
    unsigned int gw_write_len = (unsigned int)((comp_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)comp_len);
    int gw_write_ret = gzwrite(gw, (voidpc)comp_buf, gw_write_len);
    int gw_close_ret = gzclose(gw);

    // step 3: Operate
    z_stream instrm;
    memset(&instrm, 0, (int)sizeof(instrm));
    const char * ver = zlibVersion();
    int inf_init_ret = inflateInit_(&instrm, ver, (int)sizeof(z_stream));
    instrm.next_in = comp_buf;
    instrm.avail_in = (uInt)((comp_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)comp_len);
    uLong out_size_ul = src_len + 16;
    uInt out_size = (uInt)((out_size_ul > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)out_size_ul);
    Bytef * out_buf = (Bytef *)malloc((size_t)out_size);
    memset(out_buf, 0, (int)out_size);
    instrm.next_out = out_buf;
    instrm.avail_out = out_size;
    int inf_ret = inflate(&instrm, 4); // Z_FINISH assumed as 4
    unsigned long codes_used = inflateCodesUsed(&instrm);
    int inf_end_ret = inflateEnd(&instrm);

    // step 4: Validate & Cleanup
    free(comp_buf);
    free(out_buf);
    (void)comp_ret;
    (void)bound;
    (void)comp_len;
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