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
//<ID> 240
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream def_strm;
    z_stream inf_strm;
    Bytef input_buf[] = "zlib API sequence test payload: init deflate, tune it, compress, write via gz with buffer and seek64, then inflate back.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[65536];
    Bytef outbuf[65536];
    const char *ver;
    gzFile gz;
    off64_t seekpos;
    uLong comp_len;
    uLong decomp_len;
    int ret_def_init;
    int ret_def_tune;
    int ret_def_call;
    int ret_def_end;
    int ret_gzbuffer;
    int ret_gzwrite;
    int ret_gzclose;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;

    // step 2: Setup / Initialize
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_def_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    ret_def_tune = deflateTune(&def_strm, 20, 10, 30, 512);

    // step 3: Operate (deflate -> write gz with buffer and seek64 -> inflate)
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    ret_def_call = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    ret_def_end = deflateEnd(&def_strm);

    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gzbuffer = gzbuffer(gz, 8192);
    ret_gzwrite = gzwrite(gz, (voidpc)input_buf, (unsigned int)input_len);
    seekpos = gzseek64(gz, (off64_t)0, 0);
    ret_gzclose = gzclose(gz);

    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    ret_inflate_end = inflateEnd(&inf_strm);

    // step 4: Cleanup / finalize
    (void)ret_def_init;
    (void)ret_def_tune;
    (void)ret_def_call;
    (void)ret_def_end;
    (void)ret_gzbuffer;
    (void)ret_gzwrite;
    (void)ret_gzclose;
    (void)seekpos;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)comp_len;
    (void)decomp_len;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}