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
//<ID> 1544
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compute CRC, init inflate, write gz, and use gzdirect";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong computed_crc = crc32(0UL, src, (uInt)src_len);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *ver = zlibVersion();
    int init_ret = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    strm.adler = computed_crc;

    // step 2: Configure
    int undermine_ret = inflateUndermine(&strm, 1);
    uInt out_alloc = 256u;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (int)out_alloc);
    strm.next_out = out_buf;
    strm.avail_out = out_alloc;

    // step 3: Operate
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    unsigned int write_len = (unsigned int)((src_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)src_len);
    int write_ret = gzwrite(gz, (voidpc)src, write_len);
    int direct_flag = gzdirect(gz);

    // step 4: Validate & Cleanup
    int gz_close_ret = gzclose(gz);
    int end_ret = inflateEnd(&strm);
    free(out_buf);
    (void)init_ret;
    (void)undermine_ret;
    (void)write_ret;
    (void)direct_flag;
    (void)gz_close_ret;
    (void)end_ret;
    (void)computed_crc;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}