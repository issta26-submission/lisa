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
//<ID> 578
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef input_buf[128];
    memset(input_buf, 'A', sizeof(input_buf));
    uLong input_len = (uLong)sizeof(input_buf);
    Bytef comp_buf[512];
    memset(comp_buf, 0, sizeof(comp_buf));
    uLongf comp_len = (uLongf)sizeof(comp_buf);
    uLong bound_est = compressBound(input_len);

    // step 2: Setup / Configure
    int comp_ret = compress(comp_buf, &comp_len, input_buf, input_len);
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gw, comp_buf, (unsigned int)comp_len);
    off_t gw_seek_ret = gzseek(gw, (off_t)0, 0);
    int gw_flush = gzflush(gw, 0);
    int gw_close_ret = gzclose(gw);

    // step 3: Operate / Read back using gzopen64 and seek
    gzFile gr = gzopen64("test_zlib_api_sequence.tmp.gz", "rb");
    Bytef read_back[512];
    memset(read_back, 0, sizeof(read_back));
    int gr_read = gzread(gr, read_back, (unsigned int)sizeof(read_back));
    off_t gr_seek_ret = gzseek(gr, (off_t)0, 0);
    int gr_close_ret = gzclose(gr);

    // step 4: Validate / Cleanup
    (void)ver;
    (void)bound_est;
    (void)comp_ret;
    (void)comp_len;
    (void)gw_write;
    (void)gw_seek_ret;
    (void)gw_flush;
    (void)gw_close_ret;
    (void)gr_read;
    (void)gr_seek_ret;
    (void)gr_close_ret;
    // API sequence test completed successfully
    return 66;
}