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
//<ID> 173
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream def_strm;
    Bytef input_buf[] = "zlib deflate roundtrip payload for gzwrite/gzoffset test.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef out_buf[512];
    const char * ver;
    gzFile gz;
    uLong bound;
    off_t off;
    int ret_def_init;
    int ret_def_reset;
    int ret_def;
    int ret_def_end;
    int ret_gz_write;
    int ret_gz_flush;
    int ret_gz_close;

    // step 2: Setup / Initialize
    memset(&def_strm, 0, sizeof(def_strm));
    memset(out_buf, 0, sizeof(out_buf));
    ver = zlibVersion();
    ret_def_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = out_buf;
    def_strm.avail_out = (uInt)sizeof(out_buf);

    // step 3: Configure / Operate
    ret_def_reset = deflateReset(&def_strm);
    bound = deflateBound(&def_strm, input_len);
    ret_def = deflate(&def_strm, 4); /* Z_FINISH = 4 */
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, (voidpc)out_buf, (unsigned int)def_strm.total_out);
    ret_gz_flush = gzflush(gz, 1);
    off = gzoffset(gz);
    ret_gz_close = gzclose(gz);

    // step 4: Validate / Cleanup
    ret_def_end = deflateEnd(&def_strm);
    (void)ret_def_init;
    (void)ret_def_reset;
    (void)bound;
    (void)ret_def;
    (void)ret_gz_write;
    (void)ret_gz_flush;
    (void)off;
    (void)ret_gz_close;
    (void)ret_def_end;
    (void)ver;
    (void)input_len;
    (void)def_strm.total_out;
    // API sequence test completed successfully
    return 66;
}