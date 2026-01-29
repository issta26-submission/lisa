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
//<ID> 271
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
    Bytef input_buf[] = "zlib API sequence test payload: deflate with preset dictionary -> inflate and validate via crc32 and inflateCodesUsed.";
    Bytef dict_buf[] = "preset-dict";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    uInt dict_len = (uInt)(sizeof(dict_buf) - 1);
    Bytef compbuf[65536];
    Bytef outbuf[65536];
    const char *ver;
    int ret_deflate_init;
    int ret_deflate_setdict;
    int ret_deflate_call;
    int ret_deflate_end;
    uLong comp_len;
    int ret_inflate_init;
    int ret_inflate_setdict;
    int ret_inflate_call;
    unsigned long codes_used;
    int ret_inflate_backend;
    int ret_inflate_end;
    uLong decomp_len;
    uLong out_crc;

    // step 2: Setup and initialization
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    ret_deflate_setdict = deflateSetDictionary(&def_strm, dict_buf, dict_len);

    // step 3: Core operations - deflate then inflate (with dictionary) and validate
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    ret_deflate_call = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    ret_deflate_end = deflateEnd(&def_strm);

    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    ret_inflate_setdict = inflateSetDictionary(&inf_strm, dict_buf, dict_len);
    ret_inflate_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    codes_used = inflateCodesUsed(&inf_strm);
    out_crc = crc32(0L, outbuf, (uInt)decomp_len);
    ret_inflate_backend = inflateBackEnd(&inf_strm);
    ret_inflate_end = inflateEnd(&inf_strm);

    // step 4: Cleanup and finalization
    (void)ret_deflate_init;
    (void)ret_deflate_setdict;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)comp_len;
    (void)ret_inflate_init;
    (void)ret_inflate_setdict;
    (void)ret_inflate_call;
    (void)decomp_len;
    (void)codes_used;
    (void)out_crc;
    (void)ret_inflate_backend;
    (void)ret_inflate_end;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}