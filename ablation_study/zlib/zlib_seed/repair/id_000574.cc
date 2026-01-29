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
//<ID> 574
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
    Bytef comp_buf[512];
    memset(comp_buf, 0, sizeof(comp_buf));
    uLongf comp_len = (uLongf)sizeof(comp_buf);
    int comp_ret = compress(comp_buf, &comp_len, plain_in, (uLong)sizeof(plain_in));

    // step 2: Setup / Configure
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gw, comp_buf, (unsigned int)comp_len);
    off_t gw_pos = gzseek(gw, (off_t)0, 0);

    // step 3: Operate
    int gw_close_ret = gzclose(gw);
    gzFile gr = gzopen64("test_zlib_api_sequence.tmp.gz", "rb");
    off_t gr_pos = gzseek(gr, (off_t)0, 0);
    Bytef read_back[512];
    memset(read_back, 0, sizeof(read_back));
    int gr_read = gzread(gr, read_back, (unsigned int)sizeof(read_back));
    int gr_close_ret = gzclose(gr);

    // step 4: Validate / Cleanup
    (void)ver;
    (void)comp_ret;
    (void)comp_len;
    (void)gw_write;
    (void)gw_pos;
    (void)gw_close_ret;
    (void)gr_pos;
    (void)gr_read;
    (void)gr_close_ret;
    // API sequence test completed successfully
    return 66;
}