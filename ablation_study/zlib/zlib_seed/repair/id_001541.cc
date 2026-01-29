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
//<ID> 1541
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: write gz, check direct mode, compute crc32 and use inflateUndermine";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong orig_crc = crc32(0UL, src, (uInt)src_len);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *ver = zlibVersion();
    int init_ret = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    int undermine_ret = inflateUndermine(&strm, 1);

    // step 2: Configure & Operate
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    unsigned int write_len = (unsigned int)src_len;
    int gz_write_ret = gzwrite(gz, (voidpc)src, write_len);
    int direct_flag = gzdirect(gz);

    // step 3: Validate
    uLong validate_crc = crc32(0UL, src, (uInt)src_len);
    (void)validate_crc;
    (void)direct_flag;
    (void)gz_write_ret;
    (void)init_ret;
    (void)undermine_ret;
    (void)ver;
    (void)orig_crc;

    // step 4: Cleanup
    int inf_end_ret = inflateEnd(&strm);
    int gz_close_ret = gzclose(gz);

    // API sequence test completed successfully
    (void)inf_end_ret;
    (void)gz_close_ret;
    return 66;
}