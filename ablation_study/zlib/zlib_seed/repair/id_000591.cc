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
//<ID> 591
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef input_buf[300];
    memset(input_buf, 'A', sizeof(input_buf));
    uLong input_len = (uLong)sizeof(input_buf);
    Bytef comp_buf[1024];
    memset(comp_buf, 0, sizeof(comp_buf));
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)sizeof(comp_buf);

    // step 2: Configure
    int prime_ret = deflatePrime(&def_strm, 3, 5);
    (void)prime_ret;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong comp_len = def_strm.total_out;
    gzFile out_gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    int write_ret = gzwrite(out_gz, comp_buf, (unsigned int)comp_len);
    off64_t cur_pos = gzseek64(out_gz, 0, 1); /* whence=1 -> current position */
    int close_ret = gzclose(out_gz);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    (void)ver;
    (void)init_ret;
    (void)def_ret;
    (void)comp_len;
    (void)write_ret;
    (void)cur_pos;
    (void)close_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}