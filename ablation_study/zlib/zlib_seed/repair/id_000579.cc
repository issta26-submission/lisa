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
//<ID> 579
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *version = zlibVersion();
    Bytef plain_in[200];
    memset(plain_in, 'P', sizeof(plain_in));
    uLong source_len = (uLong)sizeof(plain_in);
    Bytef comp_buf[1024];
    memset(comp_buf, 0, sizeof(comp_buf));
    uLong bound_est = compressBound(source_len);
    uLongf comp_len = (uLongf)sizeof(comp_buf);
    int comp_ret = compress(comp_buf, &comp_len, plain_in, source_len);

    // step 2: Configure
    gzFile gw_plain = gzopen("test_plain.gz", "wb");
    int gw_plain_write = gzwrite(gw_plain, plain_in, (unsigned int)sizeof(plain_in));
    gzFile gw_comp = gzopen64("test_comp.gz", "wb");
    int gw_comp_write = gzwrite(gw_comp, comp_buf, (unsigned int)comp_len);

    // step 3: Operate
    off_t gw_plain_pos = gzseek(gw_plain, 0, 1);
    off_t gw_comp_pos = gzseek(gw_comp, 0, 1);
    off_t gw_plain_rewind = gzseek(gw_plain, 0, 0);
    off_t gw_comp_rewind = gzseek(gw_comp, 0, 0);

    // step 4: Validate / Cleanup
    int gw_plain_close = gzclose(gw_plain);
    int gw_comp_close = gzclose(gw_comp);
    gzFile gr_plain = gzopen("test_plain.gz", "rb");
    Bytef read_back[200];
    memset(read_back, 0, sizeof(read_back));
    int gr_plain_read = gzread(gr_plain, read_back, (unsigned int)sizeof(read_back));
    int gr_plain_close = gzclose(gr_plain);

    (void)version;
    (void)bound_est;
    (void)comp_ret;
    (void)gw_plain_write;
    (void)gw_comp_write;
    (void)gw_plain_pos;
    (void)gw_comp_pos;
    (void)gw_plain_rewind;
    (void)gw_comp_rewind;
    (void)gw_plain_close;
    (void)gw_comp_close;
    (void)gr_plain_read;
    (void)gr_plain_close;
    (void)read_back;
    // API sequence test completed successfully
    return 66;
}