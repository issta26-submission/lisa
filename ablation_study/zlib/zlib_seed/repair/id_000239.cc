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
//<ID> 239
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
    Bytef input_buf[] = "zlib API sequence test: deflate -> inflate, write/read via gz, use gzgetc and gzgetc_.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[65536];
    Bytef outbuf[65536];
    const char *ver;
    gzFile gzw;
    gzFile gzr;
    z_size_t read_items;
    uLong comp_len;
    uLong decomp_len;
    int ret_deflate_init;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;
    int ret_gzw_write;
    int ret_gzw_close;
    int ret_gzr_close;
    int ch_gzgetc;
    int ch_gzgetc_;

    // step 2: Setup and compress with deflate
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    ret_deflate_call = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    ret_deflate_end = deflateEnd(&def_strm);

    // step 3: write original data via gz, then read it back using gzfread and single-char reads
    gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gzw_write = gzwrite(gzw, (voidpc)input_buf, (unsigned int)input_len);
    ret_gzw_close = gzclose(gzw);
    gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    read_items = gzfread(outbuf, 1, (z_size_t)sizeof(outbuf), gzr);
    ch_gzgetc = gzgetc(gzr);
    ch_gzgetc_ = gzgetc_(gzr);
    ret_gzr_close = gzclose(gzr);

    // step 4: inflate the deflated buffer and cleanup
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    ret_inflate_end = inflateEnd(&inf_strm);

    (void)ret_deflate_init;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)ret_gzw_write;
    (void)ret_gzw_close;
    (void)ret_gzr_close;
    (void)read_items;
    (void)ch_gzgetc;
    (void)ch_gzgetc_;
    (void)comp_len;
    (void)decomp_len;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}