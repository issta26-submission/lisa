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
//<ID> 259
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
    Bytef input_buf[] = "zlib API sequence test payload: deflate -> gz buffered write/read -> inflate with dictionary call.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[65536];
    Bytef comp_read[65536];
    Bytef outbuf[65536];
    const char *ver;
    gzFile gz;
    Bytef dict_buf[] = "preset-dict";
    int ret_deflate_init;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_gz_write;
    int ret_gz_close;
    int ret_gz_read;
    int ret_gz_close2;
    int ret_inflate_init;
    int ret_inflate_setdict;
    int ret_inflate_call;
    int ret_inflate_end;
    uLong comp_len;
    uLong read_len;
    uLong decomp_len;

    // step 2: Setup and initialization
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(comp_read, 0, sizeof(comp_read));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));

    // step 3: Operate - compress, write/read via gz with buffering, then inflate (with a call to set dictionary)
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    ret_deflate_call = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    ret_deflate_end = deflateEnd(&def_strm);

    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzbuffer(gz, 16384);
    ret_gz_write = gzwrite(gz, compbuf, (unsigned int)comp_len);
    ret_gz_close = gzclose(gz);

    gz = gzopen("test_zlib_api_sequence.gz", "rb");
    gzbuffer(gz, 4096);
    ret_gz_read = gzread(gz, comp_read, (unsigned int)comp_len);
    read_len = (uLong)(ret_gz_read >= 0 ? ret_gz_read : 0);
    ret_gz_close2 = gzclose(gz);

    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_read;
    inf_strm.avail_in = (uInt)read_len;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    ret_inflate_setdict = inflateSetDictionary(&inf_strm, dict_buf, (uInt)(sizeof(dict_buf) - 1));
    ret_inflate_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    ret_inflate_end = inflateEnd(&inf_strm);

    (void)ret_deflate_init;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_gz_write;
    (void)ret_gz_close;
    (void)ret_gz_read;
    (void)ret_gz_close2;
    (void)ret_inflate_init;
    (void)ret_inflate_setdict;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)comp_len;
    (void)read_len;
    (void)decomp_len;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}