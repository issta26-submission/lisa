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
//<ID> 590
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *ver = zlibVersion();
    Bytef plain_in[128];
    memset(plain_in, 'A', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    Bytef comp_buf[512];
    memset(comp_buf, 0, sizeof(comp_buf));
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    strm.next_in = plain_in;
    strm.avail_in = (uInt)plain_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)sizeof(comp_buf);

    // step 2: Configure
    int prime_ret = deflatePrime(&strm, 3, 5);
    int def_ret = deflate(&strm, 4);
    uLong comp_len = strm.total_out;

    // step 3: Operate
    gzFile gf = gzopen64("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gf, comp_buf, (unsigned int)comp_len);
    off64_t gw_pos = gzseek64(gf, 0, 1);

    // step 4: Validate / Cleanup
    int end_ret = deflateEnd(&strm);
    int gw_close = gzclose(gf);

    (void)ver;
    (void)init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)comp_len;
    (void)gw_write;
    (void)gw_pos;
    (void)end_ret;
    (void)gw_close;
    (void)plain_in;
    (void)comp_buf;
    // API sequence test completed successfully
    return 66;
}