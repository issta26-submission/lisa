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
//<ID> 246
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
    Bytef input_buf[] = "zlib sequence test: init -> tune -> deflate -> write gz -> seek64 -> inflate -> cleanup";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[65536];
    Bytef outbuf[65536];
    const char *ver;
    gzFile gzw;
    gzFile gzr;
    int ret_deflate_init;
    int ret_deflate_tune;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_gzbuffer_w;
    int ret_gzbuffer_r;
    off64_t ret_gzseek64;
    int ret_gzwrite;
    int ret_gzclose_w;
    int ret_gzclose_r;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;
    uLong comp_len;
    uLong decomp_len;

    // step 2: Setup / Initialize streams and buffers
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    ret_deflate_tune = deflateTune(&def_strm, 4, 32, 16, 128);

    // step 3: Core operations: deflate, write gz, use gzseek64, then inflate
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    ret_deflate_call = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    ret_deflate_end = deflateEnd(&def_strm);

    gzw = gzopen64("test_zlib_api_sequence.gz", "wb");
    ret_gzbuffer_w = gzbuffer(gzw, 16384);
    ret_gzwrite = gzwrite(gzw, (voidpc)input_buf, (unsigned int)input_len);
    ret_gzclose_w = gzclose(gzw);

    gzr = gzopen64("test_zlib_api_sequence.gz", "rb");
    ret_gzbuffer_r = gzbuffer(gzr, 8192);
    ret_gzseek64 = gzseek64(gzr, (off64_t)0, 0);

    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    ret_inflate_end = inflateEnd(&inf_strm);

    ret_gzclose_r = gzclose(gzr);

    // step 4: Validation and cleanup
    (void)ret_deflate_init;
    (void)ret_deflate_tune;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_gzbuffer_w;
    (void)ret_gzbuffer_r;
    (void)ret_gzwrite;
    (void)ret_gzclose_w;
    (void)ret_gzclose_r;
    (void)ret_gzseek64;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)comp_len;
    (void)decomp_len;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}