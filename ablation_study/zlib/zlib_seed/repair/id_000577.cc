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
//<ID> 577
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    Bytef input[128];
    memset(input, 'Z', sizeof(input));
    Bytef comp_buf[4096];
    memset(comp_buf, 0, sizeof(comp_buf));
    uLong bound_est = compressBound((uLong)sizeof(input));
    uLongf comp_len = (uLongf)sizeof(comp_buf);

    // step 2: Setup / Configure
    int comp_ret = compress(comp_buf, &comp_len, input, (uLong)sizeof(input));

    // step 3: Operate
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzFile gw64 = gzopen64("test_zlib_api_sequence64.tmp.gz", "wb");
    int gw_write = gzwrite(gw, comp_buf, (unsigned int)comp_len);
    int gw64_write = gzwrite(gw64, comp_buf, (unsigned int)comp_len);
    int gw_close = gzclose(gw);
    int gw64_close = gzclose(gw64);

    // step 4: Validate / Cleanup
    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    off_t end_offset = gzseek(gr, (off_t)0, 2);
    int gr_close = gzclose(gr);

    (void)bound_est;
    (void)comp_len;
    (void)comp_ret;
    (void)gw_write;
    (void)gw64_write;
    (void)gw_close;
    (void)gw64_close;
    (void)end_offset;
    (void)gr_close;
    // API sequence test completed successfully
    return 66;
}