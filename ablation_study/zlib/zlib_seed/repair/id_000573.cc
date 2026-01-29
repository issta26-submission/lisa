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
//<ID> 573
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef plain[128];
    memset(plain, 'A', sizeof(plain));
    uLong src_len = (uLong)sizeof(plain);
    Bytef comp_buf[512];
    memset(comp_buf, 0, sizeof(comp_buf));
    uLongf comp_len = (uLongf)sizeof(comp_buf);
    int comp_ret = compress(comp_buf, &comp_len, plain, src_len);

    // step 2: Configure / Write compressed data with gzopen64
    gzFile gw64 = gzopen64("test_zlib_api_sequence.tmp.gz", "wb");
    int gw64_write_ret = gzwrite(gw64, comp_buf, (unsigned int)comp_len);
    off_t gw64_seek_ret = gzseek(gw64, (off_t)0, SEEK_SET);
    int gw64_close_ret = gzclose(gw64);

    // step 3: Operate / Read back using gzopen and reposition with gzseek
    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    Bytef read_back[256];
    memset(read_back, 0, sizeof(read_back));
    int gr_read_ret = gzread(gr, read_back, (unsigned int)sizeof(read_back));
    off_t gr_seek_ret = gzseek(gr, (off_t)0, SEEK_SET);
    int gr_close_ret = gzclose(gr);

    // step 4: Validate / Cleanup
    uLong crc_orig = crc32(0UL, plain, (uInt)src_len);
    uLong crc_read = crc32(0UL, read_back, (uInt)sizeof(read_back));

    (void)ver;
    (void)comp_ret;
    (void)comp_len;
    (void)gw64_write_ret;
    (void)gw64_seek_ret;
    (void)gw64_close_ret;
    (void)gr_read_ret;
    (void)gr_seek_ret;
    (void)gr_close_ret;
    (void)crc_orig;
    (void)crc_read;
    // API sequence test completed successfully
    return 66;
}