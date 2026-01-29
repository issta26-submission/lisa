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
//<ID> 277
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and zeroing
    z_stream def_strm;
    z_stream inf_strm;
    Bytef input_buf[] = "zlib API sequence test payload: compress -> crc32 -> inflate with preset dictionary.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef dict_buf[] = "preset-dict";
    uInt dict_len = (uInt)(sizeof(dict_buf) - 1);
    Bytef compbuf[65536];
    Bytef outbuf[65536];
    const char *ver;
    int ret_deflate_init;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_inflate_init;
    int ret_inflate_setdict;
    int ret_inflate_call;
    int ret_inflate_back;
    int ret_inflate_end;
    uLong comp_len;
    uLong decomp_len;
    unsigned long codes_used;
    uLong crc_val;

    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(outbuf, 0, sizeof(outbuf));

    // step 2: Initialize deflate and compress input
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    ret_deflate_call = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    ret_deflate_end = deflateEnd(&def_strm);

    // step 3: Compute CRC and initialize inflate, set dictionary
    crc_val = crc32(0UL, input_buf, (uInt)input_len);
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    ret_inflate_setdict = inflateSetDictionary(&inf_strm, dict_buf, dict_len);

    // step 4: Inflate, query codes used, backend and cleanup
    ret_inflate_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    codes_used = inflateCodesUsed(&inf_strm);
    ret_inflate_back = inflateBackEnd(&inf_strm);
    ret_inflate_end = inflateEnd(&inf_strm);

    (void)ret_deflate_init;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_inflate_init;
    (void)ret_inflate_setdict;
    (void)ret_inflate_call;
    (void)ret_inflate_back;
    (void)ret_inflate_end;
    (void)comp_len;
    (void)decomp_len;
    (void)codes_used;
    (void)crc_val;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}