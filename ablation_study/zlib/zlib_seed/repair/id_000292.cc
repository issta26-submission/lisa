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
//<ID> 292
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
    Bytef input_buf[] = "zlib API sequence: deflateTune -> deflate -> write gz -> seek64 -> inflate -> backend cleanup";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    uInt input_len_u = (uInt)input_len;
    Bytef compbuf[32768];
    Bytef outbuf[32768];
    const char *ver;
    uLong comp_len;
    uLong decomp_len;
    int ret_deflate_init;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_deflate_tune;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;
    int ret_inflate_backend;
    gzFile gz;
    off64_t seek_res;
    int gz_write_ret;
    int gz_buf_ret;

    // step 2: Setup / initialize
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));

    // step 3: Core operations - configure, compress, write to gz, seek, then inflate
    def_strm.next_in = input_buf;
    def_strm.avail_in = input_len_u;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    ret_deflate_tune = deflateTune(&def_strm, 4, 4, 8, 32);
    ret_deflate_call = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    ret_deflate_end = deflateEnd(&def_strm);

    gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    gz_buf_ret = gzbuffer(gz, 8192);
    gz_write_ret = gzwrite(gz, (voidpc)compbuf, (unsigned int)comp_len);
    seek_res = gzseek64(gz, (off64_t)0, 0);
    gzclose(gz);

    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    ret_inflate_backend = inflateBackEnd(&inf_strm);
    ret_inflate_end = inflateEnd(&inf_strm);

    (void)ret_deflate_init;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_deflate_tune;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)ret_inflate_backend;
    (void)comp_len;
    (void)decomp_len;
    (void)gz_write_ret;
    (void)gz_buf_ret;
    (void)seek_res;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}