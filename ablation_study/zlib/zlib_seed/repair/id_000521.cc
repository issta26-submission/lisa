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
//<ID> 521
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
    memset(plain_in, 'P', sizeof(plain_in));
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    int prime_ret = deflatePrime(&strm, 3, 5);

    // step 2: Configure
    Bytef comp_buf[512];
    memset(comp_buf, 0, sizeof(comp_buf));
    strm.next_in = plain_in;
    strm.avail_in = (uInt)sizeof(plain_in);
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)sizeof(comp_buf);

    // step 3: Operate
    int def_ret = deflate(&strm, 4); /* 4 == Z_FINISH */
    uLong compressed_len = strm.total_out;
    Bytef decomp_buf[256];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    uLongf decomp_len = (uLongf)sizeof(decomp_buf);
    int uncompress_ret = uncompress(decomp_buf, &decomp_len, comp_buf, compressed_len);
    uLong adler_orig = adler32(0UL, plain_in, (uInt)sizeof(plain_in));
    uLong adler_decomp = adler32(0UL, decomp_buf, (uInt)decomp_len);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&strm);

    (void)ver;
    (void)init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)compressed_len;
    (void)uncompress_ret;
    (void)adler_orig;
    (void)adler_decomp;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}