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
//<ID> 243
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
    Bytef input_buf[] = "zlib API sequence test payload: deflateTune + gzbuffer + gzseek64 + inflate.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[65536];
    Bytef readbuf[65536];
    Bytef outbuf[65536];
    const char *ver;
    gzFile gw;
    gzFile gr;
    off64_t seek_ret;
    uLong comp_len;
    uLong decomp_len;
    int ret_deflate_init;
    int ret_deflate_tune;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_gzbuffer;
    int ret_gzwrite;
    int ret_gzclose;
    int ret_gzread;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;

    // step 2: Setup and initialization
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(readbuf, 0, sizeof(readbuf));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    ret_deflate_tune = deflateTune(&def_strm, 64, 32, 258, 128);

    // step 3: Operate - deflate, write compressed to gz, seek and read it back
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    ret_deflate_call = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    ret_deflate_end = deflateEnd(&def_strm);

    gw = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gzbuffer = gzbuffer(gw, 16384);
    ret_gzwrite = gzwrite(gw, (voidpc)compbuf, (unsigned int)comp_len);
    ret_gzclose = gzclose(gw);

    gr = gzopen("test_zlib_api_sequence.gz", "rb");
    ret_gzbuffer = gzbuffer(gr, 8192);
    seek_ret = gzseek64(gr, (off64_t)0, 0);
    ret_gzread = gzread(gr, (voidp)readbuf, (unsigned int)comp_len);
    ret_gzclose = gzclose(gr);

    // step 4: Validate - inflate the read compressed data and cleanup
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = readbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    ret_inflate_end = inflateEnd(&inf_strm);

    (void)ret_deflate_init;
    (void)ret_deflate_tune;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_gzbuffer;
    (void)ret_gzwrite;
    (void)ret_gzclose;
    (void)ret_gzread;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)seek_ret;
    (void)comp_len;
    (void)decomp_len;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}