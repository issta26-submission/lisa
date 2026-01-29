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
//<ID> 269
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
    Bytef input_buf[] = "zlib API sequence test payload: deflate -> write gz via gzputs -> inflate codes/backend.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[65536];
    Bytef outbuf[65536];
    const char *ver;
    gzFile gz;
    int ret_deflate_init;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_gz_puts;
    int ret_gz_eof;
    int ret_gz_close;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_back;
    int ret_inflate_end;
    uLong comp_len;
    uLong decomp_len;
    unsigned long codes_used;

    // step 2: Setup and initialize
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));

    // step 3: Operate - compress input with deflate, write original text to a gzip file via gzputs
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    ret_deflate_call = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    ret_deflate_end = deflateEnd(&def_strm);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_puts = gzputs(gz, (const char *)input_buf);
    ret_gz_eof = gzeof(gz);
    ret_gz_close = gzclose(gz);

    // step 4: Validate and cleanup - inflate the compressed data, query codes used and run inflateBackEnd
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    codes_used = inflateCodesUsed(&inf_strm);
    ret_inflate_back = inflateBackEnd(&inf_strm);
    ret_inflate_end = inflateEnd(&inf_strm);

    (void)ret_deflate_init;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_gz_puts;
    (void)ret_gz_eof;
    (void)ret_gz_close;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_back;
    (void)ret_inflate_end;
    (void)comp_len;
    (void)decomp_len;
    (void)codes_used;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}