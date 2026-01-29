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
//<ID> 571
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
    uLong in_len = (uLong)sizeof(plain_in);
    Bytef comp_buf[256];
    memset(comp_buf, 0, sizeof(comp_buf));
    uLongf comp_len = (uLongf)sizeof(comp_buf);

    // step 2: Setup / Configure
    int comp_ret = compress(comp_buf, &comp_len, plain_in, in_len);
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzFile gw64 = gzopen64("test_zlib_api_sequence.tmp64.gz", "wb");

    // step 3: Operate
    int gw_write = gzwrite(gw, comp_buf, (unsigned int)comp_len);
    int gw64_write = gzwrite(gw64, comp_buf, (unsigned int)comp_len);
    off_t gw_pos_before = gzseek(gw, (off_t)0, 0);
    off_t gw64_pos_before = gzseek(gw64, (off_t)0, 0);
    int gw_close = gzclose(gw);
    int gw64_close = gzclose(gw64);

    // step 4: Validate / Cleanup
    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    gzFile gr64 = gzopen64("test_zlib_api_sequence.tmp64.gz", "rb");
    Bytef read_back[256];
    memset(read_back, 0, sizeof(read_back));
    int gr_read = gzread(gr, read_back, (unsigned int)sizeof(read_back));
    int gr64_read = gzread(gr64, read_back, (unsigned int)sizeof(read_back));
    off_t gr_pos_after = gzseek(gr, (off_t)0, 0);
    off_t gr64_pos_after = gzseek(gr64, (off_t)0, 0);
    Bytef decomp_buf[256];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    uLongf decomp_len = (uLongf)sizeof(decomp_buf);
    int un_ret = uncompress(decomp_buf, &decomp_len, read_back, (uLong)gr_read);
    int gr_close = gzclose(gr);
    int gr64_close = gzclose(gr64);

    (void)ver;
    (void)comp_ret;
    (void)comp_len;
    (void)gw_write;
    (void)gw64_write;
    (void)gw_pos_before;
    (void)gw64_pos_before;
    (void)gw_close;
    (void)gw64_close;
    (void)gr_read;
    (void)gr64_read;
    (void)gr_pos_after;
    (void)gr64_pos_after;
    (void)decomp_len;
    (void)un_ret;
    (void)gr_close;
    (void)gr64_close;
    // API sequence test completed successfully
    return 66;
}