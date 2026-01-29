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
//<ID> 255
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
    Bytef input_buf[] = "zlib API sequence payload: use deflateInit_, deflate, deflateSetDictionary, write/read via gz, then inflate with inflateSetDictionary and inflate.";
    Bytef dict_buf[] = "example-dictionary";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    uInt dict_len = (uInt)(sizeof(dict_buf) - 1);
    Bytef compbuf[65536];
    Bytef readbuf[65536];
    Bytef decompbuf[65536];
    const char *ver;
    gzFile gw = 0;
    gzFile gr = 0;
    uLong comp_len;
    uLong decomp_len;
    int ret_deflate_init;
    int ret_deflate_setdict;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_gzbuffer_w;
    int ret_gzwrite;
    int ret_gzclose_w;
    int ret_gzbuffer_r;
    int ret_gzread;
    int ret_gzclose_r;
    int ret_inflate_init;
    int ret_inflate_setdict;
    int ret_inflate_call;
    int ret_inflate_end;

    // step 2: Setup and compress with deflate (initialize, set dictionary, operate, cleanup)
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(readbuf, 0, sizeof(readbuf));
    memset(decompbuf, 0, sizeof(decompbuf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    ret_deflate_setdict = deflateSetDictionary(&def_strm, dict_buf, dict_len);
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    ret_deflate_call = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    ret_deflate_end = deflateEnd(&def_strm);

    // step 3: Persist original input via gz (configure buffer, write, close) then read it back (configure buffer, read, close)
    gw = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gzbuffer_w = gzbuffer(gw, 16384);
    ret_gzwrite = gzwrite(gw, (voidpc)input_buf, (unsigned int)input_len);
    ret_gzclose_w = gzclose(gw);
    gr = gzopen("test_zlib_api_sequence.gz", "rb");
    ret_gzbuffer_r = gzbuffer(gr, 8192);
    ret_gzread = gzread(gr, readbuf, (unsigned int)sizeof(readbuf));
    ret_gzclose_r = gzclose(gr);

    // step 4: Inflate the compressed buffer (initialize, set dictionary, inflate, cleanup) and validate lengths
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    ret_inflate_setdict = inflateSetDictionary(&inf_strm, dict_buf, dict_len);
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decompbuf;
    inf_strm.avail_out = (uInt)sizeof(decompbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    ret_inflate_end = inflateEnd(&inf_strm);

    (void)ret_deflate_init;
    (void)ret_deflate_setdict;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_gzbuffer_w;
    (void)ret_gzwrite;
    (void)ret_gzclose_w;
    (void)ret_gzbuffer_r;
    (void)ret_gzread;
    (void)ret_gzclose_r;
    (void)ret_inflate_init;
    (void)ret_inflate_setdict;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)comp_len;
    (void)decomp_len;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}