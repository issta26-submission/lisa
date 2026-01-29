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
//<ID> 728
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain[512];
    memset(plain, 'Z', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);

    // step 2: Configure
    gzFile gz = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw = gzwrite(gz, plain, (unsigned int)plain_len);
    int gw_close = gzclose(gz);

    // step 3: Operate
    gzFile gzr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    off64_t seek_pos = gzseek64(gzr, (off64_t)128, 0);
    Bytef read_buf[256];
    memset(read_buf, 0, sizeof(read_buf));
    z_size_t items_read = gzfread(read_buf, (z_size_t)1, (z_size_t)256, gzr);
    int gr_close = gzclose(gzr);

    // step 4: Validate / Cleanup
    uLong a1 = adler32(0UL, plain, (uInt)128);
    uLong a2 = adler32(0UL, read_buf, (uInt)items_read);
    off64_t part_len = (off64_t)items_read;
    uLong combined = adler32_combine64(a1, a2, part_len);

    (void)version;
    (void)plain_len;
    (void)gw;
    (void)gw_close;
    (void)seek_pos;
    (void)items_read;
    (void)gr_close;
    (void)a1;
    (void)a2;
    (void)part_len;
    (void)combined;
    // API sequence test completed successfully
    return 66;
}