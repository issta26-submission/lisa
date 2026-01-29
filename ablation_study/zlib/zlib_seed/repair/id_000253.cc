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
//<ID> 253
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
    Bytef input_buf[] = "zlib API sequence test: use deflate with dictionary, write/read compressed blob via gz, then inflate supplying dictionary.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef dict_buf[] = "preset_dictionary_for_zlib";
    uInt dict_len = (uInt)(sizeof(dict_buf) - 1);
    Bytef compbuf[65536];
    Bytef outbuf[65536];
    const char *ver;
    gzFile gz;
    int ret_deflate_init;
    int ret_deflate_setdict;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_gzw_write;
    int ret_gzw_close;
    int ret_gzr_read;
    int ret_gzr_close;
    int ret_inflate_init;
    int ret_inflate_call1;
    int ret_inflate_setdict;
    int ret_inflate_call2;
    int ret_inflate_end;
    uLong comp_len;
    uLong read_len;
    uLong decomp_len;

    // step 2: Setup and compress using deflate with a preset dictionary
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(outbuf, 0, sizeof(outbuf));
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

    // step 3: Persist compressed data via gz, then read it back (use gzbuffer)
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzbuffer(gz, 16384);
    ret_gzw_write = gzwrite(gz, (voidpc)compbuf, (unsigned int)comp_len);
    ret_gzw_close = gzclose(gz);
    gz = gzopen("test_zlib_api_sequence.gz", "rb");
    gzbuffer(gz, 8192);
    ret_gzr_read = gzread(gz, compbuf, (unsigned int)comp_len);
    read_len = (uLong)ret_gzr_read;
    ret_gzr_close = gzclose(gz);

    // step 4: Inflate the read compressed data, supply the dictionary if needed, and cleanup
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)read_len;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    ret_inflate_call1 = inflate(&inf_strm, 4);
    ret_inflate_setdict = inflateSetDictionary(&inf_strm, dict_buf, dict_len);
    ret_inflate_call2 = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    ret_inflate_end = inflateEnd(&inf_strm);

    (void)ret_deflate_init;
    (void)ret_deflate_setdict;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_gzw_write;
    (void)ret_gzw_close;
    (void)ret_gzr_read;
    (void)ret_gzr_close;
    (void)ret_inflate_init;
    (void)ret_inflate_call1;
    (void)ret_inflate_setdict;
    (void)ret_inflate_call2;
    (void)ret_inflate_end;
    (void)comp_len;
    (void)read_len;
    (void)decomp_len;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}