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
//<ID> 596
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain_in[256];
    memset(plain_in, 'Z', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    Bytef comp_buf[2048];
    memset(comp_buf, 0, sizeof(comp_buf));
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    def_strm.next_in = plain_in;
    def_strm.avail_in = (uInt)plain_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)sizeof(comp_buf);

    // step 2: Configure
    int prime_ret = deflatePrime(&def_strm, 3, 5); /* prime the bit buffer with 3 bits == value 5 */

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong comp_len = def_strm.total_out;
    gzFile gz = gzopen64("test_zlib_api_sequence_output.gz", "wb");
    int gz_write_ret = gzwrite(gz, plain_in, (unsigned int)plain_len);
    off64_t gz_pos = gzseek64(gz, 0, 1); /* get current position */

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    int gz_close_ret = gzclose(gz);

    (void)version;
    (void)def_init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)comp_len;
    (void)gz_write_ret;
    (void)gz_pos;
    (void)def_end_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}