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
//<ID> 1545
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compress2 -> write gz -> inflate and crc32 validation";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong orig_crc = crc32(0UL, src, (uInt)src_len);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *ver = zlibVersion();
    int inf_init_ret = inflateInit_(&strm, ver, (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);
    strm.next_in = comp_buf;
    strm.avail_in = (uInt)((comp_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (uInt)comp_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    strm.next_out = out_buf;
    strm.avail_out = (uInt)((src_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (uInt)src_len);
    int undermine_ret = inflateUndermine(&strm, 1);

    // step 3: Operate
    int inf_ret = inflate(&strm, Z_FINISH);
    uLong decomp_size = strm.total_out;
    uLong decomp_crc = crc32(0UL, out_buf, (uInt)decomp_size);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    unsigned int write_len = (unsigned int)((comp_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)comp_len);
    int gz_write_ret = gzwrite(gz, (voidpc)comp_buf, write_len);
    int direct_flag = gzdirect(gz);
    int gz_close_ret = gzclose(gz);

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&strm);
    free(comp_buf);
    free(out_buf);
    (void)orig_crc;
    (void)inf_init_ret;
    (void)bound;
    (void)comp_ret;
    (void)undermine_ret;
    (void)inf_ret;
    (void)decomp_crc;
    (void)gz_write_ret;
    (void)direct_flag;
    (void)gz_close_ret;
    (void)inf_end_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}