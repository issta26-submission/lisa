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
//<ID> 1510
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test payload: compress then write via gz, track inflate usage counters";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(src_len);
    Bytef * comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;

    // step 2: Configure
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);
    z_stream instrm;
    memset(&instrm, 0, (int)sizeof(instrm));
    const char * ver = zlibVersion();
    int inf_init_ret = inflateInit_(&instrm, ver, (int)sizeof(z_stream));

    // step 3: Operate
    gzFile gw = gzopen64("test_zlib_api_sequence.gz", "wb");
    unsigned int write_len = (unsigned int)((comp_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)comp_len);
    int gw_write_ret = gzwrite(gw, (voidpc)comp_buf, write_len);
    unsigned long codes_used = inflateCodesUsed(&instrm);

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&instrm);
    int gw_close_ret = gzclose(gw);
    free(comp_buf);
    (void)src_len;
    (void)bound;
    (void)comp_ret;
    (void)ver;
    (void)inf_init_ret;
    (void)gw_write_ret;
    (void)codes_used;
    (void)inf_end_ret;
    (void)gw_close_ret;
    // API sequence test completed successfully
    return 66;
}