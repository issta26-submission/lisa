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
//<ID> 529
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
    memset(plain_in, 'A', sizeof(plain_in));
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    int def_prime_ret = deflatePrime(&def_strm, 3, 5);

    // step 2: Configure
    Bytef comp_buf[512];
    memset(comp_buf, 0, sizeof(comp_buf));
    def_strm.next_in = plain_in;
    def_strm.avail_in = (uInt)sizeof(plain_in);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)sizeof(comp_buf);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong compressed_len = def_strm.total_out;
    Bytef decomp_buf[256];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    uLongf decomp_len = (uLongf)sizeof(decomp_buf);
    int uncompress_ret = uncompress(decomp_buf, &decomp_len, comp_buf, compressed_len);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def_strm);

    (void)ver;
    (void)def_init_ret;
    (void)def_prime_ret;
    (void)def_ret;
    (void)compressed_len;
    (void)decomp_len;
    (void)uncompress_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}