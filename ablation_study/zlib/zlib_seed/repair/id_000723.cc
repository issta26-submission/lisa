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
//<ID> 723
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

    // step 2: Configure (write compressed file)
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_write = gzwrite(gw, plain, (unsigned int)plain_len);
    int gw_close = gzclose(gw);

    // step 3: Operate (reopen, seek and read a portion)
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    off64_t seek_offset = (off64_t)128;
    off64_t seek_ret = gzseek64(gr, seek_offset, 0);
    z_size_t read_len = (z_size_t)256;
    Bytef *read_buf = (Bytef *)malloc((size_t)read_len);
    memset(read_buf, 0, (size_t)read_len);
    z_size_t read_count = gzfread(read_buf, (z_size_t)1, read_len, gr);
    int gr_close = gzclose(gr);

    // step 4: Validate / Cleanup
    uLong a1 = adler32(0UL, plain, (uInt)seek_offset);
    uLong a2 = adler32(0UL, read_buf, (uInt)read_count);
    off64_t part_len = (off64_t)read_count;
    uLong combined = adler32_combine64(a1, a2, part_len);
    free(read_buf);
    (void)version;
    (void)plain_len;
    (void)gw_write;
    (void)gw_close;
    (void)seek_ret;
    (void)read_count;
    (void)gr_close;
    (void)a1;
    (void)a2;
    (void)part_len;
    (void)combined;
    // API sequence test completed successfully
    return 66;
}