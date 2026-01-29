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
//<ID> 177
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream def_strm;
    Bytef input_buf[] = "zlib deflate integration test payload.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef out_buf[512];
    uLong out_buf_len = (uLong)sizeof(out_buf);
    uLong bound_val;
    const char * ver;
    gzFile gz = (gzFile)0;
    off_t gz_off;
    int ret_def_init;
    int ret_def_reset;
    int ret_deflate;
    int ret_def_end;
    int ret_gz_close;

    // step 2: Setup
    memset(&def_strm, 0, sizeof(def_strm));
    memset(out_buf, 0, sizeof(out_buf));
    ver = zlibVersion();
    ret_def_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    ret_def_reset = deflateReset(&def_strm);

    // step 3: Core operations
    bound_val = deflateBound(&def_strm, input_len);
    def_strm.next_in = (Bytef *)input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = out_buf;
    def_strm.avail_out = (uInt)out_buf_len;
    ret_deflate = deflate(&def_strm, 4);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gz_off = gzoffset(gz);
    ret_gz_close = gzclose(gz);

    // step 4: Cleanup
    ret_def_end = deflateEnd(&def_strm);
    (void)ret_def_init;
    (void)ret_def_reset;
    (void)ret_deflate;
    (void)ret_def_end;
    (void)ret_gz_close;
    (void)bound_val;
    (void)gz_off;
    (void)ver;
    (void)out_buf_len;
    (void)def_strm.total_out;
    // API sequence test completed successfully
    return 66;
}