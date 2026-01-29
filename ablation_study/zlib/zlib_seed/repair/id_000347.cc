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
//<ID> 347
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
    Bytef input_buf[] = "Zlib API sequence payload for inflate/gz operations.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[16384];
    Bytef decompbuf[16384];
    Bytef readbuf[16384];
    uLongf comp_len_f;
    int ret_compress;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;
    gzFile gz_w;
    gzFile gz_r;
    int gzwrite_ret;
    int gzclose_w_ret;
    int gzbuffer_ret;
    int gzeof_ret;
    int gzread_ret;

    // step 2: Setup
    ver = zlibVersion();
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(decompbuf, 0, sizeof(decompbuf));
    memset(readbuf, 0, sizeof(readbuf));
    comp_len_f = (uLongf)sizeof(compbuf);
    ret_compress = compress2(compbuf, &comp_len_f, input_buf, input_len, 6);

    // step 3: Core operations
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len_f;
    inf_strm.next_out = decompbuf;
    inf_strm.avail_out = (uInt)sizeof(decompbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    ret_inflate_end = inflateEnd(&inf_strm);
    gz_w = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite_ret = gzwrite(gz_w, (voidpc)compbuf, (unsigned int)comp_len_f);
    gzclose_w_ret = gzclose_w(gz_w);
    gz_r = gzopen("test_zlib_api_sequence.gz", "rb");
    gzbuffer_ret = gzbuffer(gz_r, 4096);
    gzeof_ret = gzeof(gz_r);
    gzread_ret = gzread(gz_r, readbuf, (unsigned int)comp_len_f);
    gzclose_r(gz_r);

    // step 4: Cleanup / validation
    (void)ver;
    (void)input_len;
    (void)comp_len_f;
    (void)ret_compress;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)gzbuffer_ret;
    (void)gzeof_ret;
    (void)gzread_ret;
    // API sequence test completed successfully
    return 66;
}