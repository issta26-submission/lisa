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
//<ID> 344
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
    Bytef input_buf[] = "Zlib API sequence payload: compress with compress2, inflate with inflateInit_, write/read gz file, test gzbuffer/gzeof/gzclose_r.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    uLongf comp_len_f = 0;
    uLong decomp_len = 0;
    uInt avail_in_u;
    Bytef compbuf[16384];
    Bytef decompbuf[16384];
    int ret_compress;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;
    gzFile gzw;
    gzFile gzr;
    int ret_gzbuffer_w;
    int ret_gzbuffer_r;
    int ret_gzwrite;
    int ret_gzflush;
    int ret_gzclose_w;
    int ret_gzclose_r;
    int ret_gzeof;

    // step 2: Setup / initialization
    ver = zlibVersion();
    memset(compbuf, 0, sizeof(compbuf));
    memset(decompbuf, 0, sizeof(decompbuf));
    comp_len_f = (uLongf)sizeof(compbuf);
    ret_compress = compress2(compbuf, &comp_len_f, (const Bytef *)input_buf, (uLong)input_len, 6);
    memset(&inf_strm, 0, sizeof(inf_strm));
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    avail_in_u = (uInt)comp_len_f;
    inf_strm.avail_in = avail_in_u;
    inf_strm.next_out = decompbuf;
    inf_strm.avail_out = (uInt)sizeof(decompbuf);

    // step 3: Core operations - inflate compressed buffer, write to gz, read back, use gzbuffer/gzeof/gzclose_r
    ret_inflate_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    ret_inflate_end = inflateEnd(&inf_strm);
    gzw = gzopen("test_zlib_api_sequence_out.gz", "wb");
    ret_gzbuffer_w = gzbuffer(gzw, 16384);
    ret_gzwrite = gzwrite(gzw, (voidpc)decompbuf, (unsigned int)decomp_len);
    ret_gzflush = gzflush(gzw, 2);
    ret_gzclose_w = gzclose(gzw);
    gzr = gzopen("test_zlib_api_sequence_out.gz", "rb");
    ret_gzbuffer_r = gzbuffer(gzr, 8192);
    ret_gzeof = gzeof(gzr);
    ret_gzclose_r = gzclose_r(gzr);

    // step 4: Validation / cleanup
    (void)ver;
    (void)input_len;
    (void)comp_len_f;
    (void)decomp_len;
    (void)ret_compress;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)ret_gzbuffer_w;
    (void)ret_gzbuffer_r;
    (void)ret_gzwrite;
    (void)ret_gzflush;
    (void)ret_gzclose_w;
    (void)ret_gzclose_r;
    (void)ret_gzeof;
    // API sequence test completed successfully
    return 66;
}