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
//<ID> 244
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
    Bytef input_buf[] = "zlib API sequence test payload: initialize deflate, tune, compress, write via gz, seek64, then inflate back.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[65536];
    Bytef decompbuf[65536];
    const char *ver;
    gzFile gz;
    int ret_def_init;
    int ret_def_tune;
    int ret_def_call;
    int ret_def_end;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;
    int ret_gzbuffer;
    int ret_gz_write;
    int ret_gz_close;
    off64_t seek_ret;
    uLong comp_len;
    uLong decomp_len;

    // step 2: Setup / Initialize
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(decompbuf, 0, sizeof(decompbuf));
    ver = zlibVersion();
    ret_def_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    ret_def_tune = deflateTune(&def_strm, 4, 32, 16, 128);

    // step 3: Operate (compress, write via gz with buffer and seek64, then inflate)
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    ret_def_call = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    ret_def_end = deflateEnd(&def_strm);

    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gzbuffer = gzbuffer(gz, 8192);
    ret_gz_write = gzwrite(gz, (voidpc)input_buf, (unsigned int)input_len);
    seek_ret = gzseek64(gz, (off64_t)0, 0);
    ret_gz_close = gzclose(gz);

    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decompbuf;
    inf_strm.avail_out = (uInt)sizeof(decompbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    ret_inflate_end = inflateEnd(&inf_strm);

    // step 4: Cleanup and finalization
    (void)ret_def_init;
    (void)ret_def_tune;
    (void)ret_def_call;
    (void)ret_def_end;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)ret_gzbuffer;
    (void)ret_gz_write;
    (void)ret_gz_close;
    (void)seek_ret;
    (void)comp_len;
    (void)decomp_len;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}