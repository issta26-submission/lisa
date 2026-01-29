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
//<ID> 341
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *ver;
    z_stream inf_strm;
    Bytef input_buf[] = "Zlib API sequence payload demonstrating compress->inflate and gz file buffering/IO.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[65536];
    Bytef decompbuf[65536];
    Bytef readbuf[65536];
    uLong comp_len = (uLong)sizeof(compbuf);
    int ret_compress;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;
    gzFile gzw;
    gzFile gzr;
    int gzbuffer_ret_w;
    int gzbuffer_ret_r;
    int gzwrite_ret;
    int gzread_ret;
    int gzeof_ret;
    int gzclose_r_ret;
    int gzclose_ret_w;

    // step 2: Setup
    ver = zlibVersion();
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(decompbuf, 0, sizeof(decompbuf));
    memset(readbuf, 0, sizeof(readbuf));
    ret_compress = compress2(compbuf, &comp_len, input_buf, input_len, 6);

    // step 3: Core operations - write compressed data to a gz file, read it back, then inflate the original compressed buffer
    gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzbuffer_ret_w = gzbuffer(gzw, 8192);
    gzwrite_ret = gzwrite(gzw, (voidpc)compbuf, (unsigned int)comp_len);
    gzclose_ret_w = gzclose(gzw);
    gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    gzbuffer_ret_r = gzbuffer(gzr, 4096);
    gzeof_ret = gzeof(gzr);
    gzread_ret = gzread(gzr, (voidp)readbuf, (unsigned int)sizeof(readbuf));
    gzclose_r_ret = gzclose_r(gzr);
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decompbuf;
    inf_strm.avail_out = (uInt)sizeof(decompbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    ret_inflate_end = inflateEnd(&inf_strm);

    // step 4: Cleanup / validation
    (void)ver;
    (void)input_len;
    (void)ret_compress;
    (void)comp_len;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)gzbuffer_ret_w;
    (void)gzbuffer_ret_r;
    (void)gzwrite_ret;
    (void)gzread_ret;
    (void)gzeof_ret;
    (void)gzclose_r_ret;
    (void)gzclose_ret_w;
    // API sequence test completed successfully
    return 66;
}