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
//<ID> 290
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
    Bytef input_buf[] = "zlib API sequence payload: deflateTune -> deflate -> inflate -> inflateBackEnd -> gzbuffer/gzseek64.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    uInt input_len_u = (uInt)input_len;
    Bytef compbuf[65536];
    Bytef outbuf[65536];
    const char *ver;
    uLong comp_len;
    uLong decomp_len;
    int ret_def_init;
    int ret_def_call;
    int ret_def_tune;
    int ret_def_end;
    int ret_inf_init;
    int ret_inf_call;
    int ret_inf_backend;
    int ret_inf_end;
    gzFile gz;
    int gzbuf_ret;
    off64_t gzpos;
    int gz_write_ret;

    // step 2: Setup / initialization
    ver = zlibVersion();
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(outbuf, 0, sizeof(outbuf));
    ret_def_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    ret_def_tune = deflateTune(&def_strm, 4, 4, 16, 32);

    // step 3: Core operations - deflate then inflate and backend
    def_strm.next_in = input_buf;
    def_strm.avail_in = input_len_u;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    ret_def_call = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    ret_def_end = deflateEnd(&def_strm);

    ret_inf_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    ret_inf_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    ret_inf_backend = inflateBackEnd(&inf_strm);
    ret_inf_end = inflateEnd(&inf_strm);

    // step 4: File operations and cleanup
    gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    gzbuf_ret = gzbuffer(gz, 8192u);
    gz_write_ret = gzwrite(gz, outbuf, (unsigned int)decomp_len);
    gzpos = gzseek64(gz, (off64_t)0, 0);
    gzclose(gz);

    (void)ret_def_init;
    (void)ret_def_call;
    (void)ret_def_tune;
    (void)ret_def_end;
    (void)ret_inf_init;
    (void)ret_inf_call;
    (void)ret_inf_backend;
    (void)ret_inf_end;
    (void)comp_len;
    (void)decomp_len;
    (void)ver;
    (void)gzbuf_ret;
    (void)gzpos;
    (void)gz_write_ret;
    // API sequence test completed successfully
    return 66;
}