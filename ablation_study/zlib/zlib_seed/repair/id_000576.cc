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
//<ID> 576
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef plain_in[128];
    memset(plain_in, 'A', sizeof(plain_in));
    uLong source_len = (uLong)sizeof(plain_in);
    Bytef comp_buf[256];
    memset(comp_buf, 0, sizeof(comp_buf));
    uLongf comp_len = (uLongf)sizeof(comp_buf);
    int comp_ret = compress(comp_buf, &comp_len, plain_in, source_len);
    uLong crc_before = crc32(0UL, plain_in, (uInt)source_len);

    // step 2: Setup / Write compressed data into a gz file (using gzopen)
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gw, plain_in, (unsigned int)source_len);
    int gw_close_ret = gzclose(gw);

    // step 3: Operate / Open via gzopen64 and seek then read back
    gzFile gr64 = gzopen64("test_zlib_api_sequence.tmp.gz", "rb");
    off_t pos_start = gzseek(gr64, 0, 0);
    Bytef read_back[128];
    memset(read_back, 0, sizeof(read_back));
    int gr_read = gzread(gr64, read_back, (unsigned int)sizeof(read_back));
    off_t pos_after_read = gzseek(gr64, 0, 1);

    // step 4: Validate / Cleanup
    uLong crc_after = crc32(0UL, read_back, (uInt)sizeof(read_back));
    int gr_close_ret = gzclose(gr64);

    (void)ver;
    (void)comp_ret;
    (void)comp_len;
    (void)crc_before;
    (void)gw_write;
    (void)gw_close_ret;
    (void)pos_start;
    (void)gr_read;
    (void)pos_after_read;
    (void)crc_after;
    (void)gr_close_ret;
    // API sequence test completed successfully
    return 66;
}