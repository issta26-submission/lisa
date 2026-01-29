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
//<ID> 1546
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: write gz, crc32, inflateUndermine and gzdirect usage";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong src_crc = crc32(0UL, src, (uInt)src_len);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");

    // step 2: Configure
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *ver = zlibVersion();
    int inf_init_ret = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    int undermine_ret = inflateUndermine(&strm, 1);

    // step 3: Operate
    unsigned int write_len = (unsigned int)((src_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)src_len);
    int gz_write_ret = gzwrite(gz, (voidpc)src, write_len);
    int direct_flag = gzdirect(gz);

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&strm);
    int gz_close_ret = gzclose(gz);

    (void)src_crc;
    (void)inf_init_ret;
    (void)undermine_ret;
    (void)gz_write_ret;
    (void)direct_flag;
    (void)inf_end_ret;
    (void)gz_close_ret;
    (void)ver;

    // API sequence test completed successfully
    return 66;
}