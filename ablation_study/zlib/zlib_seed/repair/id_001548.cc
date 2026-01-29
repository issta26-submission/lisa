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
//<ID> 1548
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compress, write gz, crc32 and inflateUndermine usage";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong orig_crc = crc32(0UL, src, (uInt)src_len);
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress(comp_buf, &comp_len, src, src_len);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    unsigned int write_len = (unsigned int)((comp_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)comp_len);
    int gz_write_ret = gzwrite(gz, (voidpc)comp_buf, write_len);
    int direct_flag = gzdirect(gz);

    // step 2: Configure
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    strm.next_in = comp_buf;
    strm.avail_in = (uInt)((comp_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (uInt)comp_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    strm.next_out = out_buf;
    strm.avail_out = (uInt)((src_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (uInt)src_len);
    const char *ver = zlibVersion();
    int inf_init_ret = inflateInit_(&strm, ver, (int)sizeof(z_stream));

    // step 3: Operate
    uLong comp_crc = crc32(0UL, comp_buf, (uInt)comp_len);
    int undermine_ret = inflateUndermine(&strm, 1);

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&strm);
    int gz_close_ret = gzclose(gz);
    free(comp_buf);
    free(out_buf);
    (void)orig_crc;
    (void)bound;
    (void)comp_ret;
    (void)gz_write_ret;
    (void)direct_flag;
    (void)inf_init_ret;
    (void)comp_crc;
    (void)undermine_ret;
    (void)inf_end_ret;
    (void)gz_close_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}