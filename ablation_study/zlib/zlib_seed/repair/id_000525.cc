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
//<ID> 525
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef plain_in[256];
    memset(plain_in, 'Z', sizeof(plain_in));
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    int init_ret = deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef comp_buf[512];
    memset(comp_buf, 0, sizeof(comp_buf));
    dstrm.next_in = plain_in;
    dstrm.avail_in = (uInt)sizeof(plain_in);
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)sizeof(comp_buf);
    int prime_ret = deflatePrime(&dstrm, 3, 5);

    // step 3: Operate
    int def_ret = deflate(&dstrm, 4); /* 4 == Z_FINISH */
    uLong comp_len = dstrm.total_out;
    Bytef decomp_buf[256];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    uLongf decomp_len = (uLongf)sizeof(decomp_buf);
    int uncmp_ret = uncompress(decomp_buf, &decomp_len, comp_buf, comp_len);

    // step 4: Validate / Cleanup
    int end_ret = deflateEnd(&dstrm);
    (void)ver;
    (void)init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)comp_len;
    (void)uncmp_ret;
    (void)decomp_len;
    (void)end_ret;
    // API sequence test completed successfully
    return 66;
}