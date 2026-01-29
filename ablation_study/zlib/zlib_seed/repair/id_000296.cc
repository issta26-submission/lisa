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
//<ID> 296
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
    Bytef input_buf[] = "zlib API sequence payload for deflate -> write gz -> seek64 -> inflate -> backend cleanup.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[65536];
    Bytef outbuf[65536];
    const char *ver;
    int ret_def_init;
    int ret_tune;
    int ret_def_call;
    int ret_def_end;
    uLong comp_len;
    gzFile gz;
    int gzbuf_ret;
    int gz_write_bytes;
    off64_t seek_ret;
    int ret_inflate_init;
    int ret_inflate_call;
    uLong decomp_len;
    int ret_inflate_backend;
    int ret_inflate_end;
    int gz_close_ret;

    // step 2: Setup / Initialization
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_def_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    ret_tune = deflateTune(&def_strm, 64, 32, 64, 128);
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);

    // step 3: Core operations - compress, write to gz, seek64, inflate, backend
    ret_def_call = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    ret_def_end = deflateEnd(&def_strm);
    gz = gzopen("test_zlib_api_sequence.gz", "wb+");
    gzbuf_ret = gzbuffer(gz, 16384);
    gz_write_bytes = gzwrite(gz, (voidpc)compbuf, (unsigned int)comp_len);
    seek_ret = gzseek64(gz, (off64_t)0, SEEK_SET);
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    ret_inflate_backend = inflateBackEnd(&inf_strm);

    // step 4: Cleanup / Validation
    ret_inflate_end = inflateEnd(&inf_strm);
    gz_close_ret = gzclose(gz);
    (void)ver;
    (void)ret_def_init;
    (void)ret_tune;
    (void)ret_def_call;
    (void)ret_def_end;
    (void)comp_len;
    (void)gzbuf_ret;
    (void)gz_write_bytes;
    (void)seek_ret;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)decomp_len;
    (void)ret_inflate_backend;
    (void)ret_inflate_end;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}