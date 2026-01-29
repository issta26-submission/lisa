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
//<ID> 1543
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: gzdirect, crc32, inflateUndermine and gzclose usage";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *ver = zlibVersion();
    int init_ret = inflateInit_(&strm, ver, (int)sizeof(z_stream));

    // step 2: Configure
    int undermine_set = inflateUndermine(&strm, 1);
    uLong initial_crc = crc32(0UL, src, (uInt)src_len);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int direct_flag = gzdirect(gz);
    unsigned int write_len = (unsigned int)src_len;
    int write_ret = gzwrite(gz, (voidpc)src, write_len);

    // step 3: Operate
    uLong verify_crc = crc32(0UL, src, (uInt)src_len);
    (void)verify_crc;
    (void)initial_crc;
    (void)direct_flag;
    (void)write_ret;

    // step 4: Validate & Cleanup
    int gz_close_ret = gzclose(gz);
    int end_ret = inflateEnd(&strm);
    (void)init_ret;
    (void)undermine_set;
    (void)gz_close_ret;
    (void)end_ret;
    // API sequence test completed successfully
    return 66;
}